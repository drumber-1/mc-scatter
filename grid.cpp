#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <cmath>
#include <sys/types.h>
#include <sys/stat.h>
#include "grid.h"
#include "param.h"
#include "para.h"

extern std::string int_to_string(int x, int width);

Grid::Grid(const GridParameters& gp){
	parameters = gp;
	
	for(int i = 0; i < 3; i++){
		spacing[i] = (parameters.right_boundary[i] - parameters.left_boundary[i])/parameters.ncells[i];
	}
	
	rho_data = new double **[parameters.ncells[0]];
	
	for(int i = 0; i < parameters.ncells[0]; i++){
		rho_data[i] = new double *[parameters.ncells[1]];
	}
	
	for(int i = 0; i < parameters.ncells[0]; i++){
		for(int j = 0; j < parameters.ncells[1]; j++){
			rho_data[i][j] = new double [parameters.ncells[2]];
		}
	}
	
	for(int i = 0; i < parameters.ncells[0]; i++){
		for(int j = 0; j < parameters.ncells[1]; j++){
			for(int k = 0; k < parameters.ncells[2]; k++){
				rho_data[i][j][k] = 0;
			}
		}
	}
	
}

Grid::~Grid(){
	for(int i = 0; i < parameters.ncells[0]; i++){
		for(int j = 0; j < parameters.ncells[1]; j++){
			delete rho_data[i][j];
		}
	}
	
	for(int i = 0; i < parameters.ncells[0]; i++){
		delete rho_data[i];
	}
	
	delete rho_data;
}

void Grid::output_slices(std::string data_location, unsigned int sliced_dim) const {
	if(sliced_dim > 2){
		if(procRank == 0){
			std::cerr << "Cannot slice across dimension " << sliced_dim << std::endl;
		}
		return;
	}
	
	if(procRank == 0){
		
		//Create folder is it doesn't exist
		std::string folder_name = data_location + "/slices";
		struct stat st = {0};
		if(stat(folder_name.c_str(), &st) == -1){
			mkdir(folder_name.c_str(), 0700);
		}
	
		unsigned int unsliced_dim1 = (sliced_dim + 1) % 3;
		unsigned int unsliced_dim2 = (sliced_dim + 2) % 3;

		std::cout << "Outputting density slices" << std::endl;
		for(int i = 0; i < parameters.ncells[sliced_dim]; i++){
			std::string filename = data_location + "/slices/slice" + "_" + int_to_string(i, 4) + ".dat";
			std::ofstream fout;
			fout.open(filename.c_str());
			
			if(!fout.good()) {
				std::cerr << "Could not open output file: " << filename << std::endl;
				return;
			}
		
			for(int j = 0; j < parameters.ncells[unsliced_dim1]; j++){
				for(int k = 0; k < parameters.ncells[unsliced_dim2]; k++){
					double rho = 0;
					if(sliced_dim == 0){
						rho = rho_data[i][j][k];
					} else if(sliced_dim == 1){
						rho = rho_data[k][i][j];
					} else {
						rho = rho_data[j][k][i];
					}
					fout << j << "\t" << k << "\t" << rho << std::endl;
				}
			}
			std::cout << "Slice " << i + 1 << " of " << parameters.ncells[sliced_dim] << " completed" << "\r";
		}
		std::cout << std::endl;
	}
}

bool Grid::is_on_grid(std::vector<double> pos) const {
	for(int i = 0; i < 3; i++){
		if(pos[i] >= parameters.right_boundary[i] || pos[i] <= parameters.left_boundary[i]){
			return false;
		}
	}
	return true;
}

std::vector<double> Grid::get_position(std::vector<int> cell) const {
	std::vector<double> pos(3);
	for(int i = 0; i < 3; i++){
		pos[i] = cell[i]*spacing[i] + parameters.left_boundary[i];
	}
	return pos;
}

std::vector<int> Grid::get_cell(std::vector<double> pos) const {
	std::vector<int> cell(3);
	for(int i = 0; i < 3; i++){
		cell[i] = floor((pos[i] - parameters.left_boundary[i])/spacing[i]);
	}
	return cell;
}

double Grid::get_rho(std::vector<double> pos) const {
	std::vector<int> cell = get_cell(pos);

	for(int i = 0; i < 3; i++){
		if(cell[i] < 0){
			cell[i] = 0;
		} else if(cell[i] >= parameters.ncells[i]){
			cell[i] = parameters.ncells[i] - 1;
		}
	}
	
	return get_rho(cell);
}

double Grid::get_rho(std::vector<int> cell) const {
	for(int i = 0; i < 3; i++){
		if(cell[i] < 0){
			cell[i] = 0;
		} else if(cell[i] >= parameters.ncells[i]){
			cell[i] = parameters.ncells[i] - 1;
		}
	}
	return rho_data[cell[0]][cell[1]][cell[2]];
}

void Grid::set_rho(std::vector<int> cell, double rho){
	for(int i = 0; i < 3; i++){
		if(cell[i] < 0 || cell[i] >= parameters.ncells[i]){
			return;
		} else {
			rho_data[cell[0]][cell[1]][cell[2]] = rho;
		}
	}
}

