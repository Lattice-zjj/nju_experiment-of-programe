#include <iostream>
#include <fstream>
#include <thread>
#include "NetworkClient.h"
#include "Configuration.h"
#include "MyPlayer.h"
#include "nlohmann/json.hpp"

std::string getConfig(int argc, char* argv[]);
void playGame(const nlohmann::json& config, QuoridorUtils::Player& player, 
              std::vector<std::string>& logFiles, bool saveLogFile = true);
void reviewGame(QuoridorUtils::GameStatus status, const std::string& cmd = "", const std::string& logFilename = "");


int main(int argc, char* argv[]) {
    // 1. 读配置文件, 默认配置文件为当前目录下的config.json 
    // 可以输入参数读取其他的配置文件, 详见 getConfig() 函数 
    const auto configFilename = getConfig(argc, argv);
    nlohmann::json config;
    std::ifstream configFile(configFilename);
    if (configFile.is_open()) {
        configFile >> config;
        configFile.close();
    } else {
        std::cerr << configFilename << " don't exist!" << std::endl;
        return -1;
    }
    // 2. 新建玩家并开始游戏 
    std::vector<std::string> logFiles;
    // 单玩家 
    QuoridorUtils::MyPlayer myPlayer(config.value("playerKey", ""));
    playGame(config, myPlayer, logFiles, config.value("saveLog", true));
    // 多玩家 
    // ...
    // 3. 游戏回放 
    if (config.value("gameReplay", false)) {
        for (auto logFile : logFiles) {
            reviewGame(QuoridorUtils::GameStatus::Ok, config.value("gameReplayCmd", "./QuoridorUI  file%"),
                       logFile);
        }
    }
    return 0;
}

void playGame(const nlohmann::json& config, QuoridorUtils::Player& player, 
              std::vector<std::string>& logFiles, const bool saveLogFile) {
    // 2.1. 连接网络, 默认连接 127.0.0.1:5620 
    NetworkClient* network = nullptr;
    if (config.contains("network")) {
        const auto networkConfig = config.value("network", "{ \"host\": \"127.0.0.1\", \"port\": 5620 }"_json);
        network = new NetworkClient(networkConfig.value("host", "127.0.0.1"),
                                    networkConfig.value("port", 5620));
    } else {
        network = new NetworkClient();
    }
    if (network->getStatus() != NetworkStatus::Ok) {
        std::cout << "Your network connect error! pls check your network." << std::endl;
        delete(network);
        return ;
    }
    // 2.2. 开始游戏 
    QuoridorUtils::Configuration quoridor(network);
    quoridor.startGame(player, logFiles, saveLogFile);
    // 2.3. 游戏结果输出并关闭网络 
    delete(network);
}

void reviewGame(const QuoridorUtils::GameStatus status, const std::string& cmd, const std::string& logFilename) {
    if (status != QuoridorUtils::GameStatus::None) {
        //std::string logFilename = quoridor.saveGameHistory();
        if (!logFilename.empty() && !cmd.empty()) { // config.value("gameReplay", false)
            std::string cmdTmpStr = cmd;  //
            size_t pos = cmdTmpStr.find("%file%");
            cmdTmpStr.replace(pos, 6, logFilename);
#if defined(_WIN32) || defined(_WIN64)
            do {
                pos = cmdTmpStr.find('/');
                if (pos != std::string::npos) {
                    cmdTmpStr.replace(pos, 1, "\\");
                }
            } while (pos == std::string::npos);
#endif
            system(cmdTmpStr.data());
        }
    }
}

std::string getConfig(int argc, char* argv[]) {
    if (argc == 2) {
        return argv[1];
    } else {
        return "config.json";
    }
}
