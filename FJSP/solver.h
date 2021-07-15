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
	int dura_time;//持续时间
	int Q;
	int R;//从起点到当前操作的时间
	int du;//后续节点的个数
	operation(){}
	operation(int a, int c, int d,int e) :job(a),seq(c),R(d), dura_time(e)
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
struct change_machine//将a机器上的u挪动到b机器的v之后
{
	int a; int u; int b; int v; int t;
	change_machine(int a1,int u1,int b1,int v1,int t1):a(a1),u(u1),b(b1),v(v1),t(t1){ }
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
	void random_init();//随机构造初始解
	void sum_critical_path();
	void sumQ();
	void sumR();
	void sumCmax();
	pair<insert,int> findmove(int ITER);//t表示当前的迭代次数，返回的second为最好的最好的优化值
	pair<change_machine,int> find_change_machine(int ITER);//返回的second为最好的最好的优化值
	void makemove(int a, bool front, int u, int v);
	bool tabued_by_tabu_section(int i, int u, int v, bool front,int t);
	int try_to_move_front(int a,int u,int v);//v挪动到u的前面去
	int try_to_move_back(int a,int u,int v); //u挪动到v之后
	pair<int,int> try_to_change_machine(int a, int u, int b, int t);//试图将a机器上的u挪动到b机器,且在b机器其执行时间为t
	bool is_move_front_legal(int i, int u, int v);
	bool is_move_back_legal(int i, int u, int v);
	void makechange(int a,int u,int b,int v,int t);
	void randommove();
	pair<int, int> is_change_machine_legal(int a, int u, int b, int t);//返回合法区间的left,right值，从left的前面到right的后面都可以插入
	//void sum_critical_block();
public:
	int job_count;
	int machine_count;
	int candidate;
	int* procedure_count;//对于每一个job有一个procedure_count；
	int** T;
	procedure** message;
	workpiece** job;//保存现在所有job的所有工序实际上在哪个机器上
	int** tabu_change_machine;//禁忌交换机器
	vector<vector<TABU_section>> tabu_section;
	vector<vector<operation>> machine;
	vector<vector<block>> critical_block;
	int Cmax;
	int best_Cmax;
	solver(int a, int b, int c, int* d, procedure** e,int** f);
	void ts_change_machine();
	void ts_insert();
	void ts_mix();
	void Its_mix(int iter);
	//int  get_jobcount(){return job_count;}
	~solver();
};
