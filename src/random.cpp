#include <cstdlib>
#include <ctime>
#include <cmath>
#include "random.h"
#include "para.h"

void random_gen::init() {
	srand(time(NULL) * (para::get_process_rank() + 1));
}

double random_gen::rand_double() {
	return (double)rand()/(double)RAND_MAX;
}

double random_gen::rand_tau() {
	return -log(rand_double());
}

double random_gen::rand_phi() {
	return rand_double() * 2 * M_PI;
}

double random_gen::rand_theta() {
	return acos(2 * rand_double() - 1);
}
