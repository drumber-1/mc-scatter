#pragma once

#include <string>
#include <vector>
#include <array>

using Point = std::array<int, 3>;
using Position = std::array<double, 3>;

class GridParameters {
	public:
		int ncells[3];
		double left_boundary[3];
		double right_boundary[3];
};

class Grid {
		GridParameters parameters;
		std::vector<double> rho_data;
		double spacing[3];
		bool empty;
		double albedo, opacity;
		inline int get_index(int i, int j, int k) const;
		inline int get_index(const Point& cell) const;
	public:
		Grid(const GridParameters&);
		Grid();
		
		void clear();
		void output_slices(std::string, unsigned int) const;
		void print_info() const;
		
		bool is_on_grid(const Position& pos) const;
		bool is_on_grid(const Point& cell) const;
		Position get_position(const Point& cell) const;
		Point get_cell(const Position& pos) const;
		double get_rho(const Position& pos) const;
		double get_rho(const Point& cell) const;
		void set_rho(const Point& cell, double rho);
		
		double get_spacing(int dim) const;
		bool is_empty() const;
		GridParameters get_parameters() const;
		double get_albedo() const;
		void set_albedo(double alb);
		double get_opacity() const;
		void set_opacity(double opac);
};

