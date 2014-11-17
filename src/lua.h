#ifndef LUA
#define LUA

#include <string>
#include <vector>
#include <stdexcept>

class LuaException : public std::runtime_error {
	public:
		LuaException(const std::string& message) : std::runtime_error(message) {};
};

namespace lua {
	void init();
	bool open_file(const std::string& filename);
	std::vector<double> call_function(const std::string& name, const std::vector<double>& params, int n_out);
	double get_number(const std::string& name);
	std::string get_string(const std::string& name);
	std::vector<double> get_table(const std::string& name);
}

#endif
