/* polynomial.cc

	basic functions for a cubic depressed polynomial; i.e. of the form

	f(z) = z^3 + pz + q for complex numbers p, q

	somewhere (earlier?) we need:

#include <complex>
#define cpx		complex<double>				// complex number
#define PI 		3.1415926535897932384626433

Probably should re-implement this with a new struct 'poly' which is
a cubic depressed polynomial

struct poly{
	cpx p,q;
};

*/

#include "polynomial.h"

const cpx I (0.0,1.0);
cpx P  = 1.0+0.3*I;	// initial value
cpx Q  = 0.7-0.4*I; // initial value;

cpx eval(cpx p, cpx q, cpx z){
	// returns value of polynomial on z; i.e z^3 + p*z + q
	return((z*z*z) + (p*z) + q);
};

cpx eval_iterate(cpx p, cpx q, cpx z, int n){
	// returns value of nth iterate of polynomial on z;
	// i.e f(z) = z^3 + p*z + q
	// returns f^n(z)
	int i;
	cpx w;

	w=z;

	for(i=0;i<n;i++){
		w=eval(p,q,w);
	};
	return(w);
};

cpx derivative(cpx p, cpx q, cpx z){
	// returns derivative of polynomial; i.e. 3*z^2 + p
	return((3.0*z*z)+p);
};

std::array<cpx, 2> critical_points(cpx p, cpx q){
	// returns critical points of polynomial; i.e. roots of derivative
	// returns as an array of cpx numbers

	std::array<cpx, 2> c;
	c[0]=sqrt(-1.0*p/3.0);
	c[1]=-1.0*c[0];
	return(c);
};

std::array<cpx, 3> roots(cpx p, cpx q){
	// returns roots of depressed cubic z^3 + p*z + q
	// using Cardano's formula
	// returns as an array r of cpx numbers

	std::array<cpx, 3> r;
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

cpx newton_root(cpx p, cpx q, cpx z){
	// finds root by Newton's method with initial guess z
	// useful for analytic continuation of paths
	// numerically unstable near a multiple root!

	cpx w,a,b;
	w=z;	// initial guess
	a=1.0;
	while(abs(a)>0.00000001){
		a=(w*w*w)+(p*w)+q;
		b=(3.0*w*w)+p;
		w=w-(a/b);
	};
	return(w);

};

std::array<cpx, 3> preimage(cpx p, cpx q, cpx z){
	// returns w such that w^3 + p*w + q = z
	// returns as an array r of cpx numbers
	// entries of r are the roots of the polynomial w^3 + p*w + (q-z);

	return(roots(p,q-z));
};

cpx newton_preimage(cpx p, cpx q, cpx z, cpx ww){
	// returns w such that w^3 + p*w + q = z by Newton's method
	// with initial guess ww

	return(newton_root(p,q-z,ww));
};

