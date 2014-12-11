#include <string>
#include <vector>

#include "fileio_lua.h"
#include "lua.h"
#include "grid.h"
#include "log.h"
#include "util.h"

//Provides a way to read analytical density functions, specified in lua files
//A function "get_density" must exist in the specified lua file and behave as
//if it has the signature: double get_density(double x, double y, double z)

//TODO profile and improve speed
Grid FileIOLua::read_file(std::string filename, const GridParameters& grid_parameters) {

	lua_State* ls;
	ls = lua::open_file(filename);
	
	Grid grid(grid_parameters);
	
	for (int i = 0; i < grid_parameters.ncells[0]; i++) {
		for (int j = 0; j < grid_parameters.ncells[1]; j++) {
			for (int k = 0; k < grid_parameters.ncells[2]; k++) {
				Point cell = {{i, j, k}};
				Position pos = grid.get_position(cell);
				
				lua_getglobal(ls, "get_density");
				lua_pushnumber(ls, pos[0]);
				lua_pushnumber(ls, pos[1]);
				lua_pushnumber(ls, pos[2]);
				lua_pcall(ls, 3, 1, 0);
	
				double rho = lua_tonumber(ls, -1);
				lua_pop(ls, 1);
				
				grid.set_rho(cell, rho);
			}
		}
	}

	return grid;
}

void FileIOLua::write_file(std::string filename, const Grid& grid) {

	logs::err << "Cannot write to lua files\n";
	
	return;
}
