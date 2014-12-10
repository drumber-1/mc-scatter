#ifndef IMAGE
#define IMAGE

#include <string>
#include <array>
#include "grid.h"

class ImageParameters {
	public:
		std::array<int, 2> npixels;
		std::array<double, 2> left_boundary;
		std::array<double, 2> right_boundary;
};

class Image {
		double obs_theta, obs_phi; //Observer Direction
		int id;
		ImageParameters parameters;
		std::array<double, 2> image_pixel_spacing;
		double **image;
		void init(double theta, double phi, const ImageParameters& gp);
		std::string construct_filename(std::string dir, std::string prefix, bool global);
		static int nimages; //Number of images defined, used to give each image a unique id
	public:
		Image(double theta, double phi, const ImageParameters& gp);
		~Image();
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
};

#endif
