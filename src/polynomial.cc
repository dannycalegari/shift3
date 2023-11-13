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
#include <iostream>

#include "polynomial.h"

const cpx I(0.0, 1.0);
cpx P = 1.0 + 0.3 * I; // initial value
cpx Q = 0.7 - 0.4 * I; // initial value;
double zoom = 2.0; // initial value
cpx P_center = 0.0; // initial value;
cpx Q_center = 0.0; // initial value;

cpx eval(cpx p, cpx q, cpx z)
{
    // returns value of polynomial on z; i.e z^3 + p*z + q
    return ((z * z * z) + (p * z) + q);
};

cpx eval_iterate(cpx p, cpx q, cpx z, int n)
{
    // returns value of nth iterate of polynomial on z;
    // i.e f(z) = z^3 + p*z + q
    // returns f^n(z)
    int i;
    cpx w;

    w = z;

    for (i = 0; i < n; i++) {
        w = eval(p, q, w);
    };
    return (w);
};

cpx derivative(cpx p, cpx q, cpx z)
{
    // returns derivative of polynomial; i.e. 3*z^2 + p
    return ((3.0 * z * z) + p);
};

cpx deriv_iterate(cpx p, cpx q, cpx z, int n)
{
    // returns derivative of nth iterate of polynomial on z;
    // i.e. \prod_{i=0}^{n-1} (3*z_i^2 + p) where z_i: = f^i(z)
    int i;
    cpx w, ww;

    w = z;
    ww = derivative(p, q, w);
    if (n >= 1) {
        for (i = 0; i < n - 1; i++) {
            w = eval(p, q, w);
            ww = ww * derivative(p, q, w);
        };
    } else {
        ww = 1.0;
    };
    return (ww);
};

std::array<cpx, 2> critical_points(cpx p, cpx q)
{
    // returns critical points of polynomial; i.e. roots of derivative
    // returns as an array of cpx numbers

    std::array<cpx, 2> c;
    c[0] = sqrt(-1.0 * p / 3.0);
    c[1] = -1.0 * c[0];
    return (c);
};

std::array<cpx, 3> roots(cpx p, cpx q)
{
    // returns roots of depressed cubic z^3 + p*z + q
    // using Cardano's formula
    // returns as an array r of cpx numbers

    std::array<cpx, 3> r;
    cpx Delta;
    cpx u, C, zeta;

    zeta = exp(I * 2.0 * PI / 3.0); // complex cube root of 1
    Delta = (q * q / 4.0) + (p * p * p / 27.0); // discriminant of cubic
    u = (-1.0 * q / 2.0) + sqrt(Delta);
    C = exp(log(u) / 3.0); // complex cube root of u
    r[0] = C - (p / (3.0 * C));
    r[1] = (C * zeta) - (p / (3.0 * C * zeta));
    r[2] = (C * zeta * zeta) - (p / (3.0 * C * zeta * zeta));

    return (r);
};

cpx newton_root(cpx p, cpx q, cpx z)
{
    // finds root by Newton's method with initial guess z
    // useful for analytic continuation of paths
    // numerically unstable near a multiple root!

    cpx w, a, b;
    w = z; // initial guess
    a = 1.0;
    while (abs(a) > 0.00000001) {
        a = (w * w * w) + (p * w) + q;
        b = (3.0 * w * w) + p;
        w = w - (a / b);
    };
    return (w);
};

std::array<cpx, 3> preimage(cpx p, cpx q, cpx z)
{
    // returns w such that w^3 + p*w + q = z
    // returns as an array r of cpx numbers
    // entries of r are the roots of the polynomial w^3 + p*w + (q-z);

    return (roots(p, q - z));
};

cpx newton_preimage(cpx p, cpx q, cpx z, cpx ww)
{
    // returns w such that w^3 + p*w + q = z by Newton's method
    // with initial guess ww

    return (newton_root(p, q - z, ww));
};

cpx newton_preimage_iterate(cpx p, cpx q, cpx z, cpx ww, int n)
{
    // returns w such that f^n(w) = z by Newton's method
    // with initial guess ww

    cpx w, a, b;
    double t;

    t = abs(z);
    w = ww; // initial guess
    a = 2.0 * z; // initial bad guess for image
    while (abs(a - z) > 0.00000001 * t) {
        a = eval_iterate(p, q, w, n); // f^n(w)
        b = deriv_iterate(p, q, w, n); // (f^n)'(w)
        w = w - (a - z) / b; // adjust w guess
    };
    return (w);
};

cpx newton_preimage_vector_iterate(cpx p, cpx q, cpx z, cpx ww, int n)
{
    // returns w such that f^n(w) = z by Newton's method
    // with initial guess ww
    // uses *vector* Newton's method; is it better and more stable?
    // experimentally, it seems slower by a factor of 2

    cpx w, v, b;
    std::vector<cpx> a;
    double error;
    int i;

    a.clear();
    a.push_back(ww); // initial guess
    for (i = 0; i < n; i++) {
        a.push_back(eval(p, q, a[i]));
    };
    w = a[n];
    a[n] = z; // adjust last element of vector!

    while (1) {
        error = 0.0;
        for (i = 0; i < n; i++) {
            v = eval(p, q, a[i]);
            b = derivative(p, q, a[i]);
            a[i] = a[i] - (v - a[i + 1]) / b; // adjust guess
            error = error + abs(eval(p, q, a[i]) - a[i + 1]) / abs(a[i + 1]);
        };
        if (error < 0.000000001)
            break;
    };
    return (a[0]);
};

void compute_period_and_multiplier(cpx p, cpx q, cpx z, double accuracy, int& period, cpx& multiplier)
{
    int i;
    cpx w, m;
    w = z;
    m = 1.0;
    for (i = 1; i < 20; i++) {
        m = m * derivative(p, q, w); // update multiplier
        w = eval(p, q, w); // update w
        if (abs(w - z) < accuracy) { // found a hit!
            period = i;
            multiplier = m;
            i = 20; // exit loop
        };
    };
    return;
};

std::array<double, 3> color_code(int iter, int maxiter, int escape)
{
    // not sure if this should really go in this file, but why not?

    double r, g, b;
    std::array<double, 3> H;

    if (escape == 0) {
        if (iter <= maxiter / 6) {
            r = 6.0 * iter / maxiter;
            g = 0.0;
            b = 0.0;
        } else if (iter <= maxiter / 2) {
            r = 1.0;
            g = 3.0 * (iter - maxiter / 6) / maxiter;
            b = 0.0;
        } else {
            r = 1.0;
            g = 1.0;
            b = 2.0 * (iter - maxiter / 2) / maxiter;
        };
        H[0] = r;
        H[1] = g;
        H[2] = b;
    } else if (escape == 1) {
        if (iter <= maxiter / 6) {
            g = 6.0 * iter / maxiter;
            b = 0.0;
            r = 0.0;
        } else if (iter <= maxiter / 2) {
            g = 1.0;
            b = 3.0 * (iter - maxiter / 6) / maxiter;
            r = 0.0;
        } else {
            g = 1.0;
            b = 1.0;
            r = 2.0 * (iter - maxiter / 2) / maxiter;
        };
        H[0] = r;
        H[1] = g;
        H[2] = b;
    } else {
        if (iter <= maxiter / 6) {
            g = 6.0 * iter / maxiter;
            b = 0.0;
            r = 1.0;
        } else if (iter <= maxiter / 2) {
            g = 1.0;
            b = 3.0 * (iter - maxiter / 6) / maxiter;
            r = 1.0;
        } else {
            g = 1.0;
            b = 1.0;
            r = 1.0 - 2.0 * (iter - maxiter / 2) / maxiter;
        };
        H[0] = r;
        H[1] = g;
        H[2] = b;
    };
    return (H);
};
