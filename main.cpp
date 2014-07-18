#define MAIN

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include "param.h"
#include "para.h"
#include "grid.h"
#include "photon.h"
#include "image.h"

#undef MAIN

using namespace std;

void init(int argc, char *argv[]);
void set_defaults();
void do_scatter_simulation(const Grid& grid, int nPhotons);
void do_colden_calculation(const Grid&);
void dispose(); //Exit safely, as success
void term(); //Exit safely, as failure

extern void init_para(int argc, char *argv[]);
extern int global_sum(int val);
extern void dispose_para();
extern void init_random();
extern void init_grid();
extern void init_usr();
extern void init_text();
extern Grid generate_grid();
extern Photon generate_photon();
extern double rand_double();
extern double toRad(double angle);
extern double get_rho(double x, double y, double z);
extern void grid_output_slices(unsigned int sliced_dim);

int main(int argc, char *argv[]){

	init(argc, argv);
	
	Grid grid = generate_grid();
	
	if(make_grid_slices){
		grid.output_slices(data_location, 1);
	}
	
	if(make_scatter_image){
		do_scatter_simulation(grid, NSAMPLES/procSize);
	}
	
	if(make_colden_image){
		do_colden_calculation(grid);
	}

	dispose();
	return 0;
}

void init(int argc, char *argv[]){ //Order of init calls is important!
	init_para(argc, argv);
	init_random();
	
	set_defaults();
	init_usr();
}

void set_defaults(){
	albedo = 1.0;
	opacity = 1.0;
	
	make_scatter_image = true;
	sub_scatter_image = false; //Whether to output image data from each processor
	
	make_colden_image = false;
	
	make_grid_slices = false;
	
	data_location = "./data";
	controlled_setup = false;
}

void do_scatter_simulation(const Grid& grid, int nPhotons){

	if(procRank == 0){
		cout << "Starting scattering simluation" << endl;
	}

	//Create folder
	string mkdir_cmd = "mkdir -p " + data_location + "/scatter";
	system(mkdir_cmd.c_str());
	
	int print_step = nPhotons/5;

	for(int i = 1; i <= nPhotons; i++){
		
		//TODO Improve output info
		if(i%print_step == 0){
			cout << "Proc " << procRank << ": " << i << " of " << nPhotons << " photons" << endl;
		}
		
		Photon p = generate_photon();
		
		while(true){
			p.update(grid);
			
			if(p.absorbed || p.escaped){
				break;
			}
			
			if(p.scattered){ //Peel of a photon
				for(list<Image>::iterator img = scatter_images.begin(); img != scatter_images.end(); img++){
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
	
	for(list<Image>::iterator img = scatter_images.begin(); img != scatter_images.end(); img++){
		if(sub_scatter_image){
			(*img).output_local_image("scatter");
		}
		(*img).output_global_image("scatter");
	}
}

//TODO: Profile different parallisation methods here
void do_colden_calculation(const Grid& grid){

	if(procRank == 0) {
		cout << "Calculating column densities" << endl;
	}

	//Create folder
	string mkdir_cmd = "mkdir -p " + data_location + "/colden";
	system(mkdir_cmd.c_str());

	//Column density calculations
	int i_img = 1;
	for(list<Image>::iterator img = colden_images.begin(); img != colden_images.end(); img++){
		if(procRank == 0){
			cout << "Column density image: " << i_img << " of " << colden_images.size() << endl;
		}
		(*img).calculate_column_density(grid);
		(*img).output_global_image("colden");
		i_img++;
	}
	
}

void dispose(){
	dispose_para();
	exit(EXIT_SUCCESS);
}

void term(){
	dispose_para();
	exit(EXIT_FAILURE);
}
