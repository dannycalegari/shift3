#pragma once

#include <math.h>
#include <complex>

// abbreviations

#define cpx		std::complex<double>				// complex number
#define cvec 	std::vector<cpx>					// vector of complex numbers

// global constants

#define PI 		3.1415926535897932384626433
#define TWOPI	6.283185307179586476925286766559
extern const cpx I;

// polynomial functions

cpx eval(cpx p, cpx q, cpx z);
cpx eval_iterate(cpx p, cpx q, cpx z, int n);
cpx derivative(cpx p, cpx q, cpx z);
std::array<cpx, 2> critical_points(cpx p, cpx q, cpx z);
std::array<cpx, 3> roots(cpx p, cpx q);
cpx newton_root(cpx p, cpx q, cpx z);
std::array<cpx, 3> preimage(cpx p, cpx q, cpx z);
cpx newton_preimage(cpx p, cpx q, cpx z, cpx ww);

