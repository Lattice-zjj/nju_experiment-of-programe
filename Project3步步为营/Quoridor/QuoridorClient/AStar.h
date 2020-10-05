#pragma once
#include<deque>
#include<iostream>
#include "../QuoridorUtils/QuoridorUtils.h"
namespace QuoridorUtils {
	typedef struct _Node
	{
		int x;//�ڵ�����
		int y;
		int px; //������
		int py;
		int dir;//����
		int f;  //Fֵ
		int g;//Gֵ
		int h;//Hֵ
	}NODE, * PNODE;
	class Astar
	{
	private:
		Location begin;
		int target;
		Location targetx;
		NODE  Start;
		NODE  Des;
		NODE  Cur;
		NODE  Next;
		std::deque<NODE> v_open;
		std::deque<NODE> v_close;
		bool A;
		NODE* BestRoad;//������·���ڵ���ʼ��ַ
		int* BestDir;//������·���ڵ㷽����ʼ��ַ
		bool GetStartAndDesPoint(void* map, NODE* start, NODE* des); //������/�յ�
		NODE GetCloseData(int x, int y);
		bool FindEdge(void* map, NODE* cur);
		bool FindSteel(void* map, NODE* cur, NODE* pcur);
		bool FindEn(NODE* cur);
		bool FindDes(NODE* cur);
		bool FindOpen(NODE* cur, NODE** old);
		bool FindClose(NODE* cur);
		int CalcG(NODE* cur, NODE* pcur);
		int CalcH(NODE* cur);
		bool FindPoint(void* map, NODE* cur, NODE* pcur);
	public:
		Astar();
		~Astar();
		void SetMode(bool a);
		bool AstarWork(void* map, Location s, int targetY, Location x);
		int* GetBestRoad();
		bool ReleaseRoad();
	};
}
