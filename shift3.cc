/*	shift3.cc

	program for polynomial dynamics:
	julia sets, bottcher coordinates,
	elaminations
	
	March 2022 version 0.03

	Copyright Danny Calegari

	released under the terms of the GNU GPL
	
*/

// standard libraries to include

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <vector>
#include <complex>
#include <sstream>
#include <limits>
#include <ctime>
#include <assert.h>

using namespace std;

// preprocessor definitions

#define debug 	1							//	debug is true by default
#define verbose 1							//	verbose is true by default
#define PI 		3.1415926535897932384626433
#define TWOPI	6.283185307179586476925286766559
#define ACC		0.00000000001				// accuracy 1.0e-11
#define cpx		complex<double>				// complex number
#define cvec 	vector<cpx>					// vector of complex numbers
#define polynomial	vector<cpx>

cpx I (0.0,1.0);

#include "graphics.cc"
#include "polynomial.cc"
#include "lamination.cc"
#include "tautological.cc"
#include "green.cc"
#include "julia.cc"
#include "mandelbrot.cc"
#include "interface.cc"


int main(int argc, char *argv[]){ 

	setup_graphics();
	poly_user_interface();
	
	
	return(0);
};
