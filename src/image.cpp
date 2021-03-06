#include <iostream>
#include <fstream>
#include <cmath>

#include "image.h"
#include "grid.h"
#include "para.h"
#include "photon.h"
#include "util.h"
#include "log.h"

int Image::nimages = 0;

Image::Image(double theta, double phi, const ImageParameters& ip) {
	init(theta, phi, ip);
}

Image::~Image() {
	for (int i = 0; i < parameters.npixels[0]; i++) {
		delete[] image[i];
	}

	delete[] image;
}

Image::Image(const Image& other) {
	id = other.id;
	
	obs_theta = other.obs_theta;
	obs_phi = other.obs_phi;
	
	parameters = other.parameters;
	
	for (int i = 0; i < 2; i++) {
		image_pixel_spacing[i] = other.image_pixel_spacing[i];
	}
		
	image = new double *[parameters.npixels[0]];
	
	for (int i = 0; i < parameters.npixels[0]; i++) {
		image[i] = new double [parameters.npixels[1]];
	}
	
	for (int i = 0; i < parameters.npixels[0]; i++) {
		for (int j = 0; j < parameters.npixels[1]; j++) {
			image[i][j] = other.image[i][j];
		}
	}
}

void Image::init(double theta, double phi, const ImageParameters& ip) {
	
	id = nimages;
	nimages++;
	
	obs_theta = theta;
	obs_phi = phi;
	
	parameters = ip;
	
	for (int i = 0; i < 2; i++) {
		image_pixel_spacing[i] = (parameters.right_boundary[i] - parameters.left_boundary[i])/parameters.npixels[i];
	}
		
	image = new double *[parameters.npixels[0]];
	
	for (int i = 0; i < parameters.npixels[0]; i++) {
		image[i] = new double [parameters.npixels[1]];
	}
	
	for (int i = 0; i < parameters.npixels[0]; i++) {
		for (int j = 0; j < parameters.npixels[1]; j++) {
			image[i][j] = 0.0;
		}
	}
	
}

std::string Image::construct_filename(std::string dir, std::string prefix, bool global) {
	std::string filename;
	if (global) {
		filename = dir + "/" + prefix + "_" + util::int_to_string(id, 4) + ".dat";
	} else {
		filename = dir + "/" + prefix + "_" +  util::int_to_string(id, 4) + "_p" +  util::int_to_string(para::get_process_rank(), 4) + ".dat";
	}
	
	return filename;
}

void Image::add(double x, double y, double z, double weight) {

	double ximage = -1*x*sin(obs_theta) + y*cos(obs_theta);
	double yimage = -1*x*cos(obs_theta)*sin(obs_phi) - y*sin(obs_theta)*sin(obs_phi) + z*cos(obs_phi);

	int i = round((ximage - parameters.left_boundary[0]) / image_pixel_spacing[0]);
	int j = round((yimage - parameters.left_boundary[1]) / image_pixel_spacing[1]);
	
	//Check if inside of image
	if (i >= 0 && i < parameters.npixels[0] && j >= 0 && j < parameters.npixels[1]) {
		image[i][j] += weight;
	}
}

void Image::calculate_column_density(const Grid& grid) {

	int bounds[para::get_process_size()+1];
	int per_proc = parameters.npixels[0] / (para::get_process_size());
	int remainder = parameters.npixels[0] % (para::get_process_size());
	for (int i = 0; i <= para::get_process_size(); i++) {
		bounds[i] = i * per_proc;
		if (i < remainder) {
			bounds[i] += i;
		} else {
			bounds[i] += remainder;
		}
	}

	for (int i = bounds[para::get_process_rank()]; i < bounds[para::get_process_rank()+1]; i++) {
		for (int j = 0; j < parameters.npixels[1]; j++) {
			double ximage = parameters.left_boundary[0] + (i * image_pixel_spacing[0]);
			double yimage = parameters.left_boundary[1] + (j * image_pixel_spacing[1]);
				
			//Instead of trying to find the edge of the grid, for each point
			//we will fire two photons, one in each direction from the centre
			
			double x = (-1 * ximage * sin(obs_theta)) - (yimage * cos(obs_theta) * sin(obs_phi));
			double y = (ximage * cos(obs_theta)) - (yimage * sin(obs_theta) * sin(obs_phi));
			double z = (yimage * cos(obs_phi));
				
			Photon p1 (x, y, z, obs_theta, obs_phi, true);
			while (!p1.escaped) {
				p1.update(grid);
			}
			double colden = p1.get_tau_cur();
				
			Photon p2 (x, y, z, obs_theta + M_PI, -1 * obs_phi, true);
			while (!p2.escaped) {
				p2.update(grid);
			}
			colden += p2.get_tau_cur();
			image[i][j] += colden;
		}				
	}
}

//TODO Add header
void Image::output_global_image(std::string dir, std::string prefix) {
		
	// Create global image
	double g_image[parameters.npixels[0]][parameters.npixels[1]];
		
	//Could be reduced to a single MPI_Reduce call to improve speed if needed
	for (int i = 0; i < parameters.npixels[0]; i++) {
		para::global_sum(image[i], g_image[i], parameters.npixels[1]);
	}
	
	if (para::get_process_rank() == 0) {
		std::ofstream fout;
		std::string fname = construct_filename(dir, prefix, true);
		fout.open(fname.c_str());
		if (fout.good()) {
			for (int j = 0; j < parameters.npixels[1]; j++) {
				for (int i = 0; i < parameters.npixels[0]; i++) {
					fout << i << "\t" << j << "\t" << g_image[i][j] << "\n";
				}
			}
			fout.close();
		} else {
			logs::err << "Could not open output file: " << fname << "\n";
		}
	}
	
}

//x axes is inner loop so a 1D array of pixel values can be reshaped more easily
void Image::output_local_image(std::string dir, std::string prefix) {
	std::ofstream fout;
	std::string fname = construct_filename(dir, prefix, false);
			
	fout.open(fname.c_str());
	if (fout.good()) {
		for (int j = 0; j < parameters.npixels[1]; j++) {
			for (int i = 0; i < parameters.npixels[0]; i++) {
				fout << i << "\t" << j << "\t" << image[i][j] << "\n";
			}				
		}
		fout.close();
	} else {
		logs::err << "Could not open output file " << fname << "\n";
	}		
}

double Image::get_theta() {
	return obs_theta;
}

double Image::get_phi() {
	return obs_phi;
}

double Image::get_left_bound(int dim) {
	return parameters.left_boundary[dim];
}

double Image::get_right_bound(int dim) {
	return parameters.right_boundary[dim];
}

double Image::get_spacing(int dim) {
	return image_pixel_spacing[dim];
}

int Image::get_npixels(int dim) {
	return parameters.npixels[dim];
}

void Image::print_info() {
	logs::out << "Image id: " << id << "\n";
	logs::out << "\tTheta: " << obs_theta << "\n";
	logs::out << "\tPhi: " << obs_phi << "\n";
}

