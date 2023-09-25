/* green.cc

bottcher coordinates for cubic depressed polynomial

also routine to draw green lines

*/
#include <future>
#include <iostream>
#include <thread>

#include "green.h"
#include "lamination.h"
#include "polynomial.h"

double pow(double d, int i)
{
    // returns d^i
    if (i == 0) {
        return (1.0);
    } else {
        return (d * pow(d, i - 1));
    };
};

std::vector<cpx> radial_segment(cpx z, double R, int i)
{
    // returns radial segment of i+1 points from z to zR/|z|
    std::vector<cpx> L, LL;
    int j;
    double T, jj, ii;

    L.clear();
    T = log(R);
    for (j = 0; j < i + 1; j++) {
        jj = (double)j;
        ii = (double)i;
        L.push_back(z * exp(T * jj / ii));
    };
    return (L);
};

cpx better_bottcher(cpx p, cpx q, cpx z)
{
    // warning: this is fast when it works, but it is very buggy
    // and infrequently works, or is numerically inaccurate for no apparent reason!

    double maxsize, T, b_abs, b_arg;
    bool escape;
    int i, j, escape_iterate, max_iterate;
    std::vector<cpx> w, L, LL;
    cpx u;

    maxsize = 1000.0;
    max_iterate = 20;
    escape = false;

    w.clear();
    w.push_back(z);

    for (i = 0; i < max_iterate; i++) {
        w.push_back(eval(p, q, w[i]));
        if (abs(w[i + 1]) > maxsize) {
            T = log(abs(w[i + 1]));
            b_abs = exp(T / pow(3.0, i + 1));
            escape = true;
            escape_iterate = i; // not i+1;
            i = max_iterate;
        };
    };
    if (escape == false) {
        return (1.0);
    };

    // determine argument b_arg of bottcher coordinate

    L = radial_segment(w[escape_iterate], 10000.0, 1000);

    for (i = escape_iterate - 1; i > -1; i--) {
        LL.clear();
        LL.push_back(w[i]);
        for (j = 1; j < L.size(); j++) {
            u = newton_preimage(p, q, L[j], LL[j - 1]);
            LL.push_back(u);
        };
        L = radial_segment(LL[LL.size() - 1], 10000.0, 1000);
        LL.insert(LL.end(), L.begin(), L.end());
        L = LL;
    };

    b_arg = arg(L[L.size() - 1]);
    std::cout << "bottcher coordinate is " << b_abs * exp(I * b_arg) << "\n";
    return (b_abs * exp(I * b_arg));
};

cpx bottcher(cpx p, cpx q, cpx z)
{
    /* returns bottcher coordinate of z for the polynomial
    z^3 + z*p + q

    Well-defined iff z is in the attracting basin of infinity;
    otherwise by default return 1.0;
    */

    int i, escape_iterate, max_iterate;
    cpx w;
    double T, b_abs, b_arg, maxsize;
    bool escapes;

    maxsize = 1000.0;
    max_iterate = 20;
    w = z;
    escapes = false;

    // determine absolute value b_abs of bottcher coordinate

    for (i = 0; i < max_iterate; i++) {
        w = eval(p, q, w);
        if (abs(w) > maxsize) {
            T = log(abs(w));
            b_abs = exp(T / pow(3.0, i + 1));
            escapes = true;
            escape_iterate = i + 1;
            i = max_iterate; // escape loop; is there a more elegant way?
        };
    };
    if (escapes == false) {
        return (1.0);
    };

    // determine argument b_arg of bottcher coordinate

    double alpha;
    cpx wd, zz, y;

    alpha = arg(w); // argument of f^escape_iterate(z)

    zz = z;
    while (abs(zz) < maxsize) {

        w = eval_iterate(p, q, zz, escape_iterate);
        wd = deriv_iterate(p, q, zz, escape_iterate);

        /* head in the direction of y
                which has abs(w')=abs(w)*1.01 and
                arg(w')=alpha */

        y = abs(w) * 1.0001 * exp(I * alpha);
        zz = zz + ((y - w) / wd);

        if (abs(w) > maxsize * maxsize) {
            escape_iterate = escape_iterate - 1;
            w = eval_iterate(p, q, zz, escape_iterate);
            alpha = arg(w);
        };
    };

    b_arg = arg(zz);

    w = b_abs * exp(I * b_arg);
    return (w);
};

cpx bottcher2(cpx p, cpx q, cpx z)
{
    /* returns bottcher coordinate of z for the polynomial
    z^3 + z*p + q

    Well-defined iff z is in the attracting basin of infinity;
    otherwise by default return 1.0;
    */

    int i, escape_iterate, max_iterate;
    cpx w, zz;
    double T, b_abs, b_arg, maxsize;
    bool escapes;

    maxsize = 1000.0;
    max_iterate = 20;
    w = z;
    escapes = false;

    // determine absolute value b_abs of bottcher coordinate

    for (i = 0; i < max_iterate; i++) {
        w = eval(p, q, w);
        if (abs(w) > maxsize) {
            T = log(abs(w));
            b_abs = exp(T / pow(3.0, i + 1));
            escapes = true;
            escape_iterate = i + 1;
            i = max_iterate; // escape loop; is there a more elegant way?
        };
    };
    if (escapes == false) {
        return (1.0);
    };

    // determine argument b_arg of bottcher coordinate
    // zz is initialized to z.
    // the method will be to move zz iteratively in such a way that
    // f^n(zz) moves radially outward, where n is the biggest iterate
    // such that abs(f^n(zz))<maxsize*maxsize.
    // when abs(zz)>maxsize we return the argument of zz.

    zz = z;
    w = eval_iterate(p, q, zz, escape_iterate); // w=f^escape_iterate(zz);
    while (abs(zz) < maxsize) {
        w = w * 1.05; // move w radially outwards
        // using newton's method, zz is adjusted so that f^escape_iterate(zz)=y

        //	zz=newton_preimage_vector_iterate(p,q,w,zz,escape_iterate);	slower by a factor of 2
        zz = newton_preimage_iterate(p, q, w, zz, escape_iterate);

        if (abs(w) > maxsize * maxsize) { // if w is too big
            escape_iterate = escape_iterate - 1; // decrement escape_iterate
            w = eval_iterate(p, q, zz, escape_iterate); // new value of w
        };
    };

    b_arg = arg(zz);

    w = b_abs * exp(I * b_arg);

    return (w);
};

std::vector<std::vector<cpx>> Julia_green(cpx p, cpx q)
{
    /* returns approximate green gradient flowlines (ie lines whose bottcher coordinates
    have constant argument) for cubic polynomial z^3 + p*z + q. Along the way we get
    an estimate of the Bottcher coordinates of the critical points; probably this is
    faster than the method above, so some future version of this program should return
    these coordinates. */

    std::vector<std::vector<cpx>> flow_collection;
    std::vector<cpx> flow_segment, flow_segment_2;
    cpx b, b1, b2, z;
    std::array<cpx, 3> c;
    double L, LL; // large real number, eg 100

    L = 100.0;
    LL = L * L * L;

    flow_collection.clear();

    /* initializing flow segment */

    flow_segment.clear();

    flow_segment.push_back(LL);
    c = preimage(p, q, LL); // array of 3 preimages

    b = newton_preimage(p, q, LL, L); // b is preimage closest to L

    // b1 and b2 are the other two preimages

    if (abs(b - c[0]) < 0.01) {
        b1 = c[1];
        b2 = c[2];
    } else if (abs(b - c[1]) < 0.01) {
        b1 = c[0];
        b2 = c[2];
    } else {
        b1 = c[0];
        b2 = c[1];
    };

    /* initial segment is a straight line consisting of 10 segments
    subdivided logarithmically from LL to b */

    double S, T;

    for (S = 0.9; S >= 0.0; S = S - 0.1) {
        T = exp(log(L) * exp(S * log(3.0))); // from 100 to 1000000
        z = b + ((T - L) / (LL - L)) * (LL - b);
        flow_segment.push_back(z);
    };

    /* continuing flow segment by inverse image,
    obtained by Newton's method for continuity */

    int i, j;
    j = 10; // initial value

    for (i = 0; i < 500; i++) {
        z = newton_preimage(p, q, flow_segment[j - 9], flow_segment[j]);
        flow_segment.push_back(z);
        j++;
        if (abs(flow_segment[j - 1] - flow_segment[j]) < 0.0001) { // keep going until segment is small
            i = 500;
        };
    };

    flow_collection.push_back(flow_segment); // add segment to the collection

    /* compute two other preimages of initial flow_segment */

    flow_segment_2.clear();
    L = abs(b1);
    LL = L * L * L;
    for (S = 1.0; S >= 0.0; S = S - 0.1) {
        T = exp(log(L) * exp(S * log(3.0))); // from 100 to 1000000
        z = L + ((T - L) / (LL - L)) * (LL - L);
        z = z * exp(I * arg(b1));
        flow_segment_2.push_back(z);
    };

    j = 1; // initial value
    for (i = 1; i < flow_segment.size() - 10; i++) {
        z = newton_preimage(p, q, flow_segment[j], flow_segment_2[10 + j - 1]);
        flow_segment_2.push_back(z);
        j++;
    };

    flow_collection.push_back(flow_segment_2); // add segment to the collection

    flow_segment_2.clear();
    L = abs(b2);
    LL = L * L * L;
    for (S = 1.0; S >= 0.0; S = S - 0.1) {
        T = exp(log(L) * exp(S * log(3.0))); // from 100 to 1000000
        z = L + ((T - L) / (LL - L)) * (LL - L);
        z = z * exp(I * arg(b2));
        flow_segment_2.push_back(z);
    };

    j = 1; // initial value
    for (i = 1; i < flow_segment.size() - 10; i++) {
        z = newton_preimage(p, q, flow_segment[j], flow_segment_2[10 + j - 1]);
        flow_segment_2.push_back(z);
        j++;
    };

    flow_collection.push_back(flow_segment_2); // add segment to the collection

    /* recursively add all preimages of last added flow segments */

    int l, k;

    for (l = 1; l < 81; l++) {
        c = preimage(p, q, flow_collection[l][0]);
        for (k = 0; k < 3; k++) {
            flow_segment.clear();

            if (l < 9) {
                L = abs(c[k]);
                LL = L * L * L;
                for (S = 1.0; S >= 0.0; S = S - 0.1) {
                    T = exp(log(L) * exp(S * log(3.0))); // from 100 to 1000000
                    z = L + ((T - L) / (LL - L)) * (LL - L);
                    z = z * exp(I * arg(c[k]));
                    flow_segment.push_back(z);
                };

                j = 1; // initial value
                for (i = 1; i < flow_collection[l].size() - 10; i++) {
                    z = newton_preimage(p, q, flow_collection[l][j], flow_segment[10 + j - 1]);
                    flow_segment.push_back(z);
                    j++;
                };
            } else {
                flow_segment.push_back(c[k]);
                j = 1;
                for (i = 1; i < flow_collection[l].size() - 10; i++) {
                    z = newton_preimage(p, q, flow_collection[l][j], flow_segment[j - 1]);
                    flow_segment.push_back(z);
                    j++;
                };
            };

            flow_collection.push_back(flow_segment);
        };
    };

    return (flow_collection);
}

cpx bottcher_3(cpx p, cpx q, cpx z)
{
    /* returns bottcher coordinate of z for the polynomial
    z^3 + z*p + q

    Well-defined iff z is in the attracting basin of infinity;
    otherwise by default return 1.0;
    */
    std::vector<cpx> orbit;

    cpx w = z;
    double maxsize = 1000.0;
    int max_iterate = 20, escape_iterate;
    bool escapes = false;

    for (int i = 0; i < max_iterate; i++) {
        orbit.push_back(w);
        w = eval(p, q, w);
        if (abs(w) > maxsize) {
            escapes = true;
            escape_iterate = i + 1;
            break;
        };
    };
    if (escapes == false) {
        return (1.0);
    };

    std::vector<std::vector<cpx>> paths;
    /* paths[j] is path from w^3 to w*(some cube root of unity),
       such that arg(paths[j].last) is in [j/3,(j+1)/3]*2pi */
    cpx omega = exp(cpx(0, 2.0 * M_PI / 3)), omega2 = exp(cpx(0, 4.0 * M_PI / 3));
    for (int j = 0; j < 3; j++) {
        cpx start = log(-pow(w, 3.0)) + cpx(0, M_PI),
            stop = log(-w) + cpx(0, M_PI) + cpx(0, j * 2.0 * M_PI / 3);
        // this log(-...) yoga to use the branch cut along the positive axis

        for (double i = 0; i < 100; i++)
            paths[j].push_back(exp((i * stop + (100 - i) * start) / 100.0));
    }

    for (int t = escape_iterate - 1; t >= 0; t--) {
        // lift paths by continuity
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < paths[j].size(); i++) {
                if (i == 0)
                    paths[j][i] = orbit[t];
                else
                    paths[j][i] = newton_preimage(p, q, paths[j][i], paths[j][i - 1]);
            }
        }

        // record new trit into w
        int trit = -1;
        for (int j = 0; j < 3; j++)
            if (abs(paths[j].back() - orbit[t]) < abs(paths[(j + 1) % 3].back() - orbit[t])
                && abs(paths[j].back() - orbit[t]) < abs(paths[(j + 2) % 3].back() - orbit[t]))
                trit = j;

        w = -pow(-w, 1.0 / 3) + cpx(0, trit * 2.0 * M_PI / 3);
    }
    return w;

    // error not even tested, just passes compilation!
}

std::array<leaf, 2> critical_bottcher_coordinates(cpx p, cpx q)
{
    std::array<leaf, 2> CL;
    leaf L;

    std::array<cpx, 2> C = critical_points(p, q);

    double alpha, beta, rho;
    int i, j, d;
    cpx z, w, bw, bww, wd, bwd, y, yy;
    cpx sec_der;
    bool escaped;
    int maxiter;
    double maxsize;

    for (i = 0; i < 2; i++) {
        z = C[i];
        w = eval(p, q, z); // critical value
        escaped = false;

        maxiter = 255;
        maxsize = 10000.0;

        for (j = 0; j < maxiter; j++) {
            w = eval(p, q, w);
            if (abs(w) > maxsize) {
                d = j + 1;
                escaped = true;
                j = maxiter;
            }
        };

        if (escaped == false) {
            L.height = 1.0;
            L.angle[0] = 0.0;
            L.angle[1] = TWOPI / 3.0;
            CL[i] = L;
        } else {
            w = eval(p, q, z); // reset critical value
            bw = eval_iterate(p, q, w, d);
            bww = eval_iterate(p, q, w + 0.0001, d);
            bwd = (bww - bw) / 0.0001;
            y = abs(bw) * 1.001 * exp(I * arg(bw));
            wd = (y - bw) / bwd; // direction to perturb w to make f^d(w) increase radially

            //		cout << "w = " << w << "\n";
            //		cout << "wd = " << wd << "\n";

            // f'(z) = 3z^2 + p; f''(z) = 6z
            sec_der = 6.0 * z; // f''(z)

            yy = sqrt(wd * 2.0 / sec_der);
            //		cout << "yy = " << yy << "\n";
            //		cout << "z+yy = " << z+yy << " f(c+yy) = " << eval(P,z+yy) << " versus " << "w+wd = " << w+wd << "\n";
            // here 1

            std::thread th(bottcher_first, p, q, z, yy, std::ref(alpha), std::ref(rho));
            std::thread th2(bottcher_second, p, q, z, yy, std::ref(beta));
            th.join();
            th2.join();
            L.height = rho;

            // "correct" alpha and beta so that their difference is TWOPI/3
            correct_angles(alpha, beta);
            L.angle[0] = alpha;
            L.angle[1] = beta;

            CL[i] = L;
        };
    };
    return (CL);
};

void bottcher_first(cpx p, cpx q, cpx z, cpx yy, double& alpha, double& rho)
{
    cpx w = z - yy; // opposite perturbation of critical point
    w = bottcher2(p, q, w);
    alpha = arg(w);
    rho = abs(w);
};

void bottcher_second(cpx p, cpx q, cpx z, cpx yy, double& beta)
{
    cpx w = z + yy; // perturbation of critical point
    beta = arg(bottcher2(p, q, w));
};

void correct_angles(double& alpha, double& beta)
{
    // the difference should be either *exactly* TWOPI/3 or 2*TWOPI/3
    // this function corrects the smaller angle to the closest value making this true
    // assumes it is already within 0.01 of some choice; if not, it complains!

    if (alpha > beta) {
        if (abs(alpha - beta - (TWOPI / 3.0)) < 0.01) {
            beta = alpha - (TWOPI / 3.0);
        } else if (abs(alpha - beta - (2.0 * TWOPI / 3.0)) < 0.01) {
            beta = alpha - (2.0 * TWOPI / 3.0);
        } else {
            std::cout << "Something's wrong! alpha = " << alpha << " ; beta = " << beta << " ; difference = " << alpha - beta << "\n";
        };
    } else {
        if (abs(beta - alpha - (TWOPI / 3.0)) < 0.01) {
            alpha = beta - (TWOPI / 3.0);
        } else if (abs(beta - alpha - (2.0 * TWOPI / 3.0)) < 0.01) {
            alpha = beta - (2.0 * TWOPI / 3.0);
        } else {
            std::cout << "Something's wrong! alpha = " << alpha << " ; beta = " << beta << " ; difference = " << alpha - beta << "\n";
        };
    };
};
