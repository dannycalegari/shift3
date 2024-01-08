#pragma once

#include <complex>
#include <vector>

#include "lamination.h"
#include "polynomial.h"

// functions
double pow(double d, int i);
cpx better_bottcher(cpx p, cpx q, cpx z);
cpx bottcher(cpx p, cpx q, cpx z);
cpx bottcher2(cpx p, cpx q, cpx z);
std::vector<std::vector<cpx>> Julia_green(cpx p, cpx q);
void bottcher_first(cpx p, cpx q, cpx z, cpx yy, double& alpha, double& rho);
void bottcher_second(cpx p, cpx q, cpx z, cpx yy, double& beta);
void correct_angles(double& alpha, double& beta);
void compute_flow_segments(cpx b, double S, double L, double LL, std::vector<cpx>& flow_segment, bool flag_rotate);
std::array<leaf, 2> critical_bottcher_coordinates(cpx p, cpx q);
void compute_newton_preimages(int initial_value, int array_length, int offset_a, int offset_b, cpx p, cpx q,  std::vector<cpx>& flow_segment);
void compute_newton_preimages(int initial_value, int array_length, int offset_a, cpx p, cpx q, std::vector<cpx> flow_segment, std::vector<cpx>& flow_segment_2);