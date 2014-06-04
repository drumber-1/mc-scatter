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
	
	make_scatter_image = true;
	sub_scatter_image = false; //Whether to output image data from each processor
	
	//Image im1 (toRad(0.0), toRad(0.0), 100);
	//images.push_back(im1);
	
	for(int i = 0; i < 60; i ++){
		Image im (toRad(0.0), toRad(i*3.0), 500);
		images.push_back(im);
	}
	
	/*for(list<Image>::iterator img = images.begin(); img != images.end(); img++){
		if(procRank == 0){
			(*img).print_info();
		}
	}*/
	
	text = Text("?", 256);
	text.generate_pixmap();
}

double setup_density(double x, double y, double z){
	double pix_size = 0.5*(grid_right[0] - grid_left[1])/text.width;
	
	int pix_val = text.get_pixel((y - grid_left[1]/2.0)/pix_size,(x - grid_left[1])/pix_size);
	double rho = 0.0001;
	
	if(abs(z) < 0.5){
		if(pix_val == 0){
			rho = 0.0001;
		} else {
			rho = pix_val/255.0;
		}
	}
	
	return rho;
}

Photon generate_photon(){
	Photon p (0, 0, 0);
	return p;
}
