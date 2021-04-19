#pragma once
#include"reader.h"
#include<vector>
#include<queue>
struct workpiece
{
	int machine;//机器
	int dura_time;//持续时间
	int start_time;
	workpiece() {}
};
struct operation
{
	int job;//工件
	int seq;//工序
	int start_time;//开始时间
	int dura_time;//持续时间
	int Q;
	int R;//从起点到当前操作的时间
	int du;//后续节点的个数
	operation() {}
	operation(int a, int c, int d,int e) :job(a),seq(c), start_time(d), dura_time(e)
	{
		Q = -1; 
		R = start_time;
	}
};

struct block//一个机器里面的关键块
{
	int start_index;
	int end_index;
	block(int a=-1,int b=-1):start_index(a),end_index(b){}
};
struct du_0
{
	int index;
	int machine;
	du_0(int a,int b):machine(a),index(b){ }
};

class solver {
private:
	int job_count;
	int machine_count;
	int candidate;
	int* procedure_count;//对于每一个job有一个procedure_count；
	int** T;
	procedure** message;
	workpiece** job;
	vector<vector<operation>> machine;
	vector<vector<block>> critical_block;
	int Cmax;
	int best_Cmax;
	int C_m, C_index;//cmax对应的机器以及在机器中的位置
	void random_init();//随机构造初始解
	void sum_critical_path();
	void sumQ();
	//void sum_critical_block();
public:
	solver(int a, int b, int c, int* d, procedure** e,int** f);
	void test();
	~solver();
};