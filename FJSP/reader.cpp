#include"reader.h"
void reader::loadfile(char* input_file_name) {
	FILE* stream1;
	freopen_s(&stream1, input_file_name, "r", stdin);
	scanf_s("%d%d%d", &job_count, &machine_count, &candidate);
	procedure_count = new int[job_count];
	message = new procedure * [job_count];
	T = new int* [job_count];
	for (int i = 0; i < job_count; i++)
	{
		scanf_s("%d", &procedure_count[i]);
		T[i] = new int[procedure_count[i]];
		message[i] = new procedure[procedure_count[i] * candidate];
		for (int j = 0; j < procedure_count[i]; j++)
		{
			int temp;
			scanf_s("%d", &temp);
			T[i][j] = temp;
			int start = j * candidate;
			int end = j * candidate + temp;
			for (int k = start; k < end; k++)
				scanf_s("%d%d", &message[i][k].machine, &message[i][k].time);
		}
	}
	fclose(stdin);
}