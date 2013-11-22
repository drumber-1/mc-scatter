#define PARA

#include <mpi.h>
#include "para.h"

#undef PARA

void init_para(int argc, char *argv[]);
int global_sum(int val);
void dispose_para();

void init_para(int argc, char *argv[]){
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
	MPI_Comm_size(MPI_COMM_WORLD, &procSize);
}

int global_sum(int val){
	int g_val = 0.0;
	MPI_Reduce(&val, &g_val, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	return g_val;
}

void dispose_para(){
	MPI_Finalize();
}
