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
	//�����㷨ģʽ
	void Astar::SetMode(bool a)
	{
		A = a;//���������㷨�Ŀ���
	}
	//��ȡ��ʼ��Ŀ��λ��
	bool  Astar::GetStartAndDesPoint(void* map, NODE* start, NODE* des)
	{
		start->px = start->x = this->begin.x;
		start->py = start->y = this->begin.y;
		des->y = target;
		return 1;
	}

	//�жϱ߽�
	bool  Astar::FindEdge(void* map, NODE* cur)
	{
		if (cur->x<1 || cur->y>SIZE || cur->x > SIZE || cur->y < 1) return true; else return false;
	}
	//�ж��ϰ�
	bool  Astar::FindSteel(void* map, NODE* cur, NODE* pcur)
	{
		std::deque<BlockBar>* v_map = (std::deque<BlockBar>*)map;
		for (auto block : *v_map) {                        // ���������б�, �ҵ����赲�ķ��� 
			if (block.isH()) {                                   // ˮƽ���򵲰�, start �� stop �� y ��� 
				if (block.start.x == pcur->x - 1 ||
					block.start.x == pcur->x - 2) {              // ���ܵ�·�ĵ��� 
					if (block.start.y == pcur->y) {
						if (cur->x == pcur->x && cur->y == pcur->y + 1) return true;
					}
					else if (block.start.y == pcur->y - 1) {
						if (cur->x == pcur->x && cur->y == pcur->y - 1) return true;
					}
				}
			}
			if (block.isV()) {                                   // ��ֱ���򵲰�, start �� stop �� x ��� 
				if (block.start.y == pcur->y - 1 ||
					block.start.y == pcur->y - 2) {              // ���ܵ�·�ĵ��� 
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
	//�ж��յ�
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
	/*�ж�open��
	//1.Ҫ�����ĵ�
	//2.�����ҵ�������ڵ�
	*/
	bool  Astar::FindOpen(NODE* cur, NODE** old)
	{
		for (int i = 0; i < v_open.size(); i++)
		{
			if (v_open[i].x == cur->x && v_open[i].y == cur->y)
			{
				*old = &(v_open[i]);//���ص�ǰ
				return true;
			}
		}
		return false;
	}
	//�ж�close��
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

	//����Gֵ��G=����� A �ƶ���ָ��������ƶ����ۣ����ŵ���÷�������ɵ�·��
	//����1 Ҫȥ�Ľڵ�
	//����2  Ŀǰ�ڵĽڵ�
	int Astar::CalcG(NODE* cur, NODE* pcur)
	{
		//��ǰ���ƶ����õ�Ĵ��� ����Ϊ10
		if (cur->x == pcur->x || cur->y == pcur->y)
			return 10;
		return 20;
	}
	//��ֵH  
	//��ͨ�����ٷ�   ȡ(��ǰ��x-Ŀ��x �ľ���ֵ) +  (��ǰ��y-Ŀ���y�ľ���ֵ)  ������ӳ�10
	int Astar::CalcH(NODE* cur)
	{
		return (abs(cur->y - Des.y)) * 10;
	}
	/* ����1 ��ͼ��Ϣ
		����2  Ҫ�����Ľڵ�
		����3  Ŀǰ�ڵĽڵ�
	*/
	bool Astar::FindPoint(void* map, NODE* cur, NODE* pcur)
	{
		NODE* old = nullptr;
		if (FindSteel(map, cur, pcur)) return 0;
		if (!FindDes(cur))//�����յ�
		{
			if (!FindClose(cur))//����close��
			{
				if (!FindOpen(cur, &old))//����Open����
				{
					if (!FindEdge(map, cur) && !FindSteel(map, cur,pcur))//���Ǳ߽��ϰ�
					{
						
						//��ֵ������
						cur->px = pcur->x;
						cur->py = pcur->y;
						//Gֵ���� 
						if (A)
							cur->g = CalcG(cur, pcur) + pcur->g;  //Gֵ��һֱ���� ��A*�㷨
						else
							cur->g = CalcG(cur, pcur);  //��Gֵ�������� �ֲ�����  �൱��A�㷨 ��A*  				
						//Hֵ����
						cur->h = CalcH(cur);
						//Fֵ����
						cur->f = cur->g + cur->h;
						//�Ӵ˽ڵ㵽open��
						v_open.insert(v_open.begin(), *cur);
					}
				}
				else//��Open��
				{
					//��open�����ҵ�������� �뵱ǰ�����Gֵ
					if (old)
					{
						if (CalcG(cur, pcur) + pcur->g < old->g)//��ǰ���ƶ����������G+�ϵ�ǰ���Gֵ  �ǲ��Ǳ�ԭ������GֵС   �����Ͳ���
						{//���¸�����
							old->px = pcur->x;//�Ͱѵ�ǰ�������Ϊ���ĸ�����
							old->py = pcur->y;
							//����GֵΪĿǰ��ֵ
							if (A)
								old->g = CalcG(cur, pcur) + pcur->g;//��ΪA*�㷨
							else
								old->g = CalcG(cur, pcur); //�˷���Ϊ��A*�㷨						
							old->f = old->g + old->h;//����Fֵ
							//���·���ֵ
							old->dir = cur->dir;
						}
					}
				}
			}
		}
		else 
		{//���յ� ֱ�Ӽ���Close���˳�
		 //ָ���յ�ĸ�����Ϊ������
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
		//����ʼ������close��
		v_close.push_back(Start);
		NODE* C1 = new NODE(); C1->x = 3; C1->y = 6;
		NODE* C2 = new NODE(); C2->x = 3; C2->y = 7;
		while (1)
		{
			memcpy_s(CUR, sizeof(NODE), &Cur, sizeof(NODE));
			//���ݵ�ǰ�� �����ܱ�4��  //������б��
			CUR->y += 1;  //��
			CUR->dir = 0;
			if (FindPoint(map, CUR, &Cur)) //�ӵ�ǰ�ڵ�Cur ����CUR
			{
				break;//���յ�ֱ���˳�ѭ��
			}
			memcpy_s(CUR, sizeof(NODE), &Cur, sizeof(NODE));//��ԭ
			CUR->y -= 1;//��
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
			//����open��
			if (v_open.size() > 0)
			{
				NODE   temp = { 0 };
				//����ÿ������С�ķ���ǰ��ð������
				for (unsigned int i = v_open.size(); i > 0; i--)
				{//�Ƚ�fֵ
					for (int j = v_open.size() - 1; j > v_open.size() - i; j--)
					{
						if (v_open[j].f < v_open[j - 1].f)
						{
							//����С��
							temp = v_open[j - 1];
							v_open[j - 1] = v_open[j];
							v_open[j] = temp;
						}
					}
				}
			}
			else//open���ѿ��޽���
			{
				delete[] CUR;
				CUR = nullptr;
				return 0;
			}
			//���¸�ֵ��ǰ��
			//��fֵ��С�ļ���close��
			//ͬʱɾ����open����ɾ��
			Cur = *v_open.begin();
			memcpy_s(CUR, sizeof(NODE), &Cur, sizeof(NODE));//���������Ϊ��ǰ�� ��ʼ��һ��ѭ������
			v_close.push_back(*v_open.begin());
			v_open.erase(v_open.begin());//��Сֵ�ڵ�һ��ֱ��ɾ��
		}
		//�����close��
		NODE* Step = new NODE[v_close.size()];//�����Ż��ݽڵ���Ϣ
		int* r_dir = new int[v_close.size()];//����Ż��ݵķ�����Ϣ
		//�������귵����close��������
		int p_x = CUR->x;//���յ㿪ʼ������Դ
		int p_y = CUR->y;
		delete[] CUR;
		CUR = nullptr;
		for (int i = 0; ; i++)
		{
			//�ѷ��ص�����ȡ�������������Դ
			*(Step + i) = GetCloseData(p_x, p_y);
			if ((*(Step + i)).x == Start.x && (*(Step + i)).y == Start.y)//��������ͽ���
			{
				//����һ���������-1
				*(r_dir + i) = -1;
				break;
			}
			*(r_dir + i) = (*(Step + i)).dir;//ȡ����������洢
			p_x = (*(Step + i)).px;//�ø����������Դ
			p_y = (*(Step + i)).py;
		}
		
		BestRoad = Step;
		BestDir = r_dir;
		delete[] Step;
		delete[] r_dir;
		return 1;
	}
	//�������귵�ؽڵ���Ϣ
	NODE Astar::GetCloseData(int x, int y)
	{
		for (unsigned int i = 0; i < v_close.size(); i++)
		{//�Ƚ�fֵ
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
	//���ⲿ��ʹ��  �����ͷſռ�
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