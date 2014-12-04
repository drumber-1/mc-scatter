#ifndef MCSCATTER
#define MCSCATTER

#include <string>
#include <list>
#include "grid.h"
#include "image.h"
#include "config.h"


namespace MCScatter {
		void init();
		void do_scatter_simulation(int n_photons);
		void do_colden_calculation();
		void do_slices();
		void print_image_info();
		void print_config_info();
		void print_grid_info();
		void add_image(double theta, double phi, const std::string& type);
		void clear_images();
		bool read_grid(const std::string& filetype, const std::string& filename, bool in_data_location);
		bool write_grid(const std::string& filetype, const std::string& filename, bool in_data_location);
		void clear_grid();
		bool read_config(const std::string& filename);
}

#endif
