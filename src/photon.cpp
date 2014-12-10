#include <cmath>
#include <iostream>
#include <iomanip>
#include "photon.h"
#include "random.h"
#include "grid.h"

std::array<double, 3> get_direction(double theta, double phi);

extern void dispose();

Photon::Photon(double x, double y, double z) {
	double theta = random_gen::rand_theta();
	double phi = random_gen::rand_phi();
	
	init(x, y, z, theta, phi, false);
}

Photon::Photon(double x, double y, double z, double ang1, double ang2, bool scan) {
	init(x, y, z, ang1, ang2, scan);
}

void Photon::init(double x, double y, double z, double ang1, double ang2, bool scan) {
	pos = {{x, y, z}};
	nScatt = 0;
	if (scan) {
		tau_target = 1.0e99;
	} else {
		tau_target = random_gen::rand_tau();
	}
	tau_cur = 0.0;
	absorbed = false;
	escaped = false;
	is_scan = scan;
	
	dir = get_direction(ang1, ang2);

}

void Photon::scatter() {
	tau_target = random_gen::rand_tau();
	double phi = random_gen::rand_phi();
	double theta = random_gen::rand_theta();
	
	dir = get_direction(theta, phi);
	
	tau_cur = 0.0;
	
	nScatt++;
	scattered = true;
}

void Photon::update(const Grid& grid) {
	scattered = false;
	
	Point cell = grid.get_cell(pos); //Cell containing photon
	
	Point next_cell; //x y and z cell faces that the photon will pass next
	for (int i = 0; i < 3; i++) {
		//If moving in positive direction, we want the face above
		if (dir[i] > 0) { 
			next_cell[i] = cell[i] + 1;
		}  else {
			next_cell[i] = cell[i];
		}
	}
	
	Position next_face = grid.get_position(next_cell); //x y and z faces, In physical space, that the photon will pass next for each axis
	std::array<double, 3> next_face_dist; //Distance, along photon path to that face
	Position midpoint; //Coordinate of mid point from current position to face that photon will hit, using this instead of photon position avoids ambiguity is starting on face
	
	for (int i = 0; i < 3; i++) {
		next_face_dist[i] = 1e99;
		
		if (dir[i] != 0) {
			next_face_dist[i] = (next_face[i] - pos[i]) / dir[i];
		} else {
			next_face_dist[i] = 1e99;
		}
	}
	
	//Sometimes "whole" numbers are rounded up/down due to floating point errors if photon is very close to a face
	//Shift to next face if within 1e-10 of the cell face
	bool cell_corrected = false;
	for (int i = 0; i < 3; i++) {
		if (next_face_dist[i] < 1e-10) {
			cell_corrected = true;
			if (dir[i] > 0) { 
				next_cell[i] += 1;
			} else if (dir[i] < 0) {
				next_cell[i] -= 1;
			}
		}
	}
	if (cell_corrected) {
		next_face = grid.get_position(next_cell);
		for (int i = 0; i < 3; i++) {
			if (dir[i] != 0) {
				next_face_dist[i] = (next_face[i] - pos[i]) / dir[i];
			} else {
				next_face_dist[i] = 1e99;
			}
		}
	}
	
	//Find which face will be hit first
	double next_face_dist_min = 1e99; //Distance, along photon path to face that will be hit next
	for (int i = 0; i < 3; i++) {
		if (next_face_dist[i] < next_face_dist_min) {
			next_face_dist_min = next_face_dist[i];
		}
	}
	
	//Calculate midpoint, to take our rho value from
	for (int i = 0; i < 3; i++) {
		midpoint[i] = pos[i] + dir[i] * next_face_dist_min / 2.0;
	}
	
	double rho = grid.get_rho(midpoint);
	double dtau = rho * grid.get_opacity() * next_face_dist_min;
	
	if (dtau + tau_cur > tau_target && !is_scan) { //Going to interact in this cell
		double ds = next_face_dist_min * (tau_target - tau_cur) / (dtau);
		move(ds);
		interact(grid.get_albedo());
	} else { //Keep going!
		tau_cur += dtau;
		move(next_face_dist_min);
	}
	
	if (!grid.is_on_grid(pos)) {
		escaped = true;
	}
}


void Photon::move(double d) {
	for (int i = 0; i < 3; i++) {
		pos[i] += d*dir[i];
	}
}

void Photon::interact(double albedo) {
	if (random_gen::rand_double() <= albedo) { //Scatter
		scatter();
	} else { //Absorb
		absorbed = true;
	}
}

double Photon::radius2() {
	return ((pos[0] * pos[0]) + (pos[1] * pos[1]) + (pos[2] * pos[2]));
}

Photon Photon::peel(double ang1, double ang2) {
	Photon p (pos[0], pos[1], pos[2], ang1, ang2, true);
	return p;
}

double Photon::get_tau_cur() {
	return tau_cur;
}

std::array<double, 3> get_direction(double theta, double phi) {
	std::array<double, 3> direction = {{cos(theta) * cos(phi),
	                                    sin(theta) * cos(phi),
	                                    sin(phi)}};
	return direction;
}
