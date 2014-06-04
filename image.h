#include <string>
#include <list>

class Image {
		double obs_theta, obs_phi; //Observer Direction
		int image_npixels[2], id;
		double image_left[2], image_right[2], image_pixel_spacing[2];
		double **image;
		void init(double, double, int, int);
		std::string construct_filename(bool);
	public:
		Image (double, double, int, int);
		Image (double, double, int);
		~Image ();
		void add(double, double, double, double);
		void output_global_image();
		void output_local_image();
		double get_theta();
		double get_phi();
		void print_info();
};

#ifdef IMAGE
bool make_scatter_image, sub_scatter_image;
int nimages = 0; //Number of images defined, used to give each image a unique id
std::list<Image> scatter_images;
#else
extern bool make_scatter_image, sub_scatter_image;
extern std::list<Image> scatter_images;
#endif


