#include <string>

namespace lua {
	void init();
	bool open_file(std::string filename);
	double get_grid_density(double x, double y, double z);
}
