#pragma once
#include "Player.h"
#include <deque>
#include "AStar.h"

namespace QuoridorUtils {
    class MyPlayer final : public Player {
    private:
        std::deque<BlockBar> blocks; //��ŵ�ǰ���еİ���
        std::deque<BlockBar> myblocks;
        std::deque<BlockBar> enblocks;
        Location me;
        Location en;
        int melength;
        int enlength;
        int targetY = 0; //Ŀ���y
        bool first; //�ж��Ƿ�������
        void Move(Location& now, int dir); //�ƶ�����
        bool Jump(Location& now, int dir); //��Ծ���з�����
        bool FindSteel(Location& cur, Location& pcur);//�жϴ�pcur��cur�Ƿ��ܹ���ȷ�ﵽ
        bool FindEdge(Location& cur);//�ж�cur�Ƿ��ڱ߽�
        bool IfRulebreak(BlockBar block);//�ж�block�������Ƿ��Ƿ����
        BlockBar AddBlockBar(Location enemyLoc, Location myLoc); //ͨ���Ե��¾��Ƶķ����ھ���λ�ü���ľ��
        std::pair<int, int> Search(Location cur,int desy, Location en);//��cur��desy��·����������en��ʾ�з�λ�ã�pair��first�����ǳ��ȣ�second������Ƿ���
        int alphaBeta(int depth, int playerId, int alpha, int beta);//�������ļ�֦
        int res = 10;
        int evaluate();//��������
        std::deque<BlockBar> possibleblock(Location mylocation, Location enlocation, int targety,int originenl);//����ľ��ļ��ϣ����ڲ�����
        int max(int a, int b) { return a > b ? a : b; }//ȡ���ֵ
        int min(int a, int b) { return a < b ? a : b; }//ȡ��Сֵ
        bool illegalMove(Location cur, int dir);//�ж�cur��dir�������Ƿ�Ƿ���· 0��1��2��3�ĺ����A*�ļ�
    public:
        MyPlayer(const std::string& key) : Player(key) {};            // �������, �����޸� 
        Step nextStep(const ChessboardChange& newChange) override;    // ��������ʵ�� 
        void restart() override;                                      // ��������ʵ�� 
    };
}

/*
 *
 * ������������:                 ��������ṹ���£�
 * ��-��-��-��-��-��-��-��-��-��          ��-��-��-��-��-��-��-��-��-��9
 * ��-��-��-��-��-��-��-��-��-�ȨI9       ��-��-��-��-��-��-��-��-��-��8
 * ��-��-��-��-��-��-��-��-��-�ȨI8       ��-��-��-��-��-��-��-��-��-��7
 * ��-��-��-��-��-��-��-��-��-�ȨI7       ��-��-��-��-��-��-��-��-��-��6
 * ��-��-��-��-��-��-��-��-��-�ȨI6       ��-��-��-��-��-��-��-��-��-��5
 * ��-��-��-��-��-��-��-��-��-�ȨI5       ��-��-��-��-��-��-��-��-��-��4
 * ��-��-��-��-��-��-��-��-��-�ȨI4       ��-��-��-��-��-��-��-��-��-��3
 * ��-��-��-��-��-��-��-��-��-�ȨI3       ��-��-��-��-��-��-��-��-��-��2
 * ��-��-��-��-��-��-��-��-��-�ȨI2       ��-��-��-��-��-��-��-��-��-��1
 * ��-��-��-��-��-��-��-��-��-���I1 y     ��-��-��-��-��-��-��-��-��-��0 y
 * x 1  2  3  4  5  6  7  8  9          x  0  1  2  3  4  5  6  7  8  9
 *
 *
 */