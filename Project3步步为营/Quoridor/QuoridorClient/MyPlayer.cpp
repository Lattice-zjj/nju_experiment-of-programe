#include "MyPlayer.h"
#include <chrono>
#include <iostream>

#include <cstdlib>
#include <ctime>

namespace QuoridorUtils {
    /*
     * Step MyPlayer::nextStep(const ChessboardChange& newChange)
     *
     * newChange 为修改的棋局, 其格式为
     * (GameStatus status, Location enemyLoc, Location myLoc, BlockBar newEnemyBlockBar)
     * 其中棋子坐标和挡板坐标见下图, 当对手没有添加新的挡板时,
     * newChange.newEnemyBlockBar.isNan()为true
     *
     * 需要返回的数据Step为
     * (Location myNewLoc, BlockBar myNewBlockBar)
     * 仅能选择让移动棋子一步（前后左右四个方向）或者放一个挡板:
     * 当移动棋子时, step.myNewBlockBar.isNan()为true;
     * 当放置挡板时, step.myNewLoc为newChange中的myLoc
     *
     * 所有错误(挡板位置错误, 挡板已用完, 移动位置错误, 同时移动并放置挡板, 超时等)均由服务器检测,
     * 可能需要在自行实现错误避免功能.
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
     */

    

    void MyPlayer::Move(Location& now, int dir)
    {
        if (dir == 0) now.y += 1;
        else if (dir == 1) now.y -= 1;
        else if (dir == 2) now.x -= 1;
        else if (dir == 3) now.x += 1;
    }

    bool MyPlayer::FindSteel(Location& cur, Location& pcur)
    {
        for (auto block : blocks) {                        // 遍历挡板列表, 找到被阻挡的方向 
            if (block.isH()) {                                   // 水平方向挡板, start 与 stop 的 y 相等 
                if (block.start.x == pcur.x - 1 ||
                    block.start.x == pcur.x - 2) {              // 可能挡路的挡板 
                    if (block.start.y == pcur.y) {
                        if (cur.x == pcur.x && cur.y == pcur.y + 1) return true;
                    }
                    else if (block.start.y == pcur.y - 1) {
                        if (cur.x == pcur.x && cur.y == pcur.y - 1) return true;
                    }
                }
            }
            if (block.isV()) {                                   // 竖直方向挡板, start 与 stop 的 x 相等 
                if (block.start.y == pcur.y - 1 ||
                    block.start.y == pcur.y - 2) {              // 可能挡路的挡板 
                    if (block.start.x == pcur.x) {
                        if (cur.x == pcur.x + 1 && cur.y == pcur.y) return true;
                    }
                    else if (block.start.x == pcur.x - 1) {
                        if (cur.x == pcur.x - 1 && cur.y == pcur.y) return true;
                    }
                }
            }
        }
        return false;
    }

    bool MyPlayer::FindEdge(Location& cur)
    {
        if (cur.x<1 || cur.y>SIZE || cur.x > SIZE || cur.y < 1) return true; else return false;
    }

    bool MyPlayer::IfRulebreak(BlockBar b)
    {
        for (auto block : blocks) {
            if (block == b) return true;
            if (block.isH() && b.isH()) {
                if (block.start.y == b.start.y) {
                    if (block.start.x == b.start.x - 1 || block.start.x == b.start.x + 1) return true;
                }
            }
            else if (block.isV() && b.isV()) {
                if (block.start.x == b.start.x) {
                    if (block.start.y == b.start.y - 1 || block.start.y == b.start.y + 1) return true;
                }
            }
        }
        return false;
    }

    BlockBar MyPlayer::AddBlockBar(Location enemyLoc,Location myLoc)
    {
        
        
        int max = 0;
        BlockBar out;
        std::pair<int, int> ansme, ansen;
        for (int a = 0; a <= 7; a++) {
            for (int b = 0; b <= 7; b++) {
                for (int k = 0; k <= 1; k++) {
                    if (a == 0 && k == 0 || b == 0 && k == 1) continue;
                    Location start(a, b), end(a + (k * 2), b + 2 * (1 - k));
                    BlockBar block(start, end);
                    if (IfRulebreak(block)) continue;
                    blocks.push_back(block);
                    ansen = Search(enemyLoc, 10 - targetY,myLoc);
                    ansme = Search(myLoc, targetY, enemyLoc);
                    if (a == 0 && b == 1 && k == 1) std::cout << ansme.first << " " << ansen.first << std::endl;
                    if (ansme.first != -1 && ansen.first != -1 && ansen.first > max) {
                        max = ansen.first; out = block;
                    }
                    blocks.pop_back();
                }
            }
        }
        return out;
    }

    std::pair<int, int> MyPlayer::Search(Location cur, int desy,Location en)
    {
        int direc = 0;
        Astar astar;
        int* BsetDir = 0;
        int step = 0;
        if (astar.AstarWork(&blocks, cur, desy, en)) {
            BsetDir = astar.GetBestRoad();
            step = 0;
            for (step; *(BsetDir + step) != -1; step++) //获取路径长度
            {
            }
            step -= 1;//多读了结束标志-1
        }
        else step = -1;
        astar.ReleaseRoad();
        if (step >= 0) {
            direc = *(BsetDir + step);
            return std::pair<int, int>(step, direc);
        }
        else return std::pair<int, int>(-1,0);
    }

    int MyPlayer::alphaBeta(int depth, int playerId, int alpha, int beta)
    {
        int score = evaluate();
        if (depth == 0 || score == 0 || score == 200) {
            //if (playerId == 1) score = 100 - (score - 100);
            return score;
        }
        if (playerId == 1) {
            //对于所有可能放木板的情况
            int maxEval = -100000;
            std::pair<int, int> ansme, ansen;
            ansme = Search(me, targetY, en);
            ansen = Search(en, 10 - targetY, me);
            melength = ansme.first;
            enlength = ansen.first;
            if (myblocks.size() < 10) {
                std::deque<BlockBar> possibleBlock = possibleblock(me, en, targetY, ansen.first);
                for (int i = 0; i < possibleBlock.size(); i++) {
                    blocks.push_back(possibleBlock[i]);
                    myblocks.push_back(possibleBlock[i]);
                    int eval = alphaBeta(depth - 1, 1-playerId, alpha, beta);
                    maxEval = max(maxEval, eval);
                    alpha = max(alpha, eval);
                    //std::cout << depth << possibleBlock[i] << " " << eval << std::endl;
                    blocks.pop_back();
                    myblocks.pop_back();
                    if (alpha >= beta) return maxEval;
                }
            }
            //对于可能移动的情况
            int direc = Search(me, targetY, en).second;
            Location t = me;
            Move(me, direc);
            if (en.distance(me) == 0) {
                if (illegalMove(me, direc)) {
                    int tdirec = direc;
                    if (direc >= 0 && direc <= 1) direc = 1 - direc; else direc = 5 - direc;
                    Move(me, direc);
                    for (int i = 3; i >= 0; i--) if (!illegalMove(me, i) && i!= tdirec) {
                        Move(me, i); break;
                    }
                }
                else {
                    Move(me, direc);
                }
            }
            int eval = alphaBeta(depth - 1, 1-playerId, alpha, beta);
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            //std::cout << depth << direc << " " << eval << std::endl;
            if (direc >= 0 && direc <= 1) direc = 1 - direc; else direc = 5 - direc;
            me = t;
            return maxEval;
        }
        else {
            int minEval = 100000;
            std::pair<int, int> ansme, ansen;
            ansme = Search(me, targetY, en);
            ansen = Search(en, 10 - targetY, me);
            melength = ansme.first;
            enlength = ansen.first;
            if (enblocks.size() < 10) {
                std::deque<BlockBar> possibleBlock = possibleblock(en, me, 10 - targetY,ansme.first);
                for (int i = 0; i < possibleBlock.size(); i++) {
                    blocks.push_back(possibleBlock[i]);
                    enblocks.push_back(possibleBlock[i]);
                    int eval = alphaBeta(depth - 1, !playerId, alpha, beta);
                    minEval = min(minEval, eval);
                    beta = min(beta, eval);
                    //std::cout << depth << possibleBlock[i] << " " << eval << std::endl;
                    blocks.pop_back();
                    enblocks.pop_back();
                    if (alpha >= beta) return minEval;
                }
            }
            //对于可能移动的情况
            int direc = Search(en, 10-targetY, me).second;
            Location t = en;
            Move(en, direc);
            if (en.distance(me) == 0) {
                if (illegalMove(en,direc)) {
                    int tdirec = direc;
                    if (direc >= 0 && direc <= 1) direc = 1 - direc; else direc = 5 - direc;
                    Move(en, direc);
                    for (int i = 3; i >= 0; i--) if (!illegalMove(en, i) && i != tdirec) {
                        Move(en, i); break;
                    }
                }
                else {
                    Move(en, direc);
                }
            }
            int eval = alphaBeta(depth - 1, !playerId, alpha, beta);
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            //std::cout << depth << direc << " " << eval << std::endl;
            en = t;
            return minEval;
        }
    }

    int MyPlayer::evaluate()
    {
        int enShortLength = Search(en, 10 - targetY, me).first;
        int meShortLength = Search(me, targetY, en).first;
        int enWallNum = enblocks.size();
        int meWallNum = myblocks.size();
        if (enShortLength == 0) return -10000;
        if (meShortLength == 0) return 10000;
        int score = 100 + (enShortLength - meShortLength) * (enShortLength - meShortLength) * (enShortLength - meShortLength) + (enShortLength - enlength) * 2 + (melength - meShortLength) * 2 + enWallNum - meWallNum;
        return score;
    }

    std::deque<BlockBar> MyPlayer::possibleblock(Location mylocation,Location enlocation,int targety,int originenl)
    {
        std::vector<Location> path;
        int direc = 0;
        Astar astar;
        int* BsetDir = 0;
        int step = 0;
        if (astar.AstarWork(&blocks, enlocation, 10 - targety, mylocation)) {
            BsetDir = astar.GetBestRoad();
            step = 0;
            for (step; *(BsetDir + step) != -1; step++) //获取路径长度
            {
            }
            step -= 1;//多读了结束标志-1
        }
        else step = -1;
        astar.ReleaseRoad();
        Location first = enlocation;
        path.push_back(first);
        for (int k = step; k > 0; k--) {
            switch (*(BsetDir + k))
            {
            case 0: first.y += 1; break;
            case 1: first.y -= 1; break;
            case 2: first.x -= 1; break;
            case 3: first.x += 1; break;
            }
            path.push_back(first);
        }

        int max = 0;
        BlockBar out;
        std::pair<int, int> ansme, ansen;
        std::vector<std::vector<int>> vis;
        for (int i = 0; i <= 7; i++) {
            std::vector<int> t;
            for (int j = 0; j <= 7; j++) {
                t.push_back(-1);
            }
            vis.push_back(t);
        }
        std::deque<std::pair<BlockBar, int>> ans;
        for (int i = 0; i < path.size(); i++) {
            for (int j1 = -2; j1 <= 2; j1++) {
                for (int j2 = -2; j2 <= 2; j2++) {
                    int a = path[i].x + j1; int b = path[i].y + j2;
                    if (!(a >= 0 && b >= 0 && a <= 7 && b <= 7)) continue;
                    if (vis[a][b] == -1) {
                        vis[a][b] = 0;
                    }
                    else continue;
                    for (int k = 0; k <= 1; k++) {
                        if (a == 0 && k == 0 || b == 0 && k == 1) continue;
                        Location start(a, b), end(a + (k * 2), b + 2 * (1 - k));
                        BlockBar block(start, end);
                        if (IfRulebreak(block)) continue;
                        blocks.push_back(block);
                        ansen = Search(enlocation, 10 - targety, mylocation);
                        ansme = Search(mylocation, targety, enlocation);
                        if (ansme.first != -1 && ansen.first != -1 && ansen.first >= max && ansen.first>originenl) {
                            max = ansen.first; ans.push_back(std::pair<BlockBar, int>(block, max));
                        }
                        blocks.pop_back();
                    }
                }
            }
        }
        std::deque<BlockBar> answer;
        for (int i = 0; i < ans.size(); i++) {
            if (ans[i].second == max) answer.push_back(ans[i].first);
        }
        vis.clear();
        path.clear();
        return answer;
    }

    bool MyPlayer::illegalMove(Location cur, int dir)
    {
        Location t = cur;
        Move(t, dir);
        if (FindEdge(t) || FindSteel(t, cur)) return true;
        return false;
    }

    Step MyPlayer::nextStep(const ChessboardChange& newChange)
    {
        clock_t a, b;
        a = clock();

        //if (newChange.myLoc == Location(4, 3)) return Step();
        if (targetY == 0) {
            if (newChange.myLoc == PLAYER0_LOC) {                // 己方初始坐标为 (5,1) 则目标为 (x,9) 
                this->targetY = PLAYER1_LOC.y;
                if (newChange.enemyLoc == PLAYER1_LOC) first = true; else first = false;
            }
            else {                                             // 己方初始坐标为 (5,9) 则目标为 (x,1) 
                this->targetY = PLAYER0_LOC.y;
                if (newChange.enemyLoc == PLAYER0_LOC) first = true; else first = false;
            }
        }
        std::cout << newChange;
        if (!newChange.newEnemyBlockBar.isNan()) {               // 对方放置了挡板 
            BlockBar tmp = newChange.newEnemyBlockBar;
            tmp.normalization();                                 // 规范为 start 坐标小于 stop 坐标的形式 
            this->blocks.push_back(tmp);                         // 存储规范挡板 
            this->enblocks.push_back(tmp);
        }
        this->en = newChange.enemyLoc;
        this->me = newChange.myLoc;
        int alpha = -100000, beta = 100000, maxSum = -100000;
        BlockBar ansBlock;
        std::pair<int, int> ansme, ansen;
        ansme = Search(newChange.myLoc, targetY, newChange.enemyLoc);
        int direc = ansme.second;
        ansen = Search(newChange.enemyLoc, 10 - targetY, newChange.myLoc);
        melength = ansme.first;
        enlength = ansen.first;
        if (myblocks.size() < 10 && ansme.first > ansen.first) {
            std::deque<BlockBar> possibleBlock = possibleblock(me, en, targetY, ansen.first);
            for (int i = 0; i < possibleBlock.size(); i++) {
                
                blocks.push_back(possibleBlock[i]);
                myblocks.push_back(possibleBlock[i]);
                if (possibleBlock[i] == BlockBar(1, 7, 1, 9)) continue;
                std::cout << 3 << possibleBlock[i];
                int eval = alphaBeta(2, 0, alpha, beta);
                std::cout <<" " <<eval << std::endl;
                if (eval > maxSum) {
                    maxSum = eval;
                    ansBlock = possibleBlock[i];
                }
                blocks.pop_back();
                myblocks.pop_back();
            }
        }
        //对于可能移动的情况
        direc = Search(me, targetY, en).second;
        if (!illegalMove(me, direc)) {
            Move(me, direc);
            if (en.distance(me) == 0) {
                if (illegalMove(en, direc)) {
                    int tdirec = direc;
                    if (direc >= 0 && direc <= 1) direc = 1 - direc; else direc = 5 - direc;
                    Move(me, direc);
                    for (int i = 3; i >= 0; i--) if (!illegalMove(me, i) && i!=tdirec) {
                        Move(me, i); break;
                    }
                    std::cout << " -> " << Step(me) << std::endl;
                    b = clock();
                    double d = (double)(b - a) / CLOCKS_PER_SEC;
                    std::cout << d << "s" << std::endl;

                    return Step(me);
                }
                else {
                    Move(me, direc);
                    std::cout << " -> " << Step(me) << std::endl;
                    b = clock();
                    double d = (double)(b - a) / CLOCKS_PER_SEC;
                    std::cout << d << "s" << std::endl;

                    return Step(me);
                }
            }
            else {
                int eval = alphaBeta(2, 0, alpha, beta);
                if (eval >= maxSum) {
                    std::cout << " -> " << Step(me) << std::endl;
                    b = clock();
                    double d = (double)(b - a) / CLOCKS_PER_SEC;
                    std::cout << d << "s" << std::endl;

                    return Step(me);
                }
                else {
                    blocks.push_back(ansBlock);
                    myblocks.push_back(ansBlock);
                    std::cout << " -> " << Step(Location(), ansBlock) << std::endl;
                    b = clock();
                    double d = (double)(b - a) / CLOCKS_PER_SEC;
                    std::cout << d << "s" << std::endl;

                    return Step(Location(), ansBlock);
                }
            }
        }
        else {
            blocks.push_back(ansBlock);
            myblocks.push_back(ansBlock);
            std::cout << " -> " << Step(Location(), ansBlock) << std::endl;
            b = clock();
            double d = (double)(b - a) / CLOCKS_PER_SEC;
            std::cout << d << "s" << std::endl;

            return Step(Location(), ansBlock);
        }
    }

    void MyPlayer::restart()
    {
        this->blocks.clear();
        this->myblocks.clear();
        this->enblocks.clear();
        this->targetY = 0;
        this->first = false;
        this->res = 10;
    }

}