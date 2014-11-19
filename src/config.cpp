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
	if(!lua::open_file(lua_file)) {
		logs::err << "Could not file lua file: " << lua_file << ", using default values\n";
		return;
	}

	data_location = lua::get_string("data_location");
	colden_location = lua::get_string("colden_location");
	scatter_location = lua::get_string("scatter_location");
	
	std::vector<double> res_vec = lua::get_table("image_resolution");
	std::vector<double> cells_vec = lua::get_table("max_cells");
	std::vector<double> left_vec = lua::get_table("left_bound");
	std::vector<double> right_vec = lua::get_table("right_bound");
	
	image_res[0] = res_vec[0];
	image_res[1] = res_vec[1];
	
	for (int i = 0; i < 3; i++) {
		max_cells[i] = cells_vec[i];
		left_bound[i] = left_vec[i];
		right_bound[i] = right_vec[i];
	}
}

void Config::print() const {
	logs::out << "Current config:\n";
	logs::out << "Data Dir:\t" << data_location << "\n";
	logs::out << "Colden Dir:\t" << colden_location << "\n";
	logs::out << "Scatter Dir:\t" << scatter_location << "\n";
	
	logs::out << "Image res:\t(" << image_res[0] << ", " << image_res[1] << ")\n";
	
	logs::out << "Max Cells:\t(" << max_cells[0] << ", " << max_cells[1] << ", " << max_cells[2] << ")\n";
	
	logs::out << "Left bound:\t(" << left_bound[0] << ", " << left_bound[1] << ", " << left_bound[2] << ")\n";
	logs::out << "Right bound:\t(" << right_bound[0] << ", " << right_bound[1] << ", " << right_bound[2] << ")\n";
}

void Config::set_defaults() {
	data_location = "./data";
	colden_location = "./data/colden";
	scatter_location = "./data/scatter";
	
	image_res[0] = 0;
	image_res[1] = 0;
	
	for (int i = 0; i < 3; i++) {
		max_cells[i] = 500;
		left_bound[i] = -1.0e99;
		right_bound[i] = 1.0e99;
	}
}


