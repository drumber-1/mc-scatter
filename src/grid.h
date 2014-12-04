#pragma once

#include <string>
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
		double ***rho_data;
		double spacing[3];
		bool empty;
		double albedo, opacity;
		void init(const Grid& other);
	public:
		Grid(const GridParameters&);
		Grid();
		~Grid();
		Grid(const Grid&);
		Grid& operator=(const Grid& other);
		
		void clear();
		void output_slices(std::string, unsigned int) const;
		void print_info() const;
		
		bool is_on_grid(const Position&) const;
		bool is_on_grid(const Point&) const;
		Position get_position(const Point&) const;
		Point get_cell(const Position&) const;
		double get_rho(const Position&) const;
		double get_rho(const Point&) const;
		void set_rho(const Point&, double rho);
		
		double get_spacing(int dim) const;
		bool is_empty() const;
		GridParameters get_parameters() const;
		double get_albedo() const;
		void set_albedo(double alb);
		double get_opacity() const;
		void set_opacity(double opac);
};

