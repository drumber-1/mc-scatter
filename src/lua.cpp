#include <string>
#include <vector>

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include "lua.h"

lua_State* lua::open_file(const std::string& filename) {
	lua_State* ls = luaL_newstate();
	luaL_openlibs(ls);
	if (luaL_loadfile(ls, filename.c_str()) || lua_pcall(ls, 0, 0, 0)) {
		throw LuaException(filename + " is not a valid lua file");
	}
	return ls;
}

std::vector<double> lua::call_function(lua_State* ls, const std::string& name, const std::vector<double>& params, int n_out) {
	lua_getglobal(ls, name.c_str());
	if (!lua_isfunction(ls, -1)) {
		throw LuaException(name + " does not reference a valid number");
	}
	std::vector<double> x = {0,0,0};
	return x;
}

double lua::get_number(lua_State* ls, const std::string& name) {
	lua_getglobal(ls, name.c_str());
	if (!lua_isnumber(ls, -1)) {
		throw LuaException(name + " does not reference a valid number");
	}
	double x = lua_tonumber(ls, -1);
	lua_pop(ls, -1);
	return x;
}

std::string lua::get_string(lua_State* ls, const std::string& name) {
	lua_getglobal(ls, name.c_str());
	if (!lua_isstring(ls, -1)) {
		throw LuaException(name + " does not reference a valid string");
	}
	std::string str = lua_tostring(ls, -1);
	lua_pop(ls, -1);
	return str;
}

std::vector<double> lua::get_number_table(lua_State* ls, const std::string& name) {
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

std::vector<std::string> lua::get_string_table(lua_State* ls, const std::string& name) {
	lua_getglobal(ls, name.c_str());
	if (!lua_istable(ls, -1)) {
		throw LuaException(name + " does not reference a valid table");
	}
	lua_pushnil(ls);
	std::vector<std::string> out;
	while (lua_next(ls, -2)) {
		if (lua_isstring(ls, -1)) {
			out.push_back(lua_tostring(ls, -1));
		}
		lua_pop(ls, 1);
	}
	return out;
}

