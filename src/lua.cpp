#include <string>
#include <vector>

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include "lua.h"
#include "log.h"
#include "photon.h"

double get_grid_density(const std::vector<double> pos);

lua_State *ls;

void lua::init() {
	ls = luaL_newstate();
	luaL_openlibs(ls);
}

bool lua::open_file(std::string filename) {
	if (luaL_loadfile(ls, filename.c_str()) || lua_pcall(ls, 0, 0, 0)) {
		return false;
	} else {
		return true;
	}
}

Photon lua::generate_photon() {

	lua_getglobal(ls, "generate_photon");
	
	if (lua_pcall(ls, 0, 3, 0) != 0) {
		logs::err << "generate_photon could not be called" << lua_tostring(ls, -1) << "\n";
		Photon p (0, 0, 0); //TODO Inform caller of error somehow
		return p;
	}
	
	for (int i = 0; i < 3; i++) {
		if (!lua_isnumber(ls, -1*i)) {
			logs::err << "get_grid_density failed to return a number\n";
			Photon p (0, 0, 0);
			return p;
		}
	}
	
	std::vector<double> pos(3);
	for (int i = 0; i < 3; i++) {
		pos[i] = lua_tonumber(ls, -1*i);
	}
	lua_pop(ls, 3);
	 
	Photon p (pos[0], pos[1], pos[2]);
	return p;
}

Grid lua::generate_grid() {
	if (!lua::is_function("generate_photon")) {
		logs::err << "No grid generation function has been defined\n";
		Grid grid;
		return grid;
	}
	
	std::vector<double> ncells = get_table("ncells");
	std::vector<double> lbound = get_table("left_boundary");
	std::vector<double> rbound = get_table("right_boundary");
	
	GridParameters gp;
	for (int i = 0; i < 3; i++) {
		gp.ncells[i] = ncells[i];
		gp.left_boundary[i] = lbound[i];
		gp.right_boundary[i] = rbound[i];
	}
	
	Grid grid(gp);
	
	for (int i = 0; i < gp.ncells[0]; i++) {
		for (int j = 0; j < gp.ncells[1]; j++) {
			for (int k = 0; k < gp.ncells[2]; k++) {
				std::vector<int> cell(3);
				cell[0] = i;
				cell[1] = j;
				cell[2] = k;
				
				std::vector<double> pos = grid.get_position(cell);
				grid.set_rho(cell, get_grid_density(pos));
			}
		}
	}
	
	return grid;
}

double get_grid_density(const std::vector<double> pos) {
	lua_getglobal(ls, "get_grid_density");
	for (auto x : pos) {
		lua_pushnumber(ls, x);
	}
	
	if (lua_pcall(ls, 3, 1, 0) != 0) {
		logs::err << "get_grid_density could not be called" << lua_tostring(ls, -1) << "\n";
		return -1;
	}
	
	if (!lua_isnumber(ls, -1)) {
		logs::err << "get_grid_density failed to return a number\n";
		return -1;
	}
	
	double rho = lua_tonumber(ls, -1);
	lua_pop(ls, 1);
	return rho;
}

bool lua::is_function(std::string name) {
	lua_getglobal(ls, name.c_str());
	bool ret = lua_isfunction(ls, -1);
	lua_pop(ls, -1);
	return ret;
}

bool lua::is_number(std::string name) {
	lua_getglobal(ls, name.c_str());
	bool ret = lua_isnumber(ls, -1);
	lua_pop(ls, -1);
	return ret;
}

double lua::get_number(std::string name) {
	lua_getglobal(ls, name.c_str());
	double x = lua_tonumber(ls, -1);
	lua_pop(ls, -1);
	return x;
}

bool lua::is_string(std::string name) {
	lua_getglobal(ls, name.c_str());
	bool ret = lua_isstring(ls, -1);
	lua_pop(ls, -1);
	return ret;
}

std::string lua::get_string(std::string name) {
	lua_getglobal(ls, name.c_str());
	std::string str = lua_tostring(ls, -1);
	lua_pop(ls, -1);
	return str;
}

bool lua::is_table(std::string name) {
	lua_getglobal(ls, name.c_str());
	bool ret = lua_istable(ls, -1);
	lua_pop(ls, -1);
	return ret;
}

std::vector<double> lua::get_table(std::string name) {
	lua_getglobal(ls, name.c_str());
	lua_pushnil(ls);
	std::vector<double> out;
	while (lua_next(ls, -2)) {
		if (lua_isnumber(ls, -1)) {
			out.push_back(lua_tonumber(ls, -1));
		}
		lua_pop(ls, 1);
	}
	return out;
}

