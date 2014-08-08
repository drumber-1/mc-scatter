#include <string>

namespace para {
	void init_para(int argc, char *argv[]);
	int global_sum(int val);
	void global_sum(double* in, double* out, int nvalues);
	void broadcast(std::string& str);
	void broadcast(int* val);
	void barrier();
	void dispose_para();
	int get_process_rank();
	int get_process_size();
}
