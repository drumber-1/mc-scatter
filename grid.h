#pragma once

#include <string>
#include <vector>

class GridParameters {
	public:
		int ncells[3];
		double left_boundary[3];
		double right_boundary[3];
};

//TODO: Add traversal/iterator methods
class Grid {
		GridParameters parameters;
		double ***rho_data;
		double spacing[3];
	public:
		Grid(const GridParameters&);
		~Grid();
		Grid(const Grid&);
		void output_slices(std::string, unsigned int) const;
		bool is_on_grid(std::vector<double> pos) const;
		std::vector<double> get_position(std::vector<int> cell) const;
		std::vector<int> get_cell(std::vector<double> pos) const;
		double get_rho(std::vector<double> pos) const;
		double get_rho(std::vector<int> cell) const;
		void set_rho(std::vector<int> cell, double rho);
};
