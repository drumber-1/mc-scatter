#include <string>
#include <iostream>
#include <list>
#include <cmath>
#include <sys/types.h>
#include <sys/stat.h>

#include "mcscatter.h"
#include "photon.h"
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

void MCScatter::add_image(double theta, double phi, const std::string& type) {

	//TODO check image size for 0s and match grid in that case
	ImageParameters ip = config.image_size;
	for (int i = 0; i < 2; i++) {
		if (config.image_size.npixels[i] == 0) {
			ip.npixels[i] = grid.get_parameters().ncells[i];
		}
		if (config.image_size.left_boundary[i] == config.image_size.right_boundary[i]) {
			ip.left_boundary[i] = grid.get_parameters().left_boundary[i];
			ip.right_boundary[i] = grid.get_parameters().right_boundary[i];
		}
	}

	Image im (theta, phi, ip);
	if (type == "colden") {
		colden_images.push_back(im);
	} else if (type == "scatter") {
		scatter_images.push_back(im);
	} else {
		logs::err << "Invalid image type: " << type << "\n";
	}
}

void MCScatter::clear_images() {
	colden_images.clear();
	scatter_images.clear();
}


void MCScatter::clear_grid() {
	grid.clear();
}

bool MCScatter::read_grid(const std::string& filetype, const std::string& filename, bool in_data_location) {

	if (!FileIOInterface::file_type_supported(filetype)) {
		logs::err << filetype << " is an unrecognised filetype\n";
		return false;
	}

	clear_grid();
	
	std::string full_path;
	if (in_data_location) {
		full_path = config.data_location + std::string("/") + filename;
	} else {
		full_path = filename;
	}
	
	grid = FileIOInterface::read_file(filetype, full_path, config.grid_limits);
	return true;
}

bool MCScatter::write_grid(const std::string& filetype, const std::string& filename, bool in_data_location) {

	if (!FileIOInterface::file_type_supported(filetype)) {
		logs::err << filetype << " is an unrecognised filetype\n";
		return false;
	}
	
	std::string full_path;
	if(in_data_location) {
		full_path = config.data_location + std::string("/") + filename;
	} else {
		full_path = filename;
	}
	
	FileIOInterface::write_file(filetype, full_path, grid);
	return true;
}

bool MCScatter::read_config(const std::string& filename) {
	try {
		config.set_lua(filename);
	} catch (LuaException& e) {
		logs::err << e.what() << "\n";
		return false;
	}
	return true;
}

