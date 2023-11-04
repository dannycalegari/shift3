#pragma once

#include <array>
#include <complex>
#include <math.h>
#include <tuple>
#include <vector>

// abbreviations
#define cpx		std::complex<double>				// complex number
#define cvec 	std::vector<cpx>					// vector of complex numbers

// global constants
#define PI 		3.1415926535897932384626433
#define TWOPI	6.283185307179586476925286766559
extern const cpx I;
extern cpx P, Q, P_center, Q_center;
extern double zoom;

// polynomial functions
cpx eval(cpx p, cpx q, cpx z);
cpx eval_iterate(cpx p, cpx q, cpx z, int n);
cpx derivative(cpx p, cpx q, cpx z);
cpx deriv_iterate(cpx p, cpx q, cpx z, int n);
std::array<cpx, 2> critical_points(cpx p, cpx q);
std::array<cpx, 3> roots(cpx p, cpx q);
cpx newton_root(cpx p, cpx q, cpx z);
std::array<cpx, 3> preimage(cpx p, cpx q, cpx z);
cpx newton_preimage(cpx p, cpx q, cpx z, cpx ww);
cpx newton_preimage_iterate(cpx p, cpx q, cpx z, cpx ww, int n);
cpx newton_preimage_vector_iterate(cpx p, cpx q, cpx z, cpx ww, int n);
void compute_period_and_multiplier(cpx p, cpx q, cpx z, double accuracy, int &period, cpx &multiplier);
std::array<double,3> color_code(int iter, int maxiter, int escape);
typedef std::array<double,3> color_value;
typedef std::tuple<int, int> julia_pixels;

