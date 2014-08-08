#pragma once

#include <string>
#include <list>
#include "grid.h"

class Image {
		double obs_theta, obs_phi; //Observer Direction
		int image_npixels[2], id;
		double image_left[2], image_right[2], image_pixel_spacing[2];
		double **image;
		void init(double theta, double phi, const GridParameters& gp);
		std::string construct_filename(std::string dir, std::string prefix, bool global);
		static int nimages; //Number of images defined, used to give each image a unique id
		static std::list<Image> scatter_images;
		static std::list<Image> colden_images;
	public:
		Image (double theta, double phi, const GridParameters& gp);
		~Image ();
		Image(const Image& other);
		void add(double x, double y, double z, double weight);
		void calculate_column_density(const Grid& grid);
		void output_global_image(std::string dir, std::string prefix);
		void output_local_image(std::string dir, std::string prefix);
		double get_theta();
		double get_phi();
		double get_left_bound(int dim);
		double get_right_bound(int dim);
		double get_spacing(int dim);
		int get_npixels(int dim);
		void print_info();
		static void add_image(double theta, double phi, const GridParameters& gp, std::string type);
};
