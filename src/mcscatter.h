#ifndef MCSCATTER
#define MCSCATTER

#include <string>
#include <list>
#include "grid.h"
#include "image.h"
#include "problem.h"
#include "config.h"


namespace MCScatter {
		void init();
		void add_image(double theta, double phi, const GridParameters& gp, const std::string& type);
		void add_image(double theta, double phi, const std::string& type);
		std::string get_data_location();
		void print_image_info();
		void print_config_info();
		void print_grid_info();
		void do_scatter_simulation(int n_photons);
		void do_colden_calculation();
		void do_slices();
		void clear_grid();
		void clear_images();
		bool read_grid(const std::string& filetype, const std::string& filename);
		bool write_grid(const std::string& filetype, const std::string& filename);
}

#endif
