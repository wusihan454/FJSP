#pragma once
#include"reader.h"
#include<vector>
#include<queue>
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
	int dura_time;//����ʱ��
	int Q;
	int R;//����㵽��ǰ������ʱ��
	int du;//�����ڵ�ĸ���
	operation(){}
	operation(int a, int c, int d,int e) :job(a),seq(c),R(d), dura_time(e)
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
struct change_machine//��a�����ϵ�uŲ����b������v֮��
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
	int tabu_time;//���ɵ�tabu_time
	vector<job_seq> m;
};

class solver {
private:
	void random_init();//��������ʼ��
	void sum_critical_path();
	void sumQ();
	void sumR();
	void sumCmax();
	pair<insert,int> findmove(int ITER);//t��ʾ��ǰ�ĵ������������ص�secondΪ��õ���õ��Ż�ֵ
	pair<change_machine,int> find_change_machine(int ITER);//���ص�secondΪ��õ���õ��Ż�ֵ
	void makemove(int a, bool front, int u, int v);
	bool tabued_by_tabu_section(int i, int u, int v, bool front,int t);
	int try_to_move_front(int a,int u,int v);//vŲ����u��ǰ��ȥ
	int try_to_move_back(int a,int u,int v); //uŲ����v֮��
	pair<int,int> try_to_change_machine(int a, int u, int b, int t);//��ͼ��a�����ϵ�uŲ����b����,����b������ִ��ʱ��Ϊt
	bool is_move_front_legal(int i, int u, int v);
	bool is_move_back_legal(int i, int u, int v);
	void makechange(int a,int u,int b,int v,int t);
	void randommove();
	pair<int, int> is_change_machine_legal(int a, int u, int b, int t);//���غϷ������left,rightֵ����left��ǰ�浽right�ĺ��涼���Բ���
	//void sum_critical_block();
public:
	int job_count;
	int machine_count;
	int candidate;
	int* procedure_count;//����ÿһ��job��һ��procedure_count��
	int** T;
	procedure** message;
	workpiece** job;//������������job�����й���ʵ�������ĸ�������
	int** tabu_change_machine;//���ɽ�������
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
