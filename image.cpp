#define IMAGE

#include <iostream>
#include <fstream>
#include <cmath>
#include <mpi.h>
#include "image.h"
#include "grid.h"
#include "para.h"

#undef IMAGE

using namespace std;

extern string int_to_string(int n, int width);

Image::Image(double theta, double phi, int nx, int ny){
	init(theta, phi, nx, ny);
}

Image::Image(double theta, double phi, int nxy){
	init(theta, phi, nxy, nxy);
}

Image::~Image(){
	for(int i = 0; i < image_npixels[0]; i++){
		delete[] image[i];
	}
	
	delete[] image;
}

void Image::init(double theta, double phi, int nx, int ny){
	
	id = nimages;
	nimages++;
	
	image_npixels[0] = nx;
	image_npixels[1] = ny;
	
	obs_theta = theta;
	obs_phi = phi;
	
	for(int i = 0; i < 2; i++){
		//TODO scale image size for angled image planes
		// Possibly have a mode for fixed size for rotating images
		image_left[i] = grid_left[i];
		image_right[i] = grid_right[i];
		image_pixel_spacing[i] = (image_right[i] - image_left[i])/image_npixels[i];
	}
		
	image = new double *[image_npixels[0]];
	
	for(int i = 0; i < image_npixels[0]; i++){
		image[i] = new double [image_npixels[1]];
	}
	
	for(int i = 0; i < image_npixels[0]; i++){
		for(int j = 0; j < image_npixels[1]; j++){
			image[i][j] = 0.0;
		}
	}
	
}

string Image::construct_filename(bool global){
	string filename;
	if(global)
		filename = "./data/image_" + int_to_string(id, 4) + ".dat";
	else
		filename = "./data/image_" + int_to_string(id, 4) + "_p" + int_to_string(procRank, 4) + ".dat";
	
	return filename;
}

void Image::add(double x, double y, double z, double weight){

	double ximage = -1*x*sin(obs_theta) + y*cos(obs_theta);
	double yimage = -1*x*cos(obs_theta)*sin(obs_phi) - y*sin(obs_theta)*sin(obs_phi) + z*cos(obs_phi);

	int i = (ximage - image_left[0])/image_pixel_spacing[0];
	int j = (yimage - image_left[1])/image_pixel_spacing[1];
	
	//Check if inside of image
	if(i >= 0 && i < image_npixels[0] && j >= 0 && j < image_npixels[1]){
		image[i][j] += weight;
	}
}

//TODO Add header
void Image::output_global_image(){
		
	// Create global image
	double g_image[image_npixels[0]][image_npixels[1]];
		
	//Could be reduced to a single MPI_Reduce call to improve speed if needed
	for(int i = 0; i < image_npixels[0]; i++){
		MPI_Reduce(image[i], g_image[i], image_npixels[1], MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	}
	
	if(procRank == 0) {
		ofstream fout;
		string fname = construct_filename(true);
		fout.open(fname.c_str());
		if(fout.good()) {
			for(int i = 0; i < image_npixels[0]; i++){
				for(int j = 0; j < image_npixels[1]; j++){
					fout << i << "\t" << j << "\t" << g_image[i][j] << endl;
				}
			}
			fout.close();
		} else {
			cerr << "Could not open output file" << endl;
		}
	}
	
}

void Image::output_local_image(){
	ofstream fout;
	string fname = construct_filename(false);
			
	fout.open(fname.c_str());
	if(fout.good()) {
		for(int i = 0; i < image_npixels[0]; i++){
			for(int j = 0; j < image_npixels[1]; j++){
				fout << i << "\t" << j << "\t" << image[i][j] << endl;
			}				
		}
		fout.close();
	} else {
		cerr << "Could not open output file " << fname << endl;
	}		
}

double Image::get_theta(){
	return obs_theta;
}

double Image::get_phi(){
	return obs_phi;
}

double Image::get_left_bound(int dim){
	return image_left[dim];
}

double Image::get_right_bound(int dim){
	return image_right[dim];
}

double Image::get_spacing(int dim){
	return image_pixel_spacing[dim];
}

int Image::get_npixels(int dim){
	return image_npixels[dim];
}

void Image::print_info(){
	cout << "Image id: " << id << endl;
}


