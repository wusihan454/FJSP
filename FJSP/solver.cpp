#include"solver.h"
#include<algorithm>

solver::solver(int a, int b, int c, int* d, procedure** e,int** f) :job_count(a), machine_count(b), candidate(c), procedure_count(d), message(e),T(f)
{
	job = new workpiece * [job_count];
	for (int i = 0; i < job_count; i++)
	{
		job[i] = new workpiece[procedure_count[i]];
	}
	machine.resize(machine_count);
	critical_block.resize(machine_count);
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

void solver::random_init()//���������ʼ��
{
	for (int i = 0; i < job_count; i++)
	{
		for (int j = 0; j < procedure_count[i]; j++)//i������ÿ���������ѡ��һ������
		{
			int choose = j * candidate + rand() % T[i][j];
			job[i][j].machine = message[i][choose].machine;
			job[i][j].dura_time = message[i][choose].time;
		}
	}
	for (int i = 0; i < machine_count; i++)
		machine[i].clear();
	//����Ŷ�n��������˳��
	int* Q;
	Q = new int[job_count];
	for (int i = 0; i < job_count; i++)
		Q[i] = i;
	random_shuffle(Q, Q + job_count);
	//for (int i = 0; i < job_count; i++) printf("%d ", Q[i]);
	//printf("\n");
	Cmax = 0;
	for (int i = 0; i < job_count; i++)
	{
		int choose = Q[i];
		for (int j = 0; j < procedure_count[choose]; j++)
		{
			int jp, mp;
			int select_machine = job[choose][j].machine;
			if (j == 0) jp = 0;
			else jp = job[choose][j - 1].start_time + job[choose][j - 1].dura_time;
			int temp = machine[select_machine].size();
			if (temp == 0) mp = 0;
			else
			{
				mp = machine[select_machine][temp - 1].start_time + machine[select_machine][temp - 1].dura_time;
			}
			job[choose][j].start_time = jp < mp ? mp : jp;
			machine[select_machine].push_back(operation(choose,j, job[choose][j].start_time, job[choose][j].dura_time));
			int count = 1;
			if (j == procedure_count[choose] - 1)
			{
				if (Cmax < job[choose][j].start_time+ job[choose][j].dura_time)
				{
					Cmax = job[choose][j].start_time + job[choose][j].dura_time;
					C_m = select_machine;
					C_index = temp;
					count = 1;
				}
				else if (Cmax == job[choose][j].start_time + job[choose][j].dura_time)
				{
					count++;
					if (rand() % count == 0)
					{
						Cmax = job[choose][j].start_time + job[choose][j].dura_time;
						C_m = select_machine;
						C_index = temp;
					}
				}
			}
		}
	}
	delete[] Q;

}

void solver::sumQ()
{
	queue<du_0> Q;
	for (int i=0;i<machine.size();i++)
	{
		if (machine[i].empty()) continue;
		int length = machine[i].size();
		for (int j=0;j< length -1;j++)
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
		int m = machine[temp.machine][temp.index].Q + machine[temp.machine][temp.index].dura_time;
		if (temp.index > 0)
		{
			machine[temp.machine][temp.index - 1].du--;
			if(machine[temp.machine][temp.index - 1].Q< m)
				machine[temp.machine][temp.index - 1].Q = m;
			if (machine[temp.machine][temp.index - 1].du == 0) Q.push(du_0(temp.machine, temp.index - 1));
		}
		int i=machine[temp.machine][temp.index].job;
		int j = machine[temp.machine][temp.index].seq;
		if (j > 0)
		{
			int dex = job[i][j - 1].machine;
			for (int k = 0; k < machine[dex].size(); k++)
			{
				if (machine[dex][k].job == i && machine[dex][k].seq == j - 1)
				{
					machine[dex][k].du--;
					if (machine[dex][k].du == 0) Q.push(du_0(dex, k));
					if (machine[dex][k].Q < m) machine[dex][k].Q = m;
					break;
				}
			}
		}
		Q.pop();
	}
}
void solver::sum_critical_path() //��Cmax��ǰ����,
{
	for (int i = 0; i < machine.size(); i++)
		critical_block[i].clear();
	int i = C_m;
	int j = C_index;
	while (machine[i][j].start_time != 0)
	{

			int start = j;
			int end = j;
			while (j-1>=0)
			{
				if (machine[i][j].start_time != machine[i][j - 1].start_time + machine[i][j - 1].dura_time) break;
				start = j - 1;
				j--;
			}
			int x = critical_block[i].size() - 1;
			if (x>=0&&critical_block[i][x].start_index == end + 1)
				critical_block[i][x].start_index = start;
			else critical_block[i].push_back(block(start, end));
			printf("%d %d %d\n",i,start,end);
			if (machine[i][start].start_time == 0)
			{
				j = -1;
				break;
			}
			else
			{
				int temp_j = machine[i][start].job;
				int temp_s = machine[i][start].seq-1;
				if (temp_s < 0) printf("error%d %d\n",i,start);
 				i= job[temp_j][temp_s].machine;
				for (int k = 0; k < machine[i].size(); k++)
					if (machine[i][k].job == temp_j && machine[i][k].seq == temp_s)
					{
						j = k;
						break;
					} 
			}
	}
	if (j != -1)
	{
		int x = critical_block[i].size() - 1;
		if (x >= 0 && critical_block[i][x].start_index == j+1)
			critical_block[i][x].start_index =j;
		else critical_block[i].push_back(block(j,j));
	}
}

void solver::test()
{
	random_init();
	/*for (int i = 0; i < job_count; i++)
	{
		for (int j = 0; j < procedure_count[i] * candidate; j++)
			printf("%d %d   ", message[i][j].machine, message[i][j].time);
		printf("\n");
	}*/
	for (int i = 0; i < machine.size(); i++)
	{
		for (int j = 0; j < machine[i].size(); j++)
			printf("%d %d %d %d  ", machine[i][j].job, machine[i][j].seq, machine[i][j].start_time, machine[i][j].dura_time);
		printf("\n");
	}
	printf("\n\n\n");
	for (int i = 0; i < job_count; i++)
	{
		for (int j = 0; j <procedure_count[i]; j++)
			printf("%d %d %d   ",job[i][j].machine, job[i][j].start_time, job[i][j].dura_time);
		printf("\n");
	}
	sum_critical_path();
	sumQ();
	for (int i = 0; i < critical_block.size(); i++)
	{
		printf("%d:", i);
		for (int j = 0; j < critical_block[i].size(); j++)
			printf(" %d %d ",critical_block[i][j].start_index, critical_block[i][j].end_index);
		printf("\n");
	}
	for (int i = 0; i < machine.size(); i++)
	{
		for (int j = 0; j < machine[i].size(); j++)
			printf("%d ", machine[i][j].R + machine[i][j].Q + machine[i][j].dura_time);
		printf("\n");
	}
	printf("%d",Cmax);
}