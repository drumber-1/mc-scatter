#define NSAMPLES 10000000 //Number of photons across all threads
#ifdef MAIN
double albedo, opacity;
#else
extern double albedo, opacity;
#endif
