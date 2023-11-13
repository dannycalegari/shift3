#pragma once

#include <complex>
#include <gtkmm.h>
#include <iostream>
#include <math.h>
#include <stdio.h>

#include "elamination_drawing_area.h"
#include "julia_drawing_area.h"
#include "polynomial.h"
#include "pq_drawing_area.h"

// abbreviations
#define cpx std::complex<double> // complex number
#define cvec std::vector<cpx> // vector of complex numbers

// global constants
#define PI 3.1415926535897932384626433
#define TWOPI 6.283185307179586476925286766559
extern const cpx I;

class ShiftApplication : public Gtk::Application {
public:
    static Glib::RefPtr<ShiftApplication> create(int& argc, char**& argv);
    bool interpolate_mode;

protected:
    ShiftApplication();
    bool on_button_press_p(GdkEventButton* event);
    bool on_button_press_q(GdkEventButton* event);
    bool on_key_press(GdkEventKey* event);

private:
    void on_activate() override;
    Gtk::Window* window;
    //    Gtk::Grid* grid;
    PQDrawingArea* p_area;
    PQDrawingArea* q_area;
    JuliaDrawingArea* julia_area;
    ElaminationDrawingArea* elamination_area;
    //  double a, b;
};