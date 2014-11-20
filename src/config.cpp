#include <string>
#include <vector>

#include "config.h"
#include "lua.h"
#include "log.h"

Config::Config() {
	set_defaults();
}

Config::Config(const std::string& lua_file) {
	set_defaults();
	lua_State* ls;
	try {
		ls = lua::open_file(lua_file);
	} catch (LuaException& e) {
		logs::err << e.what() << ", using default values\n";
		return;
	}

	data_location = lua::get_string(ls, "data_location");
	colden_location = lua::get_string(ls, "colden_location");
	scatter_location = lua::get_string(ls, "scatter_location");
	
	std::vector<double> cells_vec = lua::get_table(ls, "grid_cells");
	std::vector<double> grid_left_vec = lua::get_table(ls, "grid_left");
	std::vector<double> grid_right_vec = lua::get_table(ls, "grid_right");
	
	std::vector<double> pixels_vec = lua::get_table(ls, "image_pixels");
	std::vector<double> image_left_vec = lua::get_table(ls, "image_left");
	std::vector<double> image_right_vec = lua::get_table(ls, "image_right");
	
	for (int i = 0; i < 3; i++) {
		grid_limits.ncells[i] = cells_vec[i];
		grid_limits.left_boundary[i] = grid_left_vec[i];
		grid_limits.right_boundary[i] = grid_right_vec[i];
	}
	
	for (int i = 0; i < 2; i++) {
		image_size.npixels[i] = pixels_vec[i];
		image_size.left_boundary[i] = image_left_vec[i];
		image_size.right_boundary[i] = image_right_vec[i];
	}
}

void Config::print() const {
	logs::out << "Directories:\n";
	logs::out << "\tData Dir:\t" << data_location << "\n";
	logs::out << "\tColden Dir:\t" << colden_location << "\n";
	logs::out << "\tScatter Dir:\t" << scatter_location << "\n";
	
	logs::out << "Grid limits:\n";
	logs::out << "\tCells:\t(" << grid_limits.ncells[0] << ", " << grid_limits.ncells[1] << ", " << grid_limits.ncells[2] << ")\n";
	logs::out << "\tLeft bound:\t(" << grid_limits.left_boundary[0] << ", " << grid_limits.left_boundary[1] << ", " << grid_limits.left_boundary[2] << ")\n";
	logs::out << "\tRight bound:\t(" << grid_limits.right_boundary[0] << ", " << grid_limits.right_boundary[1] << ", " << grid_limits.right_boundary[2] << ")\n";
	
	logs::out << "Image size:\n";
	logs::out << "\tPixels:\t(" << image_size.npixels[0] << ", " << image_size.npixels[1] << ")\n";
	logs::out << "\tLeft bound:\t(" << image_size.left_boundary[0] << ", " << image_size.left_boundary[1] << ")\n";
	logs::out << "\tRight bound:\t(" << image_size.right_boundary[0] << ", " << image_size.right_boundary[1] << ")\n";
}

void Config::set_defaults() {
	data_location = "./data";
	colden_location = "./data/colden";
	scatter_location = "./data/scatter";
	
	grid_limits = GridParameters();
	for (int i = 0; i < 3; i++) {
		grid_limits.ncells[i] = 500;
		grid_limits.left_boundary[i] = 0.0;
		grid_limits.right_boundary[i] = 0.0;
	}
	
	image_size = ImageParameters();
	for (int i = 0; i < 2; i++) {
		image_size.npixels[i] = 0;
		image_size.left_boundary[i] = 0.0;
		image_size.right_boundary[i] = 0.0;
	}
}


