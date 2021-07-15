#pragma once
#include "solver.h"



class MAE
{
private:
	int sum_distance();
	solver solver1;
	solver solver2;
	solver solverc;//当前活跃解
public:
	void path_relinking(solver s1, solver s2, double arf, double beita, int gama);
	int sum_distance(solver s1, solver s2);
};