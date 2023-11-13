/* lamination.h */
#pragma once
#include <array>
#include <vector>

struct leaf {
    double height; // should be > 1
    double angle[2]; // should be in the interval [-pi,pi]
};

std::vector<leaf> dynamical_lamination(int depth, std::array<leaf, 2> C);