#ifndef LUA
#define LUA

#include <string>
#include <vector>
#include <initializer_list>
#include <stdexcept>

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

class LuaException : public std::runtime_error {
	public:
		LuaException(const std::string& message) : std::runtime_error(message) {};
};

namespace lua {
	lua_State* open_file(const std::string& filename);
	double get_number(lua_State* ls, const std::string& name);
	std::string get_string(lua_State* ls, const std::string& name);
	std::vector<double> get_number_table(lua_State* ls, const std::string& name);
	std::vector<std::string> get_string_table(lua_State* ls, const std::string& name);
}

#endif
