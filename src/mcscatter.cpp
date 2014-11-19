#include <string>
#include <iostream>
#include <list>
#include <cmath>
#include <sys/types.h>
#include <sys/stat.h>

#include "mcscatter.h"
#include "grid.h"
#include "image.h"
#include "para.h"
#include "log.h"
#include "lua.h"
#include "fileio.h"

Config config;
Grid grid;
std::list<Image> scatter_images;
std::list<Image> colden_images;

void MCScatter::init() {
	config = Config("./config.lua");
	grid = Grid();
	return;
}

void MCScatter::do_scatter_simulation(int n_photons) {
	logs::out << "Starting scattering simulation\n";

	//Create folder if it doesn't exist
	struct stat st = {0};
	if (stat(config.scatter_location.c_str(), &st) == -1) {
		mkdir(config.scatter_location.c_str(), 0700);
	}
	
	int print_step = n_photons/5;

	for (int i = 1; i <= n_photons; i++) {
		
		//TODO Improve output info
		if (i%print_step == 0) {
			logs::out << logs::ProcessFlag::ALL;
			logs::out << "Proc " << para::get_process_rank() << ": " << i << " of " << n_photons << " photons\n";
			logs::out.reset_flags();
		}
		
		//Photon p = lua::generate_photon();
		Photon p (0, 0, 0);
		
		while (true) {
			p.update(grid);
			
			if (p.absorbed || p.escaped) {
				break;
			}
			
			if (p.scattered) { //Peel of a photon
				for (auto img : scatter_images) {
					double theta = img.get_theta();
					double phi = img.get_phi();
					Photon p_peel = p.peel(theta, phi);
					while (!p_peel.escaped) {
						p_peel.update(grid);
					}
					double weight = exp(-1*p_peel.get_tau_cur());
					img.add(p_peel.pos[0], p_peel.pos[1], p_peel.pos[2], weight);
				}
			}
		}
		
	} //Photons
	
	for (auto img : scatter_images) {
		img.output_global_image(config.scatter_location, "scatter");
	}
	scatter_images.clear();
}

void MCScatter::do_colden_calculation() {
	logs::out << "Calculating column densities\n";
	
	//Create folder if it doesn't exist
	struct stat st = {0};
	if (stat(config.colden_location.c_str(), &st) == -1) {
		mkdir(config.colden_location.c_str(), 0700);
	}

	//Column density calculations
	int i_img = 1;
	for (auto img : colden_images) {
		logs::out << "Column density image: " << i_img << " of " << colden_images.size() << "\n";
		img.calculate_column_density(grid);
		img.output_global_image(config.colden_location, "colden");
		i_img++;
	}
	colden_images.clear();
}

void MCScatter::do_slices() {
	grid.output_slices(config.data_location, 0);
}

void MCScatter::print_image_info() {
	logs::out << "There are " << scatter_images.size() << " scatter images\n";
	for (auto img : scatter_images) {
		img.print_info();
	}
	logs::out << "There are " << colden_images.size() << " column density images\n";
	for (auto img : colden_images) {
		img.print_info();
	}
}

void MCScatter::print_config_info() {
	config.print();
}

void MCScatter::print_grid_info() {
	grid.print_info();
}

void MCScatter::add_image(double theta, double phi, const GridParameters& gp, const std::string& type) {
	Image im (theta, phi, gp);
	if (type == "colden") {
		colden_images.push_back(im);
	} else if (type == "scatter") {
		scatter_images.push_back(im);
	} else {
		logs::err << "Invalid image type: " << type << "\n";
	}
}

void MCScatter::add_image(double theta, double phi, const std::string& type) {
	add_image(theta, phi, grid.get_parameters(), type);
}

void MCScatter::clear_images() {
	colden_images.clear();
	scatter_images.clear();
}


void MCScatter::clear_grid() {
	grid.clear();
}

bool MCScatter::read_grid(const std::string& filetype, const std::string& filename) {

	if (!FileIOInterface::file_type_supported(filetype)) {
		logs::err << filetype << " is an unrecognised filetype\n";
		return false;
	}

	clear_grid();
	
	//These grid parameters determine the maximum number of cells to use
	GridParameters gp;
	for (int i = 0; i < 3; i++) {
		gp.ncells[i] = config.max_cells[i];
		gp.left_boundary[i] = config.left_bound[i];
		gp.right_boundary[i] = config.right_bound[i];
	}
	
	std::string full_path = config.data_location + std::string("/") + filename;
	
	grid = FileIOInterface::read_file(filetype, full_path, gp);
	return true;
}

bool MCScatter::write_grid(const std::string& filetype, const std::string& filename) {

	if (!FileIOInterface::file_type_supported(filetype)) {
		logs::err << filetype << " is an unrecognised filetype\n";
		return false;
	}
	
	std::string full_path = config.data_location + std::string("/") + filename;
	
	FileIOInterface::write_file(filetype, full_path, grid);
	return true;
}

