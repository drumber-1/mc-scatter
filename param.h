#include <string>

#define NSAMPLES 10000000 //Number of photons across all threads

#ifdef MAIN
double albedo, opacity;
std::string data_location;
#else
extern double albedo, opacity;
extern std::string data_location;
#endif
