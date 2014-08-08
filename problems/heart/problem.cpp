#include <vector>
#include <cmath>
#include "problem.h"
#include "grid.h"
#include "para.h"
#include "photon.h"
#include "random.h"

bool in_heart(std::vector<double> pos);

void problem::init(){
}

Grid problem::generate_grid(){

	GridParameters gp;
	for(int i = 0; i < 3; i++){
		gp.ncells[i] = 100;
		gp.left_boundary[i] = -2;
		gp.right_boundary[i] = 2;
	}
	
	Grid grid(gp);
	
	for(int i = 0; i < gp.ncells[0]; i++){
		for(int j = 0; j < gp.ncells[1]; j++){
			for(int k = 0; k < gp.ncells[2]; k++){
				std::vector<int> cell(3);
				cell[0] = i;
				cell[1] = j;
				cell[2] = k;
				
				std::vector<double> pos = grid.get_position(cell);
				if(in_heart(pos)){
					grid.set_rho(cell, 5.0);
				} else {
					grid.set_rho(cell, 0.0001);
				}
			}
		}
	}
	
	return grid;
	
}

Photon problem::generate_photon(){
	std::vector<double> pos(3);
	do {
		for(int i = 0; i < 3; i++){
			pos[i] = random_gen::rand_double()*2 - 1;
		}
	} while(!in_heart(pos));
	 
	Photon p (pos[0], pos[1], pos[2]);
	return p;
}

bool in_heart(std::vector<double> pos){
	double param = pow(pos[1]*pos[1] + (9.0/4.0)*pos[0]*pos[0] + pos[2]*pos[2] - 1,3)
	             + pos[1]*pos[1]*pos[2]*pos[2]*pos[2]
	             + (9.0/80.0)*pos[0]*pos[0]*pos[2]*pos[2]*pos[2];
	if(param < 0){
		return true;
	} else {
		return false;
	}
}
