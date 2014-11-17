#include <string>
#include <vector>

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

bool lua::open_file(const std::string& filename) {
	if (luaL_loadfile(ls, filename.c_str()) || lua_pcall(ls, 0, 0, 0)) {
		return false;
	} else {
		return true;
	}
}

std::vector<double> lua::call_function(const std::string& name, const std::vector<double>& params, int n_out) {
	lua_getglobal(ls, name.c_str());
	if (!lua_isfunction(ls, -1)) {
		throw LuaException(name + " does not reference a valid number");
	}
	std::vector<double> x = {0,0,0};
	return x;
}

double lua::get_number(const std::string& name) {
	lua_getglobal(ls, name.c_str());
	if (!lua_isnumber(ls, -1)) {
		throw LuaException(name + " does not reference a valid number");
	}
	double x = lua_tonumber(ls, -1);
	lua_pop(ls, -1);
	return x;
}

std::string lua::get_string(const std::string& name) {
	lua_getglobal(ls, name.c_str());
	if (!lua_isstring(ls, -1)) {
		throw LuaException(name + " does not reference a valid string");
	}
	std::string str = lua_tostring(ls, -1);
	lua_pop(ls, -1);
	return str;
}

std::vector<double> lua::get_table(const std::string& name) {
	lua_getglobal(ls, name.c_str());
	if (!lua_istable(ls, -1)) {
		throw LuaException(name + " does not reference a valid table");
	}
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

