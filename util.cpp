#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

double toRad(double angle);
string int_to_string(int x);

double toRad(double angle){
	return angle*M_PI/180.0;
}

string int_to_string(int n, int width){
	stringstream sstm;
	sstm << setw(width) << setfill('0') << n;
	return sstm.str();
}
