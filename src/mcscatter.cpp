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

MCScatter::MCScatter() {
	data_location = "./data";
	colden_location = "./data/colden";
	scatter_location = "./data/scatter";
}

MCScatter& MCScatter::get_instance() {
	static MCScatter inst;
	return inst;
}

void MCScatter::add_image(double theta, double phi, const GridParameters& gp, std::string type) {
	Image im (theta, phi, gp);
	if (type == "colden") {
		colden_images.push_back(im);
	} else if (type == "scatter") {
		scatter_images.push_back(im);
	} else {
		logs::err << "Invalid image type: " << type << "\n";
	}
}

void MCScatter::add_image(double theta, double phi, std::string type) {
	add_image(theta, phi, grid.get_parameters(), type);
}

std::string MCScatter::get_data_location() const {
	return data_location;
}

void MCScatter::set_data_location(const std::string& dl) {
	data_location = dl;
	struct stat st = {0};
	if (stat(dl.c_str(), &st) == -1) {
		mkdir(dl.c_str(), 0700);
	}
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

void MCScatter::print_misc_info() const {
	logs::out << "Data will be saved in " << data_location << "\n";
}

void MCScatter::do_scatter_simulation(int n_photons) {
	logs::out << "Starting scattering simluation\n";

	//Create folder if it doesn't exist
	struct stat st = {0};
	if (stat(scatter_location.c_str(), &st) == -1) {
		mkdir(scatter_location.c_str(), 0700);
	}
	
	int print_step = n_photons/5;

	for (int i = 1; i <= n_photons; i++) {
		
		//TODO Improve output info
		if (i%print_step == 0) {
			logs::out << logs::ProcessFlag::ALL;
			logs::out << "Proc " << para::get_process_rank() << ": " << i << " of " << n_photons << " photons\n";
			logs::out.reset_flags();
		}
		
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
		img.output_global_image(scatter_location, "scatter");
	}
	scatter_images.clear();
}

void MCScatter::do_colden_calculation() {
	logs::out << "Calculating column densities\n";
	
	//Create folder if it doesn't exist
	struct stat st = {0};
	if (stat(colden_location.c_str(), &st) == -1) {
		mkdir(colden_location.c_str(), 0700);
	}

	//Column density calculations
	int i_img = 1;
	for (auto img : colden_images) {
		logs::out << "Column density image: " << i_img << " of " << colden_images.size() << "\n";
		img.calculate_column_density(grid);
		img.output_global_image(colden_location, "colden");
		i_img++;
	}
	colden_images.clear();
}

void MCScatter::clear_grid() {
	grid.clear();
}

void MCScatter::clear_images() {
	colden_images.clear();
	scatter_images.clear();
}

void MCScatter::set_grid(const Grid& new_grid) {
	grid.clear();
	grid = new_grid;
}

const Grid& MCScatter::get_grid() const {
	return grid;
}

const std::list<Image>& MCScatter::get_scatter_images() const {
	return scatter_images;
}

const std::list<Image>& MCScatter::get_colden_images() const {
	return colden_images;
}
