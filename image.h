#include <string>
#include <list>
#include "grid.h"

class Image {
		double obs_theta, obs_phi; //Observer Direction
		int image_npixels[2], id;
		double image_left[2], image_right[2], image_pixel_spacing[2];
		double **image;
		void init(double, double, int, int, double, double);
		std::string construct_filename(bool, std::string);
	public:
		Image (double, double, int, int, double, double);
		Image (double, double, int, double, double);
		~Image ();
		Image(const Image&);
		void add(double, double, double, double);
		void calculate_column_density(const Grid&);
		void output_global_image(std::string);
		void output_local_image(std::string);
		double get_theta();
		double get_phi();
		double get_left_bound(int);
		double get_right_bound(int);
		double get_spacing(int);
		int get_npixels(int);
		void print_info();
};

#ifdef IMAGE
bool make_scatter_image, sub_scatter_image;
bool make_colden_image, make_grid_slices;
int nimages = 0; //Number of images defined, used to give each image a unique id
std::list<Image> scatter_images;
std::list<Image> colden_images;
#else
extern bool make_scatter_image, sub_scatter_image;
extern bool make_colden_image, make_grid_slices;
extern std::list<Image> scatter_images;
extern std::list<Image> colden_images;
#endif


