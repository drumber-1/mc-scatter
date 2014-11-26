#ifndef CONFIG
#define CONFIG

#include <string>

#include "grid.h"
#include "image.h"

class Config {
		void set_defaults();
	public:
		Config();
		Config(const std::string& lua_file);
		
		void print() const;
		void set_lua(const std::string& lua_file);
		
		std::string data_location, colden_location, scatter_location;
		GridParameters grid_limits;
		ImageParameters image_size;
};

#endif
