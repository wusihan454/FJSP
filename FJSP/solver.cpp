#include"solver.h"
#include<algorithm>

solver::solver(int a, int b, int c, int* d, procedure** e, int** f) :job_count(a), machine_count(b), candidate(c), procedure_count(d), message(e), T(f)
{
	job = new workpiece * [job_count];
	for (int i = 0; i < job_count; i++)
	{
		job[i] = new workpiece[procedure_count[i]];
	}
	machine.resize(machine_count);
	critical_block.resize(machine_count);
	tabu_section.resize(machine_count);
}
solver::~solver()
{
	delete[] procedure_count;
	for (int i = 0; i < job_count; i++)
		delete[] message[i];
	delete[] message;
	for (int i = 0; i < job_count; i++)
		delete[]	T[i];
	delete[] T;
	for (int i = 0; i < job_count; i++)
		delete[] job[i];
	delete[] job;
}
void solver::random_init()//随机产生初始解
{
	for (int i = 0; i < job_count; i++)
	{
		for (int j = 0; j < procedure_count[i]; j++)//i工件的每个工序随机选择一个机器
		{
			int choose = j * candidate + rand() % T[i][j];
			job[i][j].machine = message[i][choose].machine;
			job[i][j].dura_time = message[i][choose].time;
		}
	}
	for (int i = 0; i < machine_count; i++)
		machine[i].clear();
	//随机扰动n个工件的顺序；
	int* Q;
	Q = new int[job_count];
	for (int i = 0; i < job_count; i++)
		Q[i] = i;
	random_shuffle(Q, Q + job_count);
	for (int i = 0; i < job_count; i++) printf("%d ", Q[i]);
	printf("\n");
	for (int i = 0; i < job_count; i++)
	{
		int choose = Q[i];
		for (int j = 0; j < procedure_count[choose]; j++)
		{
			int jp, mp;
			int select_machine = job[choose][j].machine;
			if (j == 0) jp = 0;
			else jp = job[choose][j - 1].start_time+ job[choose][j - 1].dura_time;
			int temp = machine[select_machine].size();
			if (temp == 0) mp = 0;
			else
			{
				mp = machine[select_machine][temp - 1].R + machine[select_machine][temp - 1].dura_time;
			}
			job[choose][j].start_time = jp < mp ? mp : jp;
			machine[select_machine].push_back(operation(choose, j, job[choose][j].start_time, job[choose][j].dura_time));
		}
	}
	delete[] Q;
}
void solver::sumR()
{
	queue<du_0> Q;
	for (int i = 0; i < machine.size(); i++)
	{
		for (int j = 0; j < machine[i].size(); j++)
		{
			machine[i][j].R = -1;
		}
	}
	for (int i = 0; i < machine.size(); i++)
	{
		if (machine[i].empty()) continue;
		int length = machine[i].size();
		machine[i][0].du = machine[i][0].seq > 0 ? 1 : 0;
		if (machine[i][0].du == 0)
		{
			machine[i][0].R = 0;
			Q.push(du_0(i, 0));
		}
		for (int j = 1; j < length; j++)
		{
			machine[i][j].du = machine[i][j].seq > 0 ? 2 : 1;
		}
	}
	while (!Q.empty())
	{
		du_0 temp = Q.front();
		int m = machine[temp.machine][temp.index].R + machine[temp.machine][temp.index].dura_time;
		if (temp.index+1< machine[temp.machine].size())
		{
			machine[temp.machine][temp.index + 1].du--;
			if (machine[temp.machine][temp.index + 1].R < m)
				machine[temp.machine][temp.index + 1].R = m;
			if (machine[temp.machine][temp.index + 1].du == 0) Q.push(du_0(temp.machine, temp.index + 1));
		}
		int i = machine[temp.machine][temp.index].job;
		int j = machine[temp.machine][temp.index].seq;
		if (j+1<procedure_count[i])
		{
			int dex = job[i][j + 1].machine;
			for (int k = 0; k < machine[dex].size(); k++)
			{
				if (machine[dex][k].job == i && machine[dex][k].seq == j + 1)
				{
					machine[dex][k].du--;
					if (machine[dex][k].du == 0) Q.push(du_0(dex, k));
					if (machine[dex][k].R < m) machine[dex][k].R = m;
					break;
				}
			}
		}
		Q.pop();
	}
}
void solver::sumQ()
{
	queue<du_0> Q;
	for (int i = 0; i < machine.size(); i++)
	{
		for (int j = 0; j < machine[i].size(); j++)
		{
			machine[i][j].Q = -1;
		}
	}
	for (int i = 0; i < machine.size(); i++)
	{
		if (machine[i].empty()) continue;
		int length = machine[i].size();
		for (int j = 0; j < length - 1; j++)
		{
			machine[i][j].du = machine[i][j].seq < procedure_count[machine[i][j].job] - 1 ? 2 : 1;

		}
		machine[i][length - 1].du = machine[i][length - 1].seq < procedure_count[machine[i][length - 1].job] - 1 ? 1 : 0;
		if (machine[i][length - 1].du == 0)
		{
			machine[i][length - 1].Q = 0;
			Q.push(du_0(i, length - 1));
		}
	}
	while (!Q.empty())
	{
		du_0 temp = Q.front();
		
		if (temp.index > 0)
		{
			int m1 = machine[temp.machine][temp.index].Q + machine[temp.machine][temp.index].dura_time+ machine[temp.machine][temp.index].R- machine[temp.machine][temp.index-1].R-machine[temp.machine][temp.index - 1].dura_time;
			machine[temp.machine][temp.index - 1].du--;
			if (machine[temp.machine][temp.index - 1].Q < m1)
				machine[temp.machine][temp.index - 1].Q = m1;
			if (machine[temp.machine][temp.index - 1].du == 0) Q.push(du_0(temp.machine, temp.index - 1));
		}
		int i = machine[temp.machine][temp.index].job;
		int j = machine[temp.machine][temp.index].seq;
		if (j > 0)
		{
			int dex = job[i][j - 1].machine;
			for (int k = 0; k < machine[dex].size(); k++)
			{
				if (machine[dex][k].job == i && machine[dex][k].seq == j - 1)
				{
					int m1 = machine[temp.machine][temp.index].Q + machine[temp.machine][temp.index].dura_time + machine[temp.machine][temp.index].R - machine[dex][k].R - machine[dex][k].dura_time;
					machine[dex][k].du--;
					if (machine[dex][k].du == 0) Q.push(du_0(dex, k));
					if (machine[dex][k].Q < m1) machine[dex][k].Q = m1;
					break;
				}
			}
		}
		Q.pop();
	}
}
void solver::sumCmax()
{
	Cmax = 0;
	for (int i = 0; i < machine_count; i++)
	{
		int size = machine[i].size();
		if (size > 0)
		{
			int temp = machine[i][size - 1].R + machine[i][size - 1].Q + machine[i][size - 1].dura_time;
			Cmax = temp > Cmax ? temp : Cmax;
		}
	}
}
int solver::try_to_move_back(int a, int u, int v)//把机器a上的u挪动到v之后,u,v都是对应的位置下标 
{
	vector<int> R_copy;
	vector<int> Q_copy;
	R_copy.resize(v - u + 1);
	Q_copy.resize(v - u + 1);
	for (int i = u; i <= v; i++)
	{
		R_copy[i - u] = machine[a][i].R;
		Q_copy[i - u] = machine[a][i].Q;
	}
	int jp = 0, mp = 0;
	int l1 = u + 1;
	int x = machine[a][l1].job;
	int y = machine[a][l1].seq - 1;
	int m_seq = -1;
	int k = -1;
	if (y >= 0)
	{
		m_seq = job[x][y].machine;
		for (int i = 0; i < machine[m_seq].size(); i++)
		{
			if (machine[m_seq][i].job == x && machine[m_seq][i].seq == y)
			{
				k = i; break;
			}
		}
	}
	if (u > 0)  mp = machine[a][u - 1].R + machine[a][u - 1].dura_time;
	if (m_seq >= 0)
	{
		 jp = machine[m_seq][k].R + machine[m_seq][k].dura_time;
	}
	R_copy[l1 - u] = jp < mp ? mp : jp;

	for (int i = l1 + 1; i <= v; i++)
	{
		jp = 0;
		mp =R_copy[i-1- u] + machine[a][i - 1].dura_time;
		x = machine[a][i].job;
		y = machine[a][i].seq - 1;
		m_seq = -1;
		k = -1;
		if (y >= 0)
		{
			m_seq = job[x][y].machine;
			for (int j= 0; j< machine[m_seq].size(); j++)
			{
				if (machine[m_seq][j].job == x && machine[m_seq][j].seq == y)
				{
					k = j; break;
				}
			}
		}
		if (m_seq >= 0)
		{
			if (m_seq == a && k >= u + 1 && k <i) jp = mp;
			else
			jp = machine[m_seq][k].R + machine[m_seq][k].dura_time;
		}
		R_copy[i - u] = jp < mp ? mp : jp;
	}
	
	x = machine[a][u].job;
	y = machine[a][u].seq - 1;
	jp = 0;
	mp =R_copy[v-u] + machine[a][v].dura_time;
	m_seq = -1;
	k = -1;
	if (y >= 0)
	{
		m_seq = job[x][y].machine;
		for (int i = 0; i < machine[m_seq].size(); i++)
		{
			if (machine[m_seq][i].job == x && machine[m_seq][i].seq == y)
			{
				k = i; break;
			}
		}
	}
	if (m_seq >= 0) jp = machine[m_seq][k].R + machine[m_seq][k].dura_time;
	R_copy[u - u] = jp < mp ? mp : jp;

	//计算Q，从后往前计算
	int js=0, ms=0;
	x = machine[a][u].job;
	y = machine[a][u].seq+1;//后续作业
	m_seq = -1;
	k = -1;
	if (y < procedure_count[x])
	{
		m_seq = job[x][y].machine;
		for (int i = 0; i < machine[m_seq].size(); i++)
		{
			if (machine[m_seq][i].job == x && machine[m_seq][i].seq == y)
			{
				k = i; break;
			}
		}
	}
	if (m_seq >= 0) js = machine[m_seq][k].dura_time + machine[m_seq][k].Q;
	if (v < machine[a].size() - 1)//v不是机器的最后一个节点
	{
		ms = machine[a][v + 1].dura_time + machine[a][v + 1].Q;
	}
	Q_copy[u-u] = js < ms ? ms : js;

	js = 0, ms = 0;
	x = machine[a][v].job;
	y = machine[a][v].seq + 1;//后续作业
	m_seq = -1;
	k = -1;
	if (y < procedure_count[x])
	{
		m_seq = job[x][y].machine;
		for (int i = 0; i < machine[m_seq].size(); i++)
		{
			if (machine[m_seq][i].job == x && machine[m_seq][i].seq == y)
			{
				k = i; break;
			}
		}
	}
	if(m_seq>=0) js = machine[m_seq][k].dura_time + machine[m_seq][k].Q;
	ms= machine[a][u].dura_time + Q_copy[u - u];
	Q_copy[v - u] = js < ms ? ms : js;

	for (int i = v - 1; i >= u + 1; i--)
	{
		js = 0, ms = 0;
		x = machine[a][i].job;
		y = machine[a][i].seq + 1;//后续作业
		m_seq = -1;
		k = -1;
		if (y < procedure_count[x])
		{
			m_seq = job[x][y].machine;
			for (int j= 0; j< machine[m_seq].size(); j++)
			{
				if (machine[m_seq][j].job == x && machine[m_seq][j].seq == y)
				{
					k = j; break;
				}
			}
		}
		ms = machine[a][i + 1].dura_time + Q_copy[i + 1 - u];
		if (m_seq >= 0)
		{
			if (m_seq == a && k > i && k <= v) js = ms;
			else js = machine[m_seq][k].dura_time + machine[m_seq][k].Q;
		}
		Q_copy[i - u] = js < ms ? ms : js;
	}
	int max=0;
	for (int i = u; i <= v; i++)
	{
		int temp = R_copy[i - u] + Q_copy[i - u]+machine[a][i].dura_time;
		if (temp > max) max = temp;
	}
	return max;
}
int solver::try_to_move_front(int a, int u, int v)//v挪动到u的前面去
{
	vector<int> R_copy;
	vector<int> Q_copy;
	R_copy.resize(v - u + 1);
	Q_copy.resize(v - u + 1);
	for (int i = u; i <= v; i++)
	{
		R_copy[i - u] = machine[a][i].R;
		Q_copy[i - u] = machine[a][i].Q;
	}
	//计算r
	int jp = 0, mp = 0;
	int x = machine[a][v].job;
	int y = machine[a][v].seq - 1;
	int m_seq = -1;
	int k = -1;
	if (y >= 0)
	{
		m_seq = job[x][y].machine;
		for (int i = 0; i < machine[m_seq].size(); i++)
		{
			if (machine[m_seq][i].job == x && machine[m_seq][i].seq == y)
			{
				k = i; break;
			}
		}
	}
	if (m_seq >= 0)
	{
		 jp = machine[m_seq][k].R + machine[m_seq][k].dura_time;
	}
	if (u > 0)  mp = machine[a][u - 1].R + machine[a][u - 1].dura_time;
	R_copy[v-u]= jp < mp ? mp : jp;

	jp = 0, mp = 0;
	x = machine[a][u].job;
	y = machine[a][u].seq - 1;
	m_seq = -1;
	k = -1;
	if (y >= 0)
	{
		m_seq = job[x][y].machine;
		for (int i = 0; i < machine[m_seq].size(); i++)
		{
			if (machine[m_seq][i].job == x && machine[m_seq][i].seq == y)
			{
				k = i; break;
			}
		}
	}
	if (m_seq >= 0) jp = machine[m_seq][k].R + machine[m_seq][k].dura_time;
	mp = R_copy[v - u] + machine[a][v].dura_time;
	R_copy[u - u] = jp < mp ? mp : jp;

	for (int i = u + 1; i <= v - 1; i++)
	{
		jp = 0, mp = 0;
		x = machine[a][i].job;
		y = machine[a][i].seq - 1;
		m_seq = -1;
		k = -1;
		if (y >= 0)
		{
			m_seq = job[x][y].machine;
			for (int j = 0; j < machine[m_seq].size(); j++)
			{
				if (machine[m_seq][j].job == x && machine[m_seq][j].seq == y)
				{
					k = j; break;
				}
			}
		}
		mp = R_copy[i-1- u] + machine[a][i-1].dura_time;
		if (m_seq >= 0)
		{
			if (m_seq == a && k >= u && k < i) jp = mp;
			else
			jp = machine[m_seq][k].R + machine[m_seq][k].dura_time;
		}
		R_copy[i- u] = jp < mp ? mp : jp;
	}

	// 计算Q
	int lk = v - 1;
	int js = 0, ms = 0;
	x = machine[a][lk].job;
	y = machine[a][lk].seq + 1;//后续作业
	m_seq = -1;
	k = -1;
	if (y < procedure_count[x])
	{
		m_seq = job[x][y].machine;
		for (int i = 0; i < machine[m_seq].size(); i++)
		{
			if (machine[m_seq][i].job == x && machine[m_seq][i].seq == y)
			{
				k = i; break;
			}
		}
	}
	if (m_seq >= 0) js = machine[m_seq][k].dura_time + machine[m_seq][k].Q;
	if (v < machine[a].size() - 1)//v不是机器的最后一个节点
	{
		ms = machine[a][v + 1].dura_time + machine[a][v + 1].Q;
	}
	Q_copy[lk - u] = js < ms ? ms : js;

	for (int i = lk - 1; i >= u; i--)
	{
		js = 0;
		ms = 0;
		x = machine[a][i].job;
		y = machine[a][i].seq + 1;//后续作业
		m_seq = -1;
		k = -1;
		if (y < procedure_count[x])
		{
			m_seq = job[x][y].machine;
			for (int j= 0; j< machine[m_seq].size(); j++)
			{
				if (machine[m_seq][j].job == x && machine[m_seq][j].seq == y)
				{
					k = j; break;
				}
			}
		}
		ms = machine[a][i+ 1].dura_time + Q_copy[i+1-u];
		if (m_seq >= 0)
		{
			if (m_seq == a && k > i && k <= v-1) js = ms;
			else 
			js = machine[m_seq][k].dura_time + machine[m_seq][k].Q;
		}
		Q_copy[i - u] = js < ms ? ms : js;
	}

	js = 0, ms = 0;
	x = machine[a][v].job;
	y = machine[a][v].seq + 1;//后续作业
	m_seq = -1;
	k = -1;
	if (y < procedure_count[x])
	{
		m_seq = job[x][y].machine;
		for (int i = 0; i < machine[m_seq].size(); i++)
		{
			if (machine[m_seq][i].job == x && machine[m_seq][i].seq == y)
			{
				k = i; break;
			}
		}
	}
	if (m_seq >= 0) js = machine[m_seq][k].dura_time + machine[m_seq][k].Q;
	ms = machine[a][u].dura_time +Q_copy[u-u];
	Q_copy[v- u] = js < ms ? ms : js;
	int max = 0;
	for (int i = u; i <= v; i++)
	{
		int temp = R_copy[i - u] + Q_copy[i - u] + machine[a][i].dura_time;
		if (temp > max) max = temp;
	}
	return max;
}
bool solver::is_move_back_legal(int i, int u, int v)
{
	int x = machine[i][u].job;
	int y = machine[i][u].seq + 1;//后续作业
	int m_seq = -1;
	int m_index = -1;
	if (y < procedure_count[x])
	{
		m_seq = job[x][y].machine;
		for (int t = 0; t < machine[m_seq].size(); t++)
		{
			if (machine[m_seq][t].job == x && machine[m_seq][t].seq == y)
			{
				m_index = t; break;
			}
		}
	}
	if (m_seq != -1)
	{
		for (int t = v; t >= u+1; t--)
		{
			if (machine[i][t].Q <= machine[m_seq][m_index].Q)
			{
				return false;
			}
		}
	}
	return true;
}
bool solver::is_move_front_legal(int i, int u, int v)
{
	int x = machine[i][v].job;
	int y = machine[i][v].seq - 1;
	int m_seq = -1;
	int m_index = -1;
	if (y >= 0)
	{
		m_seq = job[x][y].machine;
		for (int t = 0; t < machine[m_seq].size(); t++)
		{
			if (machine[m_seq][t].job == x && machine[m_seq][t].seq == y)
			{
				m_index = t; break;
			}
		}
	}
	if (m_seq != -1)
	{
		for (int t = u; t <= v- 1; t++)
		{
			if (machine[i][t].R <= machine[m_seq][m_index].R)
			{
				return false;
			}
		}
	}
	return true;
}
bool solver::tabued_by_tabu_section(int a, int u, int v,bool front,int t)
{
	vector<int> w;
	w.resize(v - u + 1);
	if (front)//v向前插入
	{
		w[0] = v;
		for (int i = 1; i < v - u + 1; i++)
		{
			w[i] = u - 1 + i;
		}
	}
	else
	{
		for (int i = 0; i < v - u ; i++)
		{
			w[i] = u +1 + i;
		}
		w[v - u] = u;
	}
	for (auto iter = tabu_section[a].begin(); iter != tabu_section[a].end();)
	{
		if (iter->tabu_time < t)
		{
			iter = tabu_section[a].erase(iter);
			continue;
		}
		if (iter->m.size() < v - u + 1)
		{
			++iter;
			continue;
		}
		int j = 0, k = 0;
		while (j < iter->m.size() && k < v - u + 1)
		{
			if (iter->m[j].job == machine[a][w[k]].job && iter->m[j].seq == machine[a][w[k]].seq)
			{
				j++;
				k++;
			}
			else {
				j = j - k + 1;
				k = 0;
			}
		}
		if (k == v - u + 1) return true;
		++iter;
	}
	return false;
}
insert solver::findmove(int ITER)
{
	int best_c =INT16_MAX;
	int best_c_tabu= INT16_MAX;
	vector<insert> best_move;
	vector<insert> best_move_tabu;
	for (int i = 0; i < critical_block.size(); i++)
	{
		for (int j = 0; j < critical_block[i].size(); j++)
		{
			int start = critical_block[i][j].start_index;
			int end = critical_block[i][j].end_index;
			if (start == end) continue;//之后修改为交换机器的领域动作
			else
			{
				if (start + 1 == end)//只有两个元素单独考虑
				{
					//printf("%d %d %d back    ", i, start, end);
					if (is_move_back_legal(i, start, end))
					{
						//printf("合法 ");
						int temp = try_to_move_back(i, start, end);
						if (tabued_by_tabu_section(i, start, end, false, ITER))
						{
							//printf("被禁忌%d \n",temp);
							if (temp < best_c_tabu)
							{
								best_c_tabu = temp;
								best_move_tabu.clear();
								best_move_tabu.push_back(insert(i, false, start, end));
							}
							else if (temp == best_c_tabu)  best_move_tabu.push_back(insert(i, false, start, end));
						}
						else
						{
							//printf("没被禁忌%d \n",temp);
							if (temp < best_c)
							{
								best_c = temp;
								best_move.clear();
								best_move.push_back(insert(i, false, start, end));
							}
							else if (temp == best_c)  best_move.push_back(insert(i, false, start, end));
						}
					}
					//else printf("不合法\n");
					continue;
				}
				for (int k = start + 1; k <= end - 1; k++)//对于每一个中间的元素，分别试探移动到最前面最后面
				{
					//printf("%d %d %d back    ", i,k, end);
					if (is_move_back_legal(i, k,end))//合法性
					{
						//printf("合法 ");
						int temp= try_to_move_back(i,k,end);
						if (tabued_by_tabu_section(i, k, end, false, ITER))
						{
							//printf("被禁忌%d \n", temp);
							if (temp < best_c_tabu)
							{
								best_c_tabu = temp;
								best_move_tabu.clear();
								best_move_tabu.push_back(insert(i, false, k, end));
							}
							else if (temp == best_c_tabu)  best_move_tabu.push_back(insert(i, false, k, end));
						}
						else
						{
							//printf("没被禁忌%d \n", temp);
							if (temp < best_c)
							{
								best_c = temp;
								best_move.clear();
								best_move.push_back(insert(i, false, k, end));
							}
							else if (temp == best_c)  best_move.push_back(insert(i, false, k, end));
						}
					}
					//else printf("不合法\n");
					//printf("%d %d %d front    ", i, start, k);
					if (is_move_front_legal(i,start,k))
					{
						//printf("合法 ");
						int temp = try_to_move_front(i,start,k);
						if (tabued_by_tabu_section(i, start, k, true, ITER))
						{
							//printf("被禁忌%d \n", temp);
							if (temp < best_c_tabu)
							{
								best_c_tabu = temp;
								best_move_tabu.clear();
								best_move_tabu.push_back(insert(i, true, start, k));
							}
							else if (temp == best_c_tabu)  best_move_tabu.push_back(insert(i, true, start, k));

						}
						else
						{
							//printf("没被禁忌%d \n", temp);
							if (temp < best_c)
							{
								best_c = temp;
								best_move.clear();
								best_move.push_back(insert(i, true, start, k));
							}
							else if (temp == best_c)  best_move.push_back(insert(i, true, start, k));
						}
					}
					//else printf("不合法 \n");
				}

				for (int k = start + 2; k <= end; k++)//对于start移动其他所有元素之后
				{
					//printf("%d %d %d back    ", i, start, k);
					if (is_move_back_legal(i, start, k))
					{
						//printf("合法 ");
						int temp = try_to_move_back(i, start,k);
						if (tabued_by_tabu_section(i, start, k, false, ITER))
						{
							//printf("被禁忌%d \n", temp);
							if (temp < best_c_tabu)
							{
								best_c_tabu = temp;
								best_move_tabu.clear();
								best_move_tabu.push_back(insert(i, false, start, k));
							}
							else if (temp == best_c_tabu)  best_move_tabu.push_back(insert(i, false, start, k));
						}
						else
						{
							//printf("没被禁忌%d \n", temp);
							if (temp < best_c)
							{
								best_c = temp;
								best_move.clear();
								best_move.push_back(insert(i, false, start, k));
							}
							else if (temp == best_c)  best_move.push_back(insert(i, false, start, k));
						}
					}
					else {					
						//printf("不合法 \n");
						break;
					}
				}
				for (int k = end - 2; k >= start; k--)//对于end移动其他所有元素之前
				{
					//printf("%d %d %d front    ", i, k, end);
					if (is_move_front_legal(i, k, end))
					{
						//printf("合法 ");
						int temp = try_to_move_front(i, k, end);
						if (tabued_by_tabu_section(i, k, end,true, ITER))
						{
							//printf("被禁忌%d \n", temp);
							if (temp < best_c_tabu)
							{
								best_c_tabu = temp;
								best_move_tabu.clear();
								best_move_tabu.push_back(insert(i, true, k, end));
							}
							else if (temp == best_c_tabu)  best_move_tabu.push_back(insert(i, true,k,end));
						}
						else
						{
							//printf("没被禁忌%d \n", temp);
							if (temp < best_c)
							{
								best_c = temp;
								best_move.clear();
								best_move.push_back(insert(i, true, k, end));
							}
							else if (temp == best_c)  best_move.push_back(insert(i, true, k, end));
						}
					}
					else {
						//printf("不合法\n ");
						break;
					}
				}
			}
		}
	}
	if (best_move.empty()) printf("空\n");
	if ((best_c_tabu < best_c && best_c_tabu < best_Cmax)|| best_move.empty())//破除禁忌
	{
		printf("选择禁忌");
		
		int select=rand() % best_move_tabu.size();
		int a = best_move_tabu[select].i;
		if (best_move_tabu[select].front) printf("front %d\n",a);
		else printf("back %d\n",a);
		for (int i = best_move_tabu[select].u; i <= best_move_tabu[select].v; i++)
		{
			printf("%d %d ", machine[a][i].job, machine[a][i].seq);
		}
		printf("\n");
		for (int i = 0; i < tabu_section[a].size(); i++)
		{
			for (int j = 0; j < tabu_section[a][i].m.size(); j++)
			{
				printf("%d %d   ", tabu_section[a][i].m[j].job, tabu_section[a][i].m[j].seq);
			}
			printf("\n");
		}
		return best_move_tabu[select];
	}
	else //选择没被禁忌的，然后把他禁忌掉
	{
		int select = rand() % best_move.size();
		//把这个序列插入禁忌表
		//printf("选择没被禁忌的\n" );
		vector<int> w;
		int V, U;
		V = best_move[select].v;
		U = best_move[select].u;
		w.resize(V-U + 1);
		if (best_move[select].front)//v向前插入
		{
			w[0] = V;
			for (int i = 1; i < V-U+1; i++)
			{
				w[i] = U- 1 + i;
			}
		}
		else
		{
			for (int i = 0; i < V - U; i++)
			{
				w[i] = U + 1 + i;
			}
			w[V - U] = U;
		}
		int b=best_move[select].i;
		TABU_section temp;
		temp.tabu_time = ITER + 5;//禁忌步长为5
		for (int i = 0; i < V - U + 1; i++)
		{
			temp.m.push_back(job_seq(machine[b][w[i]].job, machine[b][w[i]].seq));
		}
		tabu_section[b].push_back(temp);
		return best_move[select];
	}
}
void solver::makemove(int a,bool front,int u,int v)
{
	if (front)
	{
		machine[a].insert(machine[a].begin() + u, machine[a][v]);
		machine[a].erase(machine[a].begin() + v + 1);
	}
	else
	{
		machine[a].insert(machine[a].begin() + v+1, machine[a][u]);
		machine[a].erase(machine[a].begin() + u);
	}
}
void solver::sum_critical_path()//按照我的算法必须先计算R值后计算Q值
{
	sumR();
	sumQ();
	sumCmax();
	for (int i = 0; i < machine.size(); i++)
		critical_block[i].clear();
	for (int i = 0; i < machine.size(); i++)
	{
		for (int j = 0; j < machine[i].size();)
		{
			if (machine[i][j].Q + machine[i][j].R + machine[i][j].dura_time == Cmax)
			{
				int start = j;
				int end = j;
				j++;
				while (j < machine[i].size() && machine[i][j].Q + machine[i][j].R + machine[i][j].dura_time == Cmax&& machine[i][j].R== machine[i][j-1].R+ machine[i][j - 1].dura_time)
				{
					end = j;
					j++;
				}
				critical_block[i].push_back(block(start, end));
			}
			else j++;
		}
	}
}
void solver::test()
{
	random_init();
	sum_critical_path();
	best_Cmax = Cmax;
	int iter = 1;
	int maxfail = 1000;
	int mark = 0;
	while(mark<maxfail)
	{
		insert temp = findmove(iter);
		makemove(temp.i, temp.front, temp.u, temp.v);
		sum_critical_path();
		if (Cmax < best_Cmax)
		{
			best_Cmax = Cmax;
			mark = 0;
		}
		else mark++;
		iter++;
		printf("makemove %d %d %d\n", temp.i, temp.u, temp.v);
		for (int i = 0; i < machine.size(); i++)
		{
			for (int j = 0; j < machine[i].size(); j++)
				printf("%d %d %d %d  ", machine[i][j].job, machine[i][j].seq, machine[i][j].R, machine[i][j].dura_time);
			printf("\n");
		}


		printf("********\n");
		for (int i = 0; i < machine.size(); i++)
		{
			for (int j = 0; j < machine[i].size(); j++)
				printf("%d ", machine[i][j].R + machine[i][j].Q + machine[i][j].dura_time);
			printf("\n");
		}
		printf("********\n");
		for (int i = 0; i < critical_block.size(); i++)
		{
			printf("%d:", i);
			for (int j = 0; j < critical_block[i].size(); j++)
				printf(" %d %d ", critical_block[i][j].start_index, critical_block[i][j].end_index);
			printf("\n");
		}
		printf("********\n");
		printf("%d\n", Cmax);
	}
	
}
/*for (int t = 0; t < tabu_section.size();t++)
	{
		for (int i = 0; i < tabu_section[t].size(); i++)
		{
			for (int j = 0; j < tabu_section[t][i].m.size(); j++)
			{
				printf("%d %d   ", tabu_section[t][i].m[j].job, tabu_section[t][i].m[j].seq);
			}
			printf("\n");
		}
		printf("***\n");
	}
	printf("********\n");*/
/*for (int i = 0; i < machine.size(); i++)
	{
		for (int j = 0; j < machine[i].size(); j++)
			printf("%d %d %d %d  ", machine[i][j].job, machine[i][j].seq, machine[i][j].R, machine[i][j].dura_time);
		printf("\n");
	}


	printf("********\n");
	for (int i = 0; i < machine.size(); i++)
	{
		for (int j = 0; j < machine[i].size(); j++)
			printf("%d ", machine[i][j].R + machine[i][j].Q + machine[i][j].dura_time);
		printf("\n");
	}
	printf("********\n");
	for (int i = 0; i < critical_block.size(); i++)
	{
		printf("%d:", i);
		for (int j = 0; j < critical_block[i].size(); j++)
			printf(" %d %d ", critical_block[i][j].start_index, critical_block[i][j].end_index);
		printf("\n");
	}
	printf("********\n");
	insert temp = findmove(iter);
	printf("makemove %d %d %d\n", temp.i, temp.u,temp.v);
	//makemove(temp.i, temp.front, temp.u, temp.v);
	//sum_critical_path();*/