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
bool inHeart(double x, double y, double z);

extern double toRad(double angle);
extern double rand_double();

//TODO Move things you might want to change into here

void init_usr(){
	albedo = 1.0;
	opacity = 1.0;
	
	for(int i = 0; i < 3; i++){
		grid_ncells[i] = 500;
		grid_left[i] = -2;
		grid_right[i] = 2;
	}
	
	make_scatter_image = true;
	sub_scatter_image = false; //Whether to output image data from each processor
	
	for(int i = 0; i < 60; i ++){
		Image im (toRad(90.0), toRad(i*6.0), 500);
		scatter_images.push_back(im);
	}
	
}

double setup_density(double x, double y, double z){
	if(inHeart(x, y, z)){
		return 1.0;
	} else {
		return 0.0001;
	}
}

Photon generate_photon(){
	double pos[3];
	do {
		for(int i = 0; i < 3; i++){
			pos[i] = rand_double()*2 - 1;
		}
		//cout << pos[0] << ", " << pos[1] << ", " << pos[2] << endl;
		//cout << inHeart(pos[0], pos[1], pos[2]) << endl;
	} while(!inHeart(pos[0], pos[1], pos[2]));
	 
	Photon p (pos[0], pos[1], pos[2]);
	return p;
}

bool inHeart(double x, double y, double z){
	double param = pow(y*y + (9.0/4.0)*x*x + z*z - 1,3) + y*y*z*z*z + (9.0/80.0)*x*x*z*z*z;
	if(param < 0){
		return true;
	} else {
		return false;
	}
}
