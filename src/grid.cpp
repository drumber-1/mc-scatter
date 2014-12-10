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
#include "log.h"
#include "util.h"
#include "para.h"

Grid::Grid() : empty(true) {
	for (int i = 0; i < 3; i++) {
		parameters.ncells[i] = 0;
		parameters.left_boundary[i] = 0.0;
		parameters.right_boundary[i] = 0.0;
	}
	albedo = 1.0;
	opacity = 1.0;
}


Grid::Grid(const GridParameters& gp) : empty(false) {
	parameters = gp;
	
	for (int i = 0; i < 3; i++) {
		spacing[i] = (parameters.right_boundary[i] - parameters.left_boundary[i]) / parameters.ncells[i];
	}
	
	int total_cells = parameters.ncells[0]*parameters.ncells[1]*parameters.ncells[2];
	rho_data.resize(total_cells, 0);
	
	albedo = 1.0;
	opacity = 1.0;
	
}

//This ordering minimises cache misses when k is the inner loop
inline int Grid::get_index(int i, int j, int k) const {
	return k + j*parameters.ncells[2] + i*parameters.ncells[2]*parameters.ncells[1];
}

inline int Grid::get_index(const Point& cell) const {
	return get_index(cell[0], cell[1], cell[2]);
}


void Grid::clear() {

	rho_data.resize(0.0, 0);
	
	for (int i = 0; i < 3; i++) {
		parameters.ncells[i] = 0;
		parameters.left_boundary[i] = 0.0;
		parameters.right_boundary[i] = 0.0;
	}
		
	empty = true;
}

void Grid::output_slices(std::string data_location, unsigned int sliced_dim) const {
	if (sliced_dim > 2) {
		logs::err << "Cannot slice across dimension " << sliced_dim << std::endl;
		return;
	}
	
	if (empty) {
		logs::err << "Cannot slice, grid is empty" << std::endl;
		return;
	}
	
	if (para::get_process_rank() == 0) {
		//Create folder is it doesn't exist
		std::string folder_name = data_location + "/slices";
		struct stat st = {0};
		if (stat(folder_name.c_str(), &st) == -1) {
			mkdir(folder_name.c_str(), 0700);
		}
	
		unsigned int unsliced_dim1 = (sliced_dim + 1) % 3;
		unsigned int unsliced_dim2 = (sliced_dim + 2) % 3;

		logs::out << "Outputting density slices" << std::endl;
		for (int i = 0; i < parameters.ncells[sliced_dim]; i++) {
			std::string filename = data_location + "/slices/slice" + "_" + util::int_to_string(i, 4) + ".dat";
			std::ofstream fout;
			fout.open(filename.c_str());
			
			if (!fout.good()) {
				logs::err << "Could not open output file: " << filename << std::endl;
				return;
			}
		
			for (int j = 0; j < parameters.ncells[unsliced_dim1]; j++) {
				for (int k = 0; k < parameters.ncells[unsliced_dim2]; k++) {
					double rho = 0;
					if (sliced_dim == 0) {
						rho = rho_data[get_index(i, j, k)];
					} else if (sliced_dim == 1) {
						rho = rho_data[get_index(k, i, j)];
					} else {
						rho = rho_data[get_index(j, k, i)];
					}
					fout << j << "\t" << k << "\t" << rho << std::endl;
				}
			}
			logs::out << "Slice " << i + 1 << " of " << parameters.ncells[sliced_dim] << " completed" << "\r";
		}
		logs::out << std::endl;
	}
}

void Grid::print_info() const {
	
	if (empty) {
		logs::out << "Grid is empty" << "\n";
		return;
	}
	logs::out << "Grid has " << parameters.ncells[0] << "x"
	                         << parameters.ncells[1] << "x"
	                         << parameters.ncells[2] << " cells\n";
	logs::out << "Dimensions are:\n";
	for (int i = 0; i < 3; i++) {
		logs::out << "\t" << parameters.left_boundary[i] << " to " << parameters.right_boundary[i] << "\n";
	}
}

bool Grid::is_on_grid(const Position& pos) const {
	if (pos[0] <= parameters.left_boundary[0] ||
	    pos[1] <= parameters.left_boundary[1] ||
	    pos[2] <= parameters.left_boundary[2] ||
	    pos[0] >= parameters.right_boundary[0] ||
	    pos[1] >= parameters.right_boundary[1] ||
	    pos[2] >= parameters.right_boundary[2]) {
		return false;
	} else {
		return true;
	}
}

bool Grid::is_on_grid(const Point& cell) const {
	if (cell[0] < 0 ||
	    cell[1] < 0 ||
	    cell[2] < 0 ||
	    cell[0] >= parameters.ncells[0] ||
	    cell[1] >= parameters.ncells[1] ||
	    cell[2] >= parameters.ncells[2]) {
		return false;
	} else {
		return true;
	}
}

Position Grid::get_position(const Point& cell) const {
	Position pos = {{cell[0] * spacing[0] + parameters.left_boundary[0],
	                 cell[1] * spacing[1] + parameters.left_boundary[1],
	                 cell[2] * spacing[2] + parameters.left_boundary[2]}};
	return pos;
}

Point Grid::get_cell(const Position& pos) const {
	Point cell = {{(int)floor((pos[0] - parameters.left_boundary[0]) / spacing[0]),
	               (int)floor((pos[1] - parameters.left_boundary[1]) / spacing[1]),
	               (int)floor((pos[2] - parameters.left_boundary[2]) / spacing[2])}};
	return cell;
}

double Grid::get_rho(const Position& pos) const {
	Point cell = get_cell(pos);
	return get_rho(cell);
}

double Grid::get_rho(const Point& cell) const {
	if (is_on_grid(cell)) {
		return rho_data[get_index(cell)];
	} else {
		return 0.0;
	}
}

void Grid::set_rho(const Point& cell, double rho) {
	if (is_on_grid(cell)) {
		rho_data[get_index(cell)] = rho;
	}
}

double Grid::get_spacing(int dim) const {
	return spacing[dim];
}

bool Grid::is_empty() const {
	return empty;
}

GridParameters Grid::get_parameters() const {
	return parameters;
}

double Grid::get_albedo() const {
	return albedo;
}

void Grid::set_albedo(double alb) {
	albedo = alb;
}

double Grid::get_opacity() const {
	return opacity;
}

void Grid::set_opacity(double opac) {
	opacity = opac;
}

