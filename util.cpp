#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

double toRad(double angle);
std::string int_to_string(int x);
double string_to_double(const std::string& s);

double toRad(double angle){
	return angle*M_PI/180.0;
}

std::string int_to_string(int n, int width){
	std::stringstream sstm;
	sstm << std::setw(width) << std::setfill('0') << n;
	return sstm.str();
}

double string_to_double(const std::string& s){
	std::istringstream i(s);
	double x;
	if(!(i >> x)){
		return 0;
	}
	return x;
}
