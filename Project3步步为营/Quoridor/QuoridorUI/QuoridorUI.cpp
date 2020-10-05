#include <iostream>
#include <fstream>
#include "ConsoleView.h"
#include "LogLoader.h"
#include "Replayer.h"
#include "nlohmann/json.hpp"

nlohmann::json getConfig(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    nlohmann::json config = getConfig(argc, argv);
    QuoridorUtils::ConsoleView view(config["displayConfig"]);
    QuoridorUtils::LogLoader logLoader(config["logFile"]);
    QuoridorUtils::Replayer replayer;
    replayer.start(view, logLoader);
    return 0;
}

nlohmann::json getConfig(int argc, char* argv[]) {
    std::string configFilename;
    std::string csvFilename;
    if (argc == 1) {
        configFilename.assign("uiconfig.json");
    } else if (argc == 2) {
        std::string tmp(argv[1]);
        if (tmp.length() > 4 && tmp.substr(tmp.length() - 4, 4) == "json") {
            configFilename.assign(argv[1]);
        } else {
            configFilename.assign("uiconfig.json");
            csvFilename.assign(argv[1]);
        }
    } else if (argc == 3) {
        std::string tmp(argv[1]);
        if (tmp.length() > 4 && tmp.substr(tmp.length() - 4, 4) == "json") {
            configFilename.assign(argv[1]);
            csvFilename.assign(argv[2]);
        } else {
            configFilename.assign(argv[2]);
            csvFilename.assign(argv[1]);
        }
    } else {
        std::cout << "QuoridorUI [csvLogFile.csv] [uiconfig.json]" << std::endl;
        exit(-1);
    }

    nlohmann::json config;
    std::ifstream configFile(configFilename);
    if (configFile.is_open()) {
        configFile >> config;
        configFile.close();
    } else {
        std::cerr << configFilename << " don't exist!" << std::endl;
        exit(-1);
    }
    if (!csvFilename.empty()) {
        config["logFile"] = csvFilename;
    }
    return config;
}
