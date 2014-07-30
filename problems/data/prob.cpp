#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#include <stdlib.h>
#include "param.h"
#include "grid.h"
#include "image.h"
#include "para.h"
#include "photon.h"
#include "fileio_mg.h"

using namespace std;

void init_usr();
double setup_density(double x, double y, double z);
void setup_density();
Photon generate_photon();

extern double toRad(double angle);
extern double rand_double();
extern string int_to_string(int n, int width);
extern double string_to_double(const std::string& s);

void init_usr(){

	albedo = 1.0;
	opacity = 1.0;
	
	make_scatter_image = false;
	sub_scatter_image = false;
	make_colden_image = false;
	make_grid_slices = false;
	
}

Grid generate_grid(){
	
	GridParameters gp;
	for(int i = 0; i < 3; i++){
		gp.ncells[i] = 500;
		gp.left_boundary[i] = 0; //These are ignored currently
		gp.right_boundary[i] = 0;
	}
	
	std::string input_file = data_location + "/rd_0030";
	
	FileIOMG file_io;
	Grid grid = file_io.read_file(input_file, gp);
	
	if(!grid.is_good()){
		exit(EXIT_FAILURE);
	}
	
	for (int i = 0; i < 360; i += 5) {
		Image im (toRad(i), toRad(25.0), grid.get_parameters());
		colden_images.push_back(im);
	}
	
	return grid;
}

Photon generate_photon(){
	Photon p (0, 0, 0);
	return p;
}

