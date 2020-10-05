#include "AStar.h"

namespace QuoridorUtils {
	Astar::Astar() :Start{ 0 }, Des{ 0 }, Cur{ 0 }, Next{ 0 }, A(1)
	{

	}

	Astar::~Astar()
	{
		v_open.clear();
		v_close.clear();
	}
	//设置算法模式
	void Astar::SetMode(bool a)
	{
		A = a;//控制两种算法的开关
	}
	//获取起始和目标位置
	bool  Astar::GetStartAndDesPoint(void* map, NODE* start, NODE* des)
	{
		start->px = start->x = this->begin.x;
		start->py = start->y = this->begin.y;
		des->y = target;
		return 1;
	}

	//判断边界
	bool  Astar::FindEdge(void* map, NODE* cur)
	{
		if (cur->x<1 || cur->y>SIZE || cur->x > SIZE || cur->y < 1) return true; else return false;
	}
	//判断障碍
	bool  Astar::FindSteel(void* map, NODE* cur, NODE* pcur)
	{
		std::deque<BlockBar>* v_map = (std::deque<BlockBar>*)map;
		for (auto block : *v_map) {                        // 遍历挡板列表, 找到被阻挡的方向 
			if (block.isH()) {                                   // 水平方向挡板, start 与 stop 的 y 相等 
				if (block.start.x == pcur->x - 1 ||
					block.start.x == pcur->x - 2) {              // 可能挡路的挡板 
					if (block.start.y == pcur->y) {
						if (cur->x == pcur->x && cur->y == pcur->y + 1) return true;
					}
					else if (block.start.y == pcur->y - 1) {
						if (cur->x == pcur->x && cur->y == pcur->y - 1) return true;
					}
				}
			}
			if (block.isV()) {                                   // 竖直方向挡板, start 与 stop 的 x 相等 
				if (block.start.y == pcur->y - 1 ||
					block.start.y == pcur->y - 2) {              // 可能挡路的挡板 
					if (block.start.x == pcur->x) {
						if (cur->x == pcur->x + 1 && cur->y == pcur->y) return true;
					}
					else if (block.start.x == pcur->x - 1) {
						if (cur->x == pcur->x - 1 && cur->y == pcur->y) return true;
					}
				}
			}
		}
		return false;
	}
	bool Astar::FindEn(NODE* cur)
	{
		if (cur->x == targetx.x && cur->y == targetx.y) return true;
		return false;
	}
	//判断终点
	bool  Astar::FindDes(NODE* cur)
	{
		if (cur->y == Des.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	/*判断open表
	//1.要搜索的点
	//2.返回找到的这个节点
	*/
	bool  Astar::FindOpen(NODE* cur, NODE** old)
	{
		for (int i = 0; i < v_open.size(); i++)
		{
			if (v_open[i].x == cur->x && v_open[i].y == cur->y)
			{
				*old = &(v_open[i]);//返回当前
				return true;
			}
		}
		return false;
	}
	//判断close表
	bool  Astar::FindClose(NODE* cur)
	{
		for (int i = 0; i < v_close.size(); i++)
		{
			if (v_close[i].x == cur->x && v_close[i].y == cur->y)
			{
				return true;
			}
		}
		return false;
	}

	//计算G值，G=从起点 A 移动到指定方格的移动代价，沿着到达该方格而生成的路径
	//参数1 要去的节点
	//参数2  目前在的节点
	int Astar::CalcG(NODE* cur, NODE* pcur)
	{
		//当前点移动到该点的代价 定义为10
		if (cur->x == pcur->x || cur->y == pcur->y)
			return 10;
		return 20;
	}
	//估值H  
	//普通曼哈顿法   取(当前点x-目标x 的绝对值) +  (当前点y-目标点y的绝对值)  两者相加乘10
	int Astar::CalcH(NODE* cur)
	{
		return (abs(cur->y - Des.y)) * 10;
	}
	/* 参数1 地图信息
		参数2  要搜索的节点
		参数3  目前在的节点
	*/
	bool Astar::FindPoint(void* map, NODE* cur, NODE* pcur)
	{
		NODE* old = nullptr;
		if (FindSteel(map, cur, pcur)) return 0;
		if (!FindDes(cur))//不是终点
		{
			if (!FindClose(cur))//不在close表
			{
				if (!FindOpen(cur, &old))//不在Open表中
				{
					if (!FindEdge(map, cur) && !FindSteel(map, cur,pcur))//不是边界障碍
					{
						
						//赋值父坐标
						cur->px = pcur->x;
						cur->py = pcur->y;
						//G值计算 
						if (A)
							cur->g = CalcG(cur, pcur) + pcur->g;  //G值会一直增加 是A*算法
						else
							cur->g = CalcG(cur, pcur);  //此G值不会增加 局部计算  相当于A算法 非A*  				
						//H值计算
						cur->h = CalcH(cur);
						//F值计算
						cur->f = cur->g + cur->h;
						//加此节点到open表
						v_open.insert(v_open.begin(), *cur);
					}
				}
				else//在Open表
				{
					//在open表中找到的这个点 与当前点计算G值
					if (old)
					{
						if (CalcG(cur, pcur) + pcur->g < old->g)//当前点移动到搜索点的G+上当前点的G值  是不是比原搜索点G值小   如果大就不动
						{//更新父坐标
							old->px = pcur->x;//就把当前这个点作为他的父坐标
							old->py = pcur->y;
							//更新G值为目前新值
							if (A)
								old->g = CalcG(cur, pcur) + pcur->g;//此为A*算法
							else
								old->g = CalcG(cur, pcur); //此方法为非A*算法						
							old->f = old->g + old->h;//更新F值
							//更新方向值
							old->dir = cur->dir;
						}
					}
				}
			}
		}
		else 
		{//是终点 直接加入Close表并退出
		 //指定终点的父坐标为此坐标
			cur->px = pcur->x;
			cur->py = pcur->y;
			v_close.push_back(*cur);
			return 1;
		}
		return 0;
	}
	bool Astar::AstarWork(void* map,Location s,int targetY,Location x)
	{
		target = targetY;
		targetx = x;
		this->begin = s;
		NODE* CUR = new NODE();
		GetStartAndDesPoint(map, &Start, &Des);
		Cur = Start;
		memcpy_s(CUR, sizeof(NODE), &Cur, sizeof(NODE));
		//把起始点放入表close表
		v_close.push_back(Start);
		NODE* C1 = new NODE(); C1->x = 3; C1->y = 6;
		NODE* C2 = new NODE(); C2->x = 3; C2->y = 7;
		while (1)
		{
			memcpy_s(CUR, sizeof(NODE), &Cur, sizeof(NODE));
			//根据当前点 遍历周边4点  //不考虑斜向
			CUR->y += 1;  //上
			CUR->dir = 0;
			if (FindPoint(map, CUR, &Cur)) //从当前节点Cur 搜索CUR
			{
				break;//是终点直接退出循环
			}
			memcpy_s(CUR, sizeof(NODE), &Cur, sizeof(NODE));//还原
			CUR->y -= 1;//下
			CUR->dir = 1;
			if (FindPoint(map, CUR, &Cur))
			{
				break;
			}
			memcpy_s(CUR, sizeof(NODE), &Cur, sizeof(NODE));
			CUR->x += 1;
			CUR->dir = 3;
			if (FindPoint(map, CUR, &Cur))
			{
				break;
			}
			memcpy_s(CUR, sizeof(NODE), &Cur, sizeof(NODE));
			CUR->x -= 1;
			CUR->dir = 2;
			if (FindPoint(map, CUR, &Cur))
			{
				break;
			}
			//排序open表
			if (v_open.size() > 0)
			{
				NODE   temp = { 0 };
				//遍历每个把最小的放最前面冒泡排序
				for (unsigned int i = v_open.size(); i > 0; i--)
				{//比较f值
					for (int j = v_open.size() - 1; j > v_open.size() - i; j--)
					{
						if (v_open[j].f < v_open[j - 1].f)
						{
							//交换小的
							temp = v_open[j - 1];
							v_open[j - 1] = v_open[j];
							v_open[j] = temp;
						}
					}
				}
			}
			else//open表已空无解了
			{
				delete[] CUR;
				CUR = nullptr;
				return 0;
			}
			//重新赋值当前点
			//把f值最小的加入close表
			//同时删除在open表中删除
			Cur = *v_open.begin();
			memcpy_s(CUR, sizeof(NODE), &Cur, sizeof(NODE));//以这个点作为当前点 开始下一次循环搜索
			v_close.push_back(*v_open.begin());
			v_open.erase(v_open.begin());//最小值在第一个直接删除
		}
		//整理出close表
		NODE* Step = new NODE[v_close.size()];//这组存放回溯节点信息
		int* r_dir = new int[v_close.size()];//这组放回溯的反向信息
		//根据坐标返回在close表中索引
		int p_x = CUR->x;//从终点开始网上溯源
		int p_y = CUR->y;
		delete[] CUR;
		CUR = nullptr;
		for (int i = 0; ; i++)
		{
			//把返回的数据取出父坐标继续溯源
			*(Step + i) = GetCloseData(p_x, p_y);
			if ((*(Step + i)).x == Start.x && (*(Step + i)).y == Start.y)//如果是起点就结束
			{
				//插入一个结束标记-1
				*(r_dir + i) = -1;
				break;
			}
			*(r_dir + i) = (*(Step + i)).dir;//取出方向另外存储
			p_x = (*(Step + i)).px;//用父坐标继续溯源
			p_y = (*(Step + i)).py;
		}
		
		BestRoad = Step;
		BestDir = r_dir;
		delete[] Step;
		delete[] r_dir;
		return 1;
	}
	//根据坐标返回节点信息
	NODE Astar::GetCloseData(int x, int y)
	{
		for (unsigned int i = 0; i < v_close.size(); i++)
		{//比较f值
			if (v_close[i].x == x && v_close[i].y == y)
			{
				return v_close[i];
			}
		}
		return { 0 };
	}

	int* Astar::GetBestRoad()
	{
		if (BestDir)
		{
			return BestDir;
		}
		else
			return 0;
	}
	//给外部类使用  用来释放空间
	bool Astar::ReleaseRoad()
	{
		v_open.clear();
		v_close.clear();
		if (!BestRoad)
		{
			delete[] BestRoad;
			BestRoad = nullptr;
		}
		if (!BestDir)
		{
			delete[] BestDir;
			BestDir = nullptr;
		}
		return true;
	}
}