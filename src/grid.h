#pragma once

#include <string>
#include <vector>

class GridParameters {
	public:
		int ncells[3];
		double left_boundary[3];
		double right_boundary[3];
};

class Grid {
		GridParameters parameters;
		double ***rho_data;
		double spacing[3];
		bool empty;
		double albedo, opacity;
	public:
		Grid(const GridParameters&);
		Grid();
		~Grid();
		Grid(const Grid&);
		void clear();
		void output_slices(std::string, unsigned int) const;
		void print_info() const;
		bool is_on_grid(const std::vector<double>&) const;
		bool is_on_grid(const std::vector<int>&) const;
		std::vector<double> get_position(const std::vector<int>&) const;
		std::vector<int> get_cell(const std::vector<double>&) const;
		double get_rho(const std::vector<double>&) const;
		double get_rho(const std::vector<int>&) const;
		void set_rho(const std::vector<int>&, double rho);
		double get_spacing(int dim) const;
		bool is_empty() const;
		GridParameters get_parameters() const;
		double get_albedo() const;
		void set_albedo(double alb);
		double get_opacity() const;
		void set_opacity(double opac);
};

