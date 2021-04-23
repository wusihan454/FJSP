#pragma once
#include"reader.h"
#include<vector>
#include<queue>
#include<climits>
struct workpiece
{
	int machine;//����
	int dura_time;//����ʱ��
	int start_time;
	workpiece() {}
};
struct operation
{
	int job;//����
	int seq;//����
	int start_time;//��ʼʱ��
	int dura_time;//����ʱ��
	int Q;
	int R;//����㵽��ǰ������ʱ��
	int du;//�����ڵ�ĸ���
	operation() {}
	operation(int a, int c, int d,int e) :job(a),seq(c), start_time(d), dura_time(e)
	{
	}
};

struct block//һ����������Ĺؼ���
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
	int tabu_time;//���ɵ�tabu_time
	vector<job_seq> m;
};
class solver {
private:
	int job_count;
	int machine_count;
	int candidate;
	int* procedure_count;//����ÿһ��job��һ��procedure_count��
	int** T;
	procedure** message;
	workpiece** job;
	int** tabu_change_machine;//���ɽ�������
	vector<vector<TABU_section>> tabu_section;
	vector<vector<operation>> machine;
	vector<vector<block>> critical_block;
	int Cmax;
	int best_Cmax;
	void random_init();//��������ʼ��
	void sum_critical_path();
	void sumQ();
	void sumR();
	void sumCmax();
	insert findmove(int ITER);//t��ʾ��ǰ�ĵ�������
	void makemove(int a, bool front, int u, int v);
	bool tabued_by_tabu_section(int i, int u, int v, bool front,int t);
	int try_to_move_front(int a,int u,int v);//vŲ����u��ǰ��ȥ
	int try_to_move_back(int a,int u,int v); //uŲ����v֮��
	bool is_move_front_legal(int i, int u, int v);
	bool is_move_back_legal(int i, int u, int v);
	//void sum_critical_block();
public:
	solver(int a, int b, int c, int* d, procedure** e,int** f);
	void test();
	~solver();
};