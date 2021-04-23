#pragma once
#include"reader.h"
#include<vector>
#include<queue>
#include<climits>
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
struct insert
{
	int i;
	bool front;
	int u;
	int v;
	insert(int a, bool b, int c, int d) :i(a), front(b), u(c), v(d){}
};
struct job_seq
{
	int job;
	int seq;
	job_seq(int a,int b):job(a),seq(b){ }
};
struct TABU_section
{
	int tabu_time;//禁忌到tabu_time
	vector<job_seq> m;
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
	int** tabu_change_machine;//禁忌交换机器
	vector<vector<TABU_section>> tabu_section;
	vector<vector<operation>> machine;
	vector<vector<block>> critical_block;
	int Cmax;
	int best_Cmax;
	void random_init();//随机构造初始解
	void sum_critical_path();
	void sumQ();
	void sumR();
	void sumCmax();
	insert findmove(int ITER);//t表示当前的迭代次数
	void makemove(int a, bool front, int u, int v);
	bool tabued_by_tabu_section(int i, int u, int v, bool front,int t);
	int try_to_move_front(int a,int u,int v);//v挪动到u的前面去
	int try_to_move_back(int a,int u,int v); //u挪动到v之后
	bool is_move_front_legal(int i, int u, int v);
	bool is_move_back_legal(int i, int u, int v);
	//void sum_critical_block();
public:
	solver(int a, int b, int c, int* d, procedure** e,int** f);
	void test();
	~solver();
};