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
double setup_density(double x, double y, double z);
Photon generate_photon();

extern double toRad(double angle);

//TODO Move things you might want to change into here

void init_usr(){
	albedo = 1.0;
	opacity = 1.0;
	
	for(int i = 0; i < 3; i++){
		grid_ncells[i] = 100;
		grid_left[i] = -2;
		grid_right[i] = 2;
	}
	
	make_scatter_image = false;
	sub_scatter_image = false; //Whether to output image data from each processor
	make_colden_image = true;
	
	Image im (toRad(0.0), toRad(0.0), 200);
	scatter_images.push_back(im);
	colden_images.push_back(im);

}

double setup_density(double x, double y, double z){
	double r2 = x*x + y*y + z*z;
	
	if(r2 < 1){
		return 5.0;
	} else {
		return 0.0001;
	}
}

Photon generate_photon(){
	Photon p (0, 0, 0);
	return p;
}
