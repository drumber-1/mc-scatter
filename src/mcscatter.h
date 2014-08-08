#pragma once

#include <string>
#include <list>
#include "grid.h"
#include "image.h"
#include "problem.h"

#define NSAMPLES 10000000 //Number of photons across all threads

class MCScatter {
		std::list<Image> scatter_images;
		std::list<Image> colden_images;
		Grid grid;
		double albedo, opacity;
		std::string data_location;
		MCScatter();
		MCScatter(MCScatter const& copy);
		MCScatter& operator=(MCScatter const& copy);
	public:
		static MCScatter& get_instance();
		void do_scatter_simulation(int n_photons);
		void do_colden_calculation();
		void add_image(double theta, double phi, const GridParameters& gp, std::string type);
		std::string get_data_location() const;
		double get_albedo() const;
		double get_opacity() const;
};
