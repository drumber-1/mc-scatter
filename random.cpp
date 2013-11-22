#include <cstdlib>
#include <ctime>
#include <cmath>
#include "para.h"

void init_random();
double rand_double();
double rand_tau();
double rand_phi();
double rand_theta();

void init_random(){
	srand(time(NULL)*(procRank+1));
}

double rand_double(){
	return (double)rand()/(double)RAND_MAX;
}

double rand_tau(){
	return -log(rand_double());
}

double rand_phi(){
	return rand_double()*2*M_PI;
}

double rand_theta(){
	return acos(2*rand_double() - 1);
}
