#include <cmath>
#include <iostream>
#include <iomanip>
#include "photon.h"
#include "param.h"
#include "grid.h"

extern double rand_double();
extern double rand_tau();
extern double rand_phi();
extern double rand_theta();

extern double get_rho(double x, double y, double z);

extern void dispose();

Photon::Photon(double x, double y, double z){
	double theta = rand_theta();
	double phi = rand_phi();
	
	init(x, y, z, theta, phi, false);
}

Photon::Photon(double x, double y, double z, double ang1, double ang2, bool p){
	init(x, y, z, ang1, ang2, p);
}

void Photon::init(double x, double y, double z, double ang1, double ang2, bool p){
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
	nScatt = 0;
	if(p){
		tau_target = 1.0e99;
	} else {
		tau_target = rand_tau();
	}
	tau_cur = 0.0;
	absorbed = false;
	escaped = false;
	is_peel = p;
	
	dir[0] = sin(ang1)*cos(ang2);
	dir[1] = sin(ang1)*sin(ang2);
	dir[2] = cos(ang1);

}

void Photon::scatter(){
	tau_target = rand_tau();
	double phi = rand_phi();
	double theta = rand_theta();
	
	dir[0] = sin(theta)*cos(phi);
	dir[1] = sin(theta)*sin(phi);
	dir[2] = cos(theta);
	
	tau_cur = 0.0;
	
	nScatt++;
	scattered = true;
}

void Photon::update(){
	scattered = false;
	
	double next_face[3]; //x y and z faces, In physical space, that the photon will pass next for each axis
	double next_face_dist[3]; //Distance, along photon path to that face
	double next_face_dist_min = 1e99; //Distance, along photon path to face that will be hit next 
	double midpoint[3]; //Coordinate of mid point from current position to face that photon will hit, using this instead of photon position avoids ambiguity is starting on face
	
	for(int i = 0; i < 3; i++){
		next_face[i] = pos[i]; //If dir[i] is 0 then this will be right
		next_face_dist[i] = 1e9;
		
		//TODO check all this and make more elegent
		if(dir[i] > 0){ //Determine if lower or upper face is needed
			next_face[i] = grid_space[i]*(floor((pos[i] - grid_left[i])/grid_space[i]) + 1) + grid_left[i]; //Floor + 1 as opposed to ceil incase photon starts on cell face
		} else if(dir[i] < 0){
			next_face[i] = grid_space[i]*(ceil((pos[i] - grid_left[i])/grid_space[i]) - 1) + grid_left[i];
		}
		
		next_face_dist[i] = (next_face[i] - pos[i])/dir[i];
		
		if(next_face_dist[i] == 0){ //Sometimes "whole" numbers are rounded up/down by ceil due to floating point errors, this is to check for that
			if(dir[i] > 0){ 
				next_face[i] += grid_space[i];
			} else if(dir[i] < 0){
				next_face[i] -= grid_space[i];
			}
			next_face_dist[i] = (next_face[i] - pos[i])/dir[i];
		}
		
		if(next_face_dist[i] < next_face_dist_min){
			next_face_dist_min = next_face_dist[i];
		}
	}
	
	//Calculate midpoint
	for(int i = 0; i < 3; i++){
		midpoint[i] = pos[i] + dir[i]*next_face_dist_min/2.0;
	}
	
	//Get density
	double rho = get_rho(midpoint[0], midpoint[1], midpoint[2]);
	double dtau = rho*opacity*next_face_dist_min;
	
	
	if(dtau + tau_cur > tau_target && !is_peel){ //Going to interact in this cell
		double ds = next_face_dist_min*(tau_target - tau_cur)/(dtau);
		move(ds);
		interact();
	} else { //Keep going!
		tau_cur += dtau;
		move(next_face_dist_min);
	}
	
	for(int i = 0; i < 3; i++){ //Check if escaped
		if(pos[i] >= grid_right[i] || pos[i] <= grid_left[i]){
			escaped = true;
			break;
		}
	}
}


void Photon::move(double d){
	for(int i = 0; i < 3; i++) {
		pos[i] += d*dir[i];
	}
}

void Photon::interact(){
	if(rand_double() <= albedo){ //Scatter
		scatter();
	} else { //Absorb
		absorbed = true;
	}
}

double Photon::radius2(){
	return (pos[0]*pos[0] + pos[1]*pos[1] + pos[2]*pos[2]);
}

Photon Photon::peel(double ang1, double ang2){
	Photon p (pos[0], pos[1], pos[2], ang1, ang2, true);
	return p;
}

double Photon::get_tau_cur(){
	return tau_cur;
}
