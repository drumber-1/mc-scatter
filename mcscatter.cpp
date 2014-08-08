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
#include "problem.h"

MCScatter::MCScatter() : grid(problem::generate_grid()){
	albedo = 1.0;
	opacity = 1.0;
	data_location = "./data";
}

MCScatter& MCScatter::get_instance(){
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
		if (para::get_process_rank() == 0) {
			std::cerr << "Invalid image type: " << type << "\n";
		}
	}
}

std::string MCScatter::get_data_location() const {
	return data_location;
}

double MCScatter::get_albedo() const {
	return albedo;
}

double MCScatter::get_opacity() const {
	return opacity;
}

void MCScatter::do_scatter_simulation(int n_photons) {
	if(para::get_process_rank() == 0){
		std::cout << "Starting scattering simluation\n";
	}

	//Create folder if it doesn't exist
	std::string folder_name = data_location + "/scatter";
	struct stat st = {0};
	if(stat(folder_name.c_str(), &st) == -1){
		mkdir(folder_name.c_str(), 0700);
	}
	
	int print_step = n_photons/5;

	for(int i = 1; i <= n_photons; i++){
		
		//TODO Improve output info
		if(i%print_step == 0){
			std::cout << "Proc " << para::get_process_rank() << ": " << i << " of " << n_photons << " photons\n";
		}
		
		Photon p = problem::generate_photon();
		
		while(true){
			p.update(grid);
			
			if(p.absorbed || p.escaped){
				break;
			}
			
			if(p.scattered){ //Peel of a photon
				for(std::list<Image>::iterator img = scatter_images.begin(); img != scatter_images.end(); img++){
					double theta = (*img).get_theta();
					double phi = (*img).get_phi();
					Photon p_peel = p.peel(theta, phi);
					while(!p_peel.escaped){
						p_peel.update(grid);
					}
					double weight = exp(-1*p_peel.get_tau_cur());
					(*img).add(p_peel.pos[0], p_peel.pos[1], p_peel.pos[2], weight);
				}
			}
		}
		
	} //Photons
	
	for(std::list<Image>::iterator img = scatter_images.begin(); img != scatter_images.end(); img++){
		//(*img).output_local_image("scatter");
		(*img).output_global_image(get_data_location(), "scatter");
	}
}

void MCScatter::do_colden_calculation() {
	if(para::get_process_rank() == 0) {
		std::cout << "Calculating column densities\n";
	}
	
	//Create folder if it doesn't exist
	std::string folder_name = data_location + "/colden";
	struct stat st = {0};
	if(stat(folder_name.c_str(), &st) == -1){
		mkdir(folder_name.c_str(), 0700);
	}

	//Column density calculations
	int i_img = 1;
	for(std::list<Image>::iterator img = colden_images.begin(); img != colden_images.end(); img++){
		if(para::get_process_rank() == 0){
			std::cout << "Column density image: " << i_img << " of " << colden_images.size() << "\n";
		}
		(*img).calculate_column_density(grid);
		(*img).output_global_image(get_data_location(), "colden");
		i_img++;
	}
}
