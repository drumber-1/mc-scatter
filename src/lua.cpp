#include <string>

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include "lua.h"
#include "log.h"

lua_State *ls;

void lua::init() {
	ls = luaL_newstate();
	luaL_openlibs(ls);
}

bool lua::open_file(std::string filename) {
	if (luaL_loadfile(ls, filename.c_str()) || lua_pcall(ls, 0, 0, 0)) {
		logs::err << "Error loading lua script, " << lua_tostring(ls, -1) << "\n";
		return false;
	} else {
		return true;
	}
}

double lua::get_grid_density(double x, double y, double z) {
	lua_getglobal(ls, "get_grid_density");
	lua_pushnumber(ls, x);
	lua_pushnumber(ls, y);
	lua_pushnumber(ls, z);
	
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
