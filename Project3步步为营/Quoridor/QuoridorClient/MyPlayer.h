#pragma once
#include "Player.h"
#include <deque>
#include "AStar.h"

namespace QuoridorUtils {
    class MyPlayer final : public Player {
    private:
        std::deque<BlockBar> blocks; //存放当前所有的板子
        std::deque<BlockBar> myblocks;
        std::deque<BlockBar> enblocks;
        Location me;
        Location en;
        int melength;
        int enlength;
        int targetY = 0; //目标的y
        bool first; //判断是否是先手
        void Move(Location& now, int dir); //移动棋子
        bool Jump(Location& now, int dir); //跳跃过敌方棋子
        bool FindSteel(Location& cur, Location& pcur);//判断从pcur到cur是否能够正确达到
        bool FindEdge(Location& cur);//判断cur是否处于边界
        bool IfRulebreak(BlockBar block);//判断block这块板子是否是犯规的
        BlockBar AddBlockBar(Location enemyLoc, Location myLoc); //通过对当下局势的分析在具体位置加上木板
        std::pair<int, int> Search(Location cur,int desy, Location en);//从cur到desy的路径的搜索，en表示敌方位置，pair的first代表是长度，second代表的是方向
        int alphaBeta(int depth, int playerId, int alpha, int beta);//博弈树的剪枝
        int res = 10;
        int evaluate();//评估函数
        std::deque<BlockBar> possibleblock(Location mylocation, Location enlocation, int targety,int originenl);//可能木板的集合，用于博弈树
        int max(int a, int b) { return a > b ? a : b; }//取最大值
        int min(int a, int b) { return a < b ? a : b; }//取最小值
        bool illegalMove(Location cur, int dir);//判断cur在dir方向上是否非法走路 0，1，2，3的含义见A*文件
    public:
        MyPlayer(const std::string& key) : Player(key) {};            // 必须存在, 请勿修改 
        Step nextStep(const ChessboardChange& newChange) override;    // 必须自行实现 
        void restart() override;                                      // 必须自行实现 
    };
}

/*
 *
 * 棋子坐标如下:                 挡板坐标结构如下：
 * ┌-┬-┬-┬-┬-┬-┬-┬-┬-┐          ┌-┬-┬-┬-┬-┬-┬-┬-┬-┐9
 * ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖9       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤8
 * ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖8       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤7
 * ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖7       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤6
 * ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖6       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤5
 * ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖5       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤4
 * ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖4       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤3
 * ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖3       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤2
 * ├-┼-┼-┼-┼-┼-┼-┼-┼-┤↖2       ├-┼-┼-┼-┼-┼-┼-┼-┼-┤1
 * └-┴-┴-┴-┴-┴-┴-┴-┴-┘↖1 y     └-┴-┴-┴-┴-┴-┴-┴-┴-┘0 y
 * x 1  2  3  4  5  6  7  8  9          x  0  1  2  3  4  5  6  7  8  9
 *
 *
 */