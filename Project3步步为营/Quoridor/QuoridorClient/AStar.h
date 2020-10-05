#pragma once
#include<deque>
#include<iostream>
#include "../QuoridorUtils/QuoridorUtils.h"
namespace QuoridorUtils {
	typedef struct _Node
	{
		int x;//节点坐标
		int y;
		int px; //父坐标
		int py;
		int dir;//方向
		int f;  //F值
		int g;//G值
		int h;//H值
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
		NODE* BestRoad;//存放最佳路径节点起始地址
		int* BestDir;//存放最佳路径节点方向起始地址
		bool GetStartAndDesPoint(void* map, NODE* start, NODE* des); //获得起点/终点
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
