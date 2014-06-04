#define MAIN

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <mpi.h>
#include "param.h"
#include "para.h"
#include "grid.h"
#include "photon.h"
#include "image.h"

#undef MAIN

using namespace std;

void init(int argc, char *argv[]);
void set_defaults();
void run_scatter_simulation(int nPhotons);
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

int main(int argc, char *argv[]){

	init(argc, argv);

	run_scatter_simulation(NSAMPLES/procSize);

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
	
	make_image = true;
	sub_image = false; //Whether to output image data from each processor
}

void run_scatter_simulation(int nPhotons){
	
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
			
			if(make_image && p.scattered){ //Peel of a photon
			
				for(list<Image>::iterator img = images.begin(); img != images.end(); img++){
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
	
	for(list<Image>::iterator img = images.begin(); img != images.end(); img++){
		if(make_image){
			if(sub_image){
				(*img).output_local_image();
			}
			(*img).output_global_image();
		}
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
