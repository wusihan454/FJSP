#pragma once
#include<iostream>
#include<cstring>
using namespace std;
struct procedure
{
	int machine;
	int time;
	procedure(int a=-1,int b=-1):machine(a),time(b){}
};
class reader {
public:
	int job_count;
	int machine_count;
	int candidate;
	int* procedure_count;//����ÿһ��job��һ��procedure_count��
	int** T;//ÿ��������ÿ�������ѡ�������
	procedure** message;//message[i][j]��ʾ�� i����ҵ��һ���������Ϣ��
	void loadfile(char* input_file_name);
};