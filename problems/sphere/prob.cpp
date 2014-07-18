#include <iostream>
#include <cmath>
#include <list>
#include "param.h"
#include "grid.h"
#include "image.h"
#include "para.h"
#include "photon.h"
#include "prob.h"

using namespace std;

void init_usr();
Grid generate_grid();
Photon generate_photon();

extern double toRad(double angle);

//TODO Move things you might want to change into here

void init_usr(){
	albedo = 1.0;
	opacity = 1.0;
	
	make_scatter_image = false;
	sub_scatter_image = false; //Whether to output image data from each processor
	make_colden_image = true;
	controlled_setup = false;
}

double setup_density(double x, double y, double z){
	double r2 = x*x + y*y + z*z;
	
	if(r2 < 1){
		return 5.0;
	} else {
		return 0.0001;
	}
}

Grid generate_grid(){

	GridParameters gp;
	for(int i = 0; i < 3; i++){
		gp.ncells[i] = 100;
		gp.left_boundary[i] = -2;
		gp.right_boundary[i] = 2;
	}
	
	Grid grid(gp);
	
	for(int i = 0; i < gp.ncells[0]; i++){
		for(int j = 0; j < gp.ncells[1]; j++){
			for(int k = 0; k < gp.ncells[2]; k++){
				std::vector<int> cell(3);
				cell[0] = i;
				cell[1] = j;
				cell[2] = k;
				
				std::vector<double> pos = grid.get_position(cell);
				double r2 = pos[0]*pos[0] + pos[1]*pos[1] + pos[2]*pos[2];
				if(r2 < 1){
					grid.set_rho(cell, 5.0);
				} else {
					grid.set_rho(cell, 0.0001);
				}
			}
		}
	}
	
	Image im (toRad(0.0), toRad(0.0), 200, gp.left_boundary[0], gp.right_boundary[0]);
	colden_images.push_back(im);
	
	return grid;
	
}

Photon generate_photon(){
	Photon p (0, 0, 0);
	return p;
}
