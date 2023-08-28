#pragma once

#include <gtkmm.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <complex>
#include "shift_drawing_area.h"
#include "julia_drawing_area.h"

// abbreviations

#define cpx		std::complex<double>				// complex number
#define cvec 	std::vector<cpx>					// vector of complex numbers

// global constants

#define PI 		3.1415926535897932384626433
#define TWOPI	6.283185307179586476925286766559
extern const cpx I;

class ShiftApplication : public Gtk::Application {
public:
    static Glib::RefPtr<ShiftApplication> create(int& argc, char**& argv);

protected:
    ShiftApplication();

private:
    void on_activate() override;

    Gtk::Window* window;
    Gtk::Grid* grid;
    ShiftDrawingArea* p_square;
    ShiftDrawingArea* q_square;
    JuliaDrawingArea* julia_square;
    ShiftDrawingArea* elamination_square;
};