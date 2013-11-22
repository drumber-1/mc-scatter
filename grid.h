#ifdef GRID
int grid_ncells[3];
double ***rhoData;
double grid_left[3], grid_right[3], grid_space[3];
#else
extern int grid_ncells[3];
extern double ***rhoData;
extern double grid_left[3], grid_right[3], grid_space[3];
#endif
