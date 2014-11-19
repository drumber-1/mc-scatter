#ifndef IMAGE
#define IMAGE

#include <string>
#include <list>
#include "grid.h"

class ImageParameters {
	public:
		int npixels[2];
		double left_boundary[2];
		double right_boundary[2];
};

class Image {
		double obs_theta, obs_phi; //Observer Direction
		int id;
		ImageParameters parameters;
		double image_pixel_spacing[2];
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
