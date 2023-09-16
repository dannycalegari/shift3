#pragma once

#include <vector>
#include "lamination.h"
#include "polynomial.h"
// functions

double pow(double d, int i);
cpx bottcher(cpx p, cpx q, cpx z);
std::vector<std::vector<cpx>> Julia_green(cpx p, cpx q);
std::array<leaf,2> critical_bottcher_coordinates(cpx p, cpx q);
void bottcher_height(cpx p, cpx q, cpx z, cpx yy, leaf &L);
void bottcher_angle(cpx p, cpx q, cpx z, cpx yy, leaf &L);
