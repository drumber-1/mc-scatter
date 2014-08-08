#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#include <stdlib.h>
#include "grid.h"
#include "photon.h"
#include "fileio_mg.h"
#include "problem.h"

void problem::init(){
}

Grid problem::generate_grid(){
	Grid grid;
	return grid;
}

Photon problem::generate_photon(){
	Photon p (0, 0, 0);
	return p;
}

