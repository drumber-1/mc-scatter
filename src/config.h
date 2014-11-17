#ifndef CONFIG
#define CONFIG

#include <string>

class Config {
		void set_defaults();
	public:
		Config();
		Config(const std::string& lua_file);
		
		std::string data_location, colden_location, scatter_location;
		int image_res[2];
		int max_cells[3];
		double left_bound[3], right_bound[3];
};

#endif
