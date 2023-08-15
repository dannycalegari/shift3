/* polynomial.cc 

	basic functions for a cubic depressed polynomial; i.e. of the form
	
	f(z) = z^3 + pz + q for complex numbers p, q

	somewhere (earlier?) we need:
	
#include <complex>
#define cpx		complex<double>				// complex number
#define PI 		3.1415926535897932384626433
cpx I (0.0,1.0);

Probably should re-implement this with a new struct 'poly' which is
a cubic depressed polynomial

struct poly{
	cpx p,q;
};

*/

cpx eval(cpx p, cpx q, cpx z){
	// returns value of polynomial on z; i.e z^3 + p*z + q
	return((z*z*z) + (p*z) + q);
};

cpx derivative(cpx p, cpx q, cpx z){
	// returns derivative of polynomial; i.e. 3*z^2 + p
	return((3*z*z)+p);
};

cpx* critical_points(cpx p, cpx q, cpx z){
	// returns critical points of polynomial; i.e. roots of derivative
	// returns as an array of cpx numbers
	
	cpx c[2];
	c[0]=sqrt(-1.0*p/3.0);
	c[1]=-1.0*c[0];
	return(c);
};

cpx* roots(cpx p, cpx q){
	// returns roots of depressed cubic z^3 + p*z + q
	// using Cardano's formula
	// returns as an array r of cpx numbers
	
	cpx r[3];
	cpx Delta;
	cpx u, C, zeta;
	
	zeta = exp(I*2.0*PI/3.0);	// complex cube root of 1
	Delta = (q*q/4.0) + (p*p*p/27.0);	// discriminant of cubic
	u = (-1.0*q/2.0)+sqrt(Delta);
	C = exp(log(u)/3.0);	// complex cube root of u
	r[0] = C - (p/(3.0*C));
	r[1] = (C*zeta) - (p/(3.0*C*zeta));
	r[2] = (C*zeta*zeta) - (p/(3.0*C*zeta*zeta));
	
	return(r);	
};

cpx* preimage(cpx p, cpx q, cpx z){
	// returns w such that w^3 + p*w + q = z
	// returns as an array r of cpx numbers
	// entries of r are the roots of the polynomial w^3 + p*w + (q-z);

	return(roots(p,q-z));
};

