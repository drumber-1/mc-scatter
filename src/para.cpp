#include <mpi.h>
#include <string>
#include <algorithm>
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

void para::global_sum(double* in, double* out, int nvalues) {
	MPI_Reduce(in, out, nvalues, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
}

void para::broadcast(std::string& str) {
	int str_length = str.size();
	broadcast(&str_length);
	char* buff = new char[str_length + 1];
	if(para::get_process_rank() == 0) {
		std::copy(str.begin(), str.end(), buff);
		buff[str_length] = '\0';
	}
	MPI_Bcast(buff, str_length + 1, MPI_CHAR, 0, MPI_COMM_WORLD);
	str = std::string(buff);
	delete[] buff;
}

void para::broadcast(int* val) {
	MPI_Bcast(val, 1, MPI_INT, 0, MPI_COMM_WORLD);
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

