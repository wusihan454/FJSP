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
	int* procedure_count;//对于每一个job有一个procedure_count；
	int** T;//每个工件的每个工序可选择机器数
	procedure** message;//message[i][j]表示第 i个作业的一个工序的信息；
	void loadfile(char* input_file_name);
};