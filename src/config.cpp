#include <string>

#include "config.h"

Config::Config() {
	set_defaults();
}

Config::Config(const std::string& lua_file) {
	set_defaults();
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
