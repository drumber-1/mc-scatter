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
		const bool good; //This set false when a grid is expected to be returned, but a sensible grid cannot be created
	public:
		Grid(const GridParameters&);
		Grid(bool);
		~Grid();
		Grid(const Grid&);
		void output_slices(std::string, unsigned int) const;
		bool is_on_grid(const std::vector<double>&) const;
		bool is_on_grid(const std::vector<int>&) const;
		std::vector<double> get_position(const std::vector<int>&) const;
		std::vector<int> get_cell(const std::vector<double>&) const;
		double get_rho(const std::vector<double>&) const;
		double get_rho(const std::vector<int>&) const;
		void set_rho(const std::vector<int>&, double rho);
		double get_spacing(int dim) const;
		bool is_good() const;
		GridParameters get_parameters() const;
};

