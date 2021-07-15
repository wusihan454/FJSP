#include "mae.h"

void MAE::path_relinking(solver s1, solver s2,double arf, double beita, int gama)
{

}

int MAE::sum_distance(solver s1,solver s2)
{
	int jobcount = s1.job_count;
	int result = 0;
	for (int i = 0; i < jobcount; i++)
	{
		for (int j = 0; j < s1.procedure_count[i];j++)
		{
			if (s1.job[i][j].machine == s2.job[i][j].machine)//在同一机器
			{
				int m_seq = s1.job[i][j].machine;
				//找到s1在机器上的位置
				int k1=0;
				while (k1 < s1.machine[m_seq].size())
				{
					if (s1.machine[m_seq][k1].job == i && s1.machine[m_seq][k1].seq == j)
						break;
					k1++;
				}
				int k2 = 0;
				while (k2 < s2.machine[m_seq].size())
				{
					if (s2.machine[m_seq][k2].job == i && s1.machine[m_seq][k2].seq == j)
						break;
					k2++;
				}
				result += abs(k1 - k2);
			}
			else  //在不同的机器上
			{
				int m1_seq = s1.job[i][j].machine;
				int m2_seq = s2.job[i][j].machine;
				//找到s1在机器上的位置
				int k1 = 0;
				while (k1 < s1.machine[m1_seq].size())
				{
					if (s1.machine[m1_seq][k1].job == i && s1.machine[m1_seq][k1].seq == j)
						break;
					k1++;
				}
				int k2 = 0;
				while (k2 < s2.machine[m2_seq].size())
				{
					if (s2.machine[m2_seq][k2].job == i && s1.machine[m2_seq][k2].seq == j)
						break;
					k2++;
				}

			}
		}
	}
}