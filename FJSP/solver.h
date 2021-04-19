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
	int start_time;//��ʼʱ��
	int dura_time;//����ʱ��
	int Q;
	int R;//����㵽��ǰ������ʱ��
	int du;//�����ڵ�ĸ���
	operation() {}
	operation(int a, int c, int d,int e) :job(a),seq(c), start_time(d), dura_time(e)
	{
		Q = -1; 
		R = start_time;
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

class solver {
private:
	int job_count;
	int machine_count;
	int candidate;
	int* procedure_count;//����ÿһ��job��һ��procedure_count��
	int** T;
	procedure** message;
	workpiece** job;
	vector<vector<operation>> machine;
	vector<vector<block>> critical_block;
	int Cmax;
	int best_Cmax;
	int C_m, C_index;//cmax��Ӧ�Ļ����Լ��ڻ����е�λ��
	void random_init();//��������ʼ��
	void sum_critical_path();
	void sumQ();
	//void sum_critical_block();
public:
	solver(int a, int b, int c, int* d, procedure** e,int** f);
	void test();
	~solver();
};