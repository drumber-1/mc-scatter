#include <string>
#include <vector>

#include "photon.h"

namespace lua {
	void init();
	bool open_file(std::string filename);
	Photon generate_photon();
	Grid generate_grid();
	bool is_function(std::string name);
	bool is_number(std::string name);
	double get_number(std::string name);
	bool is_string(std::string name);
	std::string get_string(std::string name);
	bool is_table(std::string name);
	std::vector<double> get_table(std::string name);
}
