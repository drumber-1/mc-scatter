#define GRID

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <unistd.h>
#include "grid.h"
#include "param.h"
#include "para.h"

#undef GRID

void init_grid();
double get_rho(double x, double y, double z);
void grid_output_slices(unsigned int sliced_dim);

extern double setup_density(double x, double y, double z);
extern void setup_density();
extern std::string int_to_string(int n, int width);

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
				if(!controlled_setup){
					rhoData[i][j][k] = setup_density(i*grid_space[0] + grid_left[0], j*grid_space[1] + grid_left[1], k*grid_space[2] + grid_left[2]);
				} else {
					rhoData[i][j][k] = 0;
				}
			}
		}
	}
	
	if(controlled_setup){
		setup_density();
	}
	
}

double get_rho(double x, double y, double z){ //Return density given a physical position
	int cell[3];
	cell[0] = (x - grid_left[0])/grid_space[0];
	cell[1] = (y - grid_left[1])/grid_space[1];
	cell[2] = (z - grid_left[2])/grid_space[2];

	for(int i = 0; i < 3; i++){
		if(cell[i] < 0){
			cell[i] = 0;
		} else if(cell[i] >= grid_ncells[i]){
			cell[i] = grid_ncells[i] - 1;
		}
	}
	
	return rhoData[cell[0]][cell[1]][cell[2]];
}

void grid_output_slices(unsigned int sliced_dim){
	if(sliced_dim > 2){
		if(procRank == 0){
			std::cerr << "Cannot slice across dimension " << sliced_dim << std::endl;
		}
		return;
	}
	
	//Create folder
	std::string mkdir_cmd = "mkdir -p " + data_location + "/slices";
	system(mkdir_cmd.c_str());
	
	unsigned int unsliced_dim1 = (sliced_dim + 1) % 3;
	unsigned int unsliced_dim2 = (sliced_dim + 2) % 3;
	
	if(procRank == 0){
		std::cout << "Outputting density slices" << std::endl;
		for(int i = 0; i < grid_ncells[sliced_dim]; i++){
			std::string filename = data_location + "/slices/slice" + "_" + int_to_string(i, 4) + ".dat";
			std::ofstream fout;
			fout.open(filename.c_str());
			
			if(!fout.good()) {
				std::cerr << "Could not open output file: " << filename << std::endl;
				return;
			}
		
			for(int j = 0; j < grid_ncells[unsliced_dim1]; j++){
				for(int k = 0; k < grid_ncells[unsliced_dim2]; k++){
					double rho = 0;
					if(sliced_dim == 0){
						rho = rhoData[i][j][k];
					} else if(sliced_dim == 1){
						rho = rhoData[k][i][j];
					} else {
						rho = rhoData[j][k][i];
					}
					fout << j << "\t" << k << "\t" << rho << std::endl;
				}
			}
			std::cout << "Slice " << i + 1 << " of " << grid_ncells[sliced_dim] << " completed" << "\r";
		}
		std::cout << std::endl;
	}
}


