#include <mpi.h>
#include "para.h"

int proc_rank, proc_size;

void para::init_para(int argc, char *argv[]){
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
}

int para::global_sum(int val){
	int g_val = 0.0;
	MPI_Reduce(&val, &g_val, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	return g_val;
}

void para::barrier(){
	MPI_Barrier(MPI_COMM_WORLD);
}

void para::dispose_para(){
	MPI_Finalize();
}
	
int para::get_process_rank(){
	return proc_rank;
}

int para::get_process_size(){
	return proc_size;
}

