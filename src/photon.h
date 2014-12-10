#pragma once

#include <array>
#include "grid.h"

class Photon {
		std::array<double, 3> dir; //Normalized direction vectors
		double tau_target; //Total optical depth this photon will travel before interacting
		double tau_cur; //Total integrated optical depth so far
		void move(double);
		void interact(double albedo);
		void scatter();
		void init(double, double, double, double, double, bool);
	public:
		Position pos;
		int nScatt;
		bool absorbed, escaped, scattered;
		bool is_scan; //A scanning photon will never interact
		Photon(double, double, double);
		Photon(double, double, double, double, double, bool);
		void update(const Grid&);
		double radius2();
		Photon peel(double, double);
		double get_tau_cur();
};
