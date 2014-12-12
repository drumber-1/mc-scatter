#include <string>
#include <vector>

#include "config.h"
#include "lua.h"
#include "log.h"

void set_number(lua_State* ls, const std::string& name, double& number);
void set_string(lua_State* ls, const std::string& name, std::string& str);
template<typename T, size_t SIZE>
void set_number_table(lua_State* ls, const std::string& name, std::array<T, SIZE>& table);


Config::Config() {
	set_defaults();
}

Config::Config(const std::string& lua_file) {
	set_defaults();
	set_lua(lua_file);
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
		grid_limits.ncells[i] = 100;
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

void Config::set_lua(const std::string& lua_file) {
	lua_State* ls;
	
	try {
		ls = lua::open_file(lua_file);
	} catch (LuaException& e) {
		logs::err << e.what() << ", config unchanged\n";
		return;
	}

	set_string(ls, "data_location", data_location);
	set_string(ls, "colden_location", colden_location);
	set_string(ls, "scatter_location", scatter_location);
	
	set_number_table(ls, "grid_cells", grid_limits.ncells);
	set_number_table(ls, "grid_left", grid_limits.left_boundary);
	set_number_table(ls, "grid_right", grid_limits.right_boundary);
	
	set_number_table(ls, "image_pixels", image_size.npixels);
	set_number_table(ls, "image_left", image_size.left_boundary);
	set_number_table(ls, "image_right", image_size.right_boundary);
}

void set_number(lua_State* ls, const std::string& name, double& number) {
	try {
		number = lua::get_number(ls, name);
	} catch (LuaException& e) {
		logs::verbose << e.what() << ", variable not set\n";
		return;
	}
}

void set_string(lua_State* ls, const std::string& name, std::string& str) {
	try {
		str = lua::get_string(ls, name);
	} catch (LuaException& e) {
		logs::verbose << e.what() << ", variable not set\n";
		return;
	}
}

template<typename T, size_t SIZE>
void set_number_table(lua_State* ls, const std::string& name, std::array<T, SIZE>& table) {
	try {
		std::vector<double> values = lua::get_number_table(ls, name);
		if (values.size() != SIZE) {
			throw LuaException(name + " is not " + std::to_string(SIZE) + " elements long");
		}
		for (unsigned int i = 0; i < SIZE; i++) {
			table[i] = values[i];
		}
	} catch (LuaException& e) {
		logs::verbose << e.what() << ", variable not set\n";
		return;
	}
}	

