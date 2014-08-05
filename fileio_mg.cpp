#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include "fileio_mg.h"
#include "grid.h"
#include "para.h"

//This is a custom file format make of collection of files
//It consists of:
//	metadata.txt text file containing with key value pairs separated by colons
//	rd_lev_XX.rho binary files, containing data values in X, Y, Z, rho order in double precision
//XX is the grid level to which the file refers to. The number of these files is specified in metadata.txt

extern std::string int_to_string(int x, int width);
extern double string_to_double(const std::string& s);

//Ignores boundaries in grid_parameters
//TODO: Add re-griding for arbitrary cell spacings
Grid FileIOMG::read_file(std::string filename, const GridParameters& grid_parameters){

	if(para::get_process_rank() == 0){
		std::cout << "Opening file: " << filename << std::endl;
	}

	//First get the relevant metadata
	std::ifstream meta_file;
	std::string meta_filename = filename + "/metadata.txt";
	meta_file.open(meta_filename.c_str());
	
	if(!meta_file.is_open()){
		if(para::get_process_rank() == 0){
			std::cerr << "Could not open metadata file: " << meta_filename << std::endl;
		}
		return Grid(false);
	}
	
	//These are the variables we are looking for in the metadata
	double grid_spacing = -1;
	int n_levels = -1;
	
	while(meta_file.good()){
		std::string line;
		getline(meta_file, line);
		
		int sep_pos = line.find(":");
		std::string var_name = line.substr(0, sep_pos);
		std::string var = line.substr(sep_pos+1);
		
		if(var_name.compare("gridspacing_fine0") == 0){
			grid_spacing = string_to_double(var);
		} else if(var_name.compare("levels") == 0){
			n_levels = string_to_double(var);
		}
	}
	
	if(grid_spacing < 0){
		if(para::get_process_rank() == 0){
			std::cerr << "Grid spacing could not be read correctly" << std::endl;
		}
		return Grid(false);
	}
	if(n_levels < 0){
		if(para::get_process_rank() == 0){
			std::cerr << "Number of levels could not be read correctly" << std::endl;
		}
		return Grid(false);
	}
	
	GridParameters gp;
	for(int i = 0; i < 3; i++){
		gp.ncells[i] = grid_parameters.ncells[i];
		gp.left_boundary[i] = -0.5*gp.ncells[i]*grid_spacing;
		gp.right_boundary[i] = 0.5*gp.ncells[i]*grid_spacing;
	}
	
	Grid grid(gp);
	
	//Level 1 should be maximally refined, so skip level 0
	for(int ilev = 1; ilev < n_levels - 1; ilev++){
		
		if(para::get_process_rank() == 0){
			std::cout << "Processing level " << ilev << std::endl;
		}
	
		std::ifstream level_file;
		std::string level_filename = filename + "/rd_lev_" + int_to_string(ilev, 2) + ".rho";
		level_file.open(level_filename.c_str());
		
		if(!level_file.is_open()){
			if(para::get_process_rank() == 0){
				std::cerr << "Could not open level file: " << level_file << std::endl;
			}
			return Grid(false);
		}
		
		//Number of the finest cells (a cell on level n_levels) that will fit across the side of one cell on level ilev
		int fine_cells = pow(2, n_levels - ilev - 1);
		
		int data_points = 0;

		std::vector<int> cell(3);
		std::vector<int> cell_fine(3);
		
		//Note: Coordinates from file are cell centres
		while(!level_file.eof()){
			double x;
			double y;
			double z;
			double rho;
			
			//Data from mg is ordered XZY
			level_file.read((char*)&x, sizeof (double));
			level_file.read((char*)&z, sizeof (double));
			level_file.read((char*)&y, sizeof (double));
			level_file.read((char*)&rho, sizeof (double));
			
			//We are assuming that the grid parameters were not changed by the Grid constructor
			cell[0] = fine_cells*floor((x - gp.left_boundary[0])/(grid.get_spacing(0)*fine_cells));
			cell[1] = fine_cells*floor((y - gp.left_boundary[1])/(grid.get_spacing(1)*fine_cells));
			cell[2] = fine_cells*floor((z - gp.left_boundary[2])/(grid.get_spacing(2)*fine_cells));
			
			if(!grid.is_on_grid(cell)){
				continue;
			}
			
			for (int i = cell[0]; i < cell[0] + fine_cells; i++) {
				for (int j = cell[1]; j < cell[1] + fine_cells; j++) {
					for (int k = cell[2]; k < cell[2] + fine_cells; k++) {
						cell_fine[0] = i;
						cell_fine[1] = j;
						cell_fine[2] = k;
						grid.set_rho(cell_fine, rho);
					}
				}
			}
			data_points++;
		}
		level_file.close();
		
		if(para::get_process_rank() == 0){
			std::cout << data_points << " data points on level " << ilev << std::endl;
		}
		
	}
	
	return grid;
}

void FileIOMG::write_file(std::string filename, const Grid& grid){
	return;
}
