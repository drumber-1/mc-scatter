#pragma once

#include <string>
#include <list>
#include "grid.h"
#include "image.h"
#include "problem.h"

class MCScatter {
		std::list<Image> scatter_images;
		std::list<Image> colden_images;
		Grid grid;
		std::string data_location;
		std::string colden_location;
		std::string scatter_location;
		MCScatter();
		MCScatter(MCScatter const& copy);
		MCScatter& operator=(MCScatter const& copy);
	public:
		static MCScatter& get_instance();
		void read_config(std::string filename);
		void add_image(double theta, double phi, const GridParameters& gp, std::string type);
		void add_image(double theta, double phi, std::string type);
		std::string get_data_location() const;
		void set_data_location(const std::string& dl);
		void print_image_info();
		void print_misc_info() const;
		void do_scatter_simulation(int n_photons);
		void do_colden_calculation();
		void clear_grid();
		void clear_images();
		void set_grid(const Grid& new_grid);
		const Grid& get_grid() const;
		const std::list<Image>& get_scatter_images() const;
		const std::list<Image>& get_colden_images() const;
};
