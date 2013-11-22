#define GRID

#include <iostream>
#include "grid.h"

#undef GRID

void init_grid();
double get_rho(double x, double y, double z);
extern double setup_density(double x, double y, double z);

void init_grid(){
	
	for(int i = 0; i < 3; i++){
		grid_space[i] = (grid_right[i] - grid_left[i])/ grid_ncells[i];
	}
	
	rhoData = new double **[grid_ncells[0]];
	
	for(int i = 0; i < grid_ncells[0]; i++){
		rhoData[i] = new double *[grid_ncells[1]];
	}
	
	for(int i = 0; i < grid_ncells[0]; i++){
		for(int j = 0; j < grid_ncells[1]; j++){
			rhoData[i][j] = new double [grid_ncells[2]];
		}
	}
	
	for(int i = 0; i < grid_ncells[0]; i++){
		for(int j = 0; j < grid_ncells[1]; j++){
			for(int k = 0; k < grid_ncells[2]; k++){
				rhoData[i][j][k] = setup_density(i*grid_space[0] + grid_left[0], j*grid_space[1] + grid_left[1], k*grid_space[2] + grid_left[2]);
			}
		}
	}
	
}

double get_rho(double x, double y, double z){ //Return density given a physical position
	int i = (x - grid_left[0])/grid_space[0];
	int j = (y - grid_left[1])/grid_space[1];
	int k = (z - grid_left[2])/grid_space[2];
	
	return rhoData[i][j][k];
}


