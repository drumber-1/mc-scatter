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
void do_scatter_simulation(int nPhotons);
void do_colden_calculation();
void dispose(); //Exit safely, as success
void term(); //Exit safely, as failure

extern void init_para(int argc, char *argv[]);
extern int global_sum(int val);
extern void dispose_para();
extern void init_random();
extern void init_grid();
extern void init_usr();
extern void init_text();
extern Photon generate_photon();
extern double rand_double();
extern double toRad(double angle);
extern double get_rho(double x, double y, double z);

int main(int argc, char *argv[]){

	init(argc, argv);
	
	if(make_scatter_image){
		if(procRank == 0){
			cout << "Starting scattering simluation" << endl;
		}
		do_scatter_simulation(NSAMPLES/procSize);
	}
	
	if(make_colden_image){
		if(procRank == 0) {
			cout << "Calculating column densities" << endl;
		}
		do_colden_calculation();
	}

	dispose();
	return 0;
}

void init(int argc, char *argv[]){ //Order of init calls is important!
	init_para(argc, argv);
	init_random();
	
	set_defaults();
	
	init_usr();
	
	init_grid();
}

void set_defaults(){
	albedo = 1.0;
	opacity = 1.0;
	
	for(int i = 0; i < 3; i++){
		grid_ncells[i] = 100;
		grid_left[i] = -10;
		grid_right[i] = 10;
	}
	
	make_scatter_image = true;
	sub_scatter_image = false; //Whether to output image data from each processor
	
	make_colden_image = true;
}

void do_scatter_simulation(int nPhotons){
	
	int print_step = nPhotons/5;

	for(int i = 1; i <= nPhotons; i++){
		
		//TODO Improve output info
		if(i%print_step == 0){
			cout << "Proc " << procRank << ": " << i << " of " << nPhotons << " photons" << endl;
		}
		
		Photon p = generate_photon();
		
		while(true){
			p.update();
			
			if(p.absorbed || p.escaped){
				break;
			}
			
			if(p.scattered){ //Peel of a photon
				for(list<Image>::iterator img = scatter_images.begin(); img != scatter_images.end(); img++){
					double theta = (*img).get_theta();
					double phi = (*img).get_phi();
					Photon p_peel = p.peel(theta, phi);
					while(!p_peel.escaped){
						p_peel.update();
					}
					double weight = exp(-1*p_peel.get_tau_cur());
					(*img).add(p_peel.pos[0], p_peel.pos[1], p_peel.pos[2], weight);
				}
			}
		}
		
	} //Photons
	
	for(list<Image>::iterator img = scatter_images.begin(); img != scatter_images.end(); img++){
		if(sub_scatter_image){
			(*img).output_local_image();
		}
		(*img).output_global_image();
	}
}

void do_colden_calculation(){
	
	//Column density calculations
	for(list<Image>::iterator img = colden_images.begin(); img != colden_images.end(); img++){
	
		double theta = (*img).get_theta();
		double phi = (*img).get_phi();
		
		for(int i = 0; i < (*img).get_npixels(0); i++){
			for(int j = 0; j < (*img).get_npixels(1); j++){
				double ximage = (*img).get_left_bound(0) + (double)i*(*img).get_spacing(0);
				double yimage = (*img).get_left_bound(1) + (double)j*(*img).get_spacing(1);
				//Take zimage = 0
				
				//Instead of trying to find the edge of the grid, for each point
				//we will fire two photons, one in each direction from the centre
				
				//I think this is right
				//double x = ximage*sin(theta) + yimage*cos(theta);
				//double y = ximage*cos(theta)*sin(phi) - yimage*sin(theta)*sin(phi);
				//double z = ximage*cos(theta)*cos(phi) - yimage*sin(theta)*cos(phi);
				
				double x = 0.0;
				double y = ximage;
				double z = yimage;
				
				Photon p1 (x, y, z, theta, phi, true);
				while(!p1.escaped){
					p1.update();
				}
				double colden = p1.get_tau_cur();
				
				Photon p2 (x, y, z, theta + M_PI, -1*phi, true);
				while(!p2.escaped){
					p2.update();
				}
				colden += p2.get_tau_cur();
				(*img).add(x, y, z, colden);
			}				
		}
		
	}
	
	for(list<Image>::iterator img = colden_images.begin(); img != colden_images.end(); img++){
		(*img).output_global_image();
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
