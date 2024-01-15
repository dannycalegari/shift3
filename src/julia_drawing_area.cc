/* julia_drawing_area.cc */

#include <future>
#include <iostream>
#include <thread>

#include "green.h"
#include "julia_drawing_area.h"

JuliaDrawingArea::JuliaDrawingArea()
{
    add_events(Gdk::BUTTON_PRESS_MASK);
    green_mode = false;
    signal_draw().connect(sigc::mem_fun(*this, &JuliaDrawingArea::on_draw));
    signal_button_press_event().connect(sigc::mem_fun(*this, &JuliaDrawingArea::on_button_press));
}

JuliaDrawingArea::~JuliaDrawingArea()
{

}

void JuliaDrawingArea::set_green_mode(bool green)
{
    green_mode = green;
    queue_draw();
}

bool JuliaDrawingArea::on_point_set(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gdk::Rectangle allocation = get_allocation();
    width = allocation.get_width();
    height = allocation.get_height();
    const int lesser = std::min(width, height);

    cr->save();
    cr->arc(input_x, input_y, lesser / 4.0, 0.0, 2.0 * M_PI); // full circle
    cr->set_source_rgba(0.0, 0.0, 0.8, 0.6); // partially translucent
    cr->fill_preserve();
    cr->restore(); // back to opaque black
    cr->stroke();

    return true;
}

void JuliaDrawingArea::on_pq_set()
{
    queue_draw();
}

bool JuliaDrawingArea::on_button_press(GdkEventButton* event)
{
    on_pq_set();
    green_mode = 1 - green_mode;
    return true;
}

julia_pixels JuliaDrawingArea::calc_points(cpx FI, int xc, int yc)
{
    double xx = FI.real() / 2.0;
    double yy = FI.imag() / 2.0;
    double r = sqrt(xx * xx + yy * yy);

    if (r > 3.0) {
        xx = xx * 3.0 / r;
        yy = yy * 3.0 / r;
    };
    int a = xc + xc * xx;
    int b = yc + yc * yy;
    return std::make_tuple(a, b);
}

bool JuliaDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
    int width = 0;
    int height = 0;
    if (allocation.get_width() > 0 && allocation.get_height() > 0) {
        width = allocation.get_width();
        height = allocation.get_height();
    }

    int xc, yc;
    xc = width / 2;
    yc = height / 2;

    if (green_mode == true) {
        // draw gradient flowlines of green function
        std::vector<std::vector<cpx>> F = Julia_green(P, Q); // should be Julia_green(p,q)
        cr->set_line_width(1.0);
        cr->set_source_rgb(0.5, 0.0, 0.0);
        julia_pixels j_points;
        cpx FI;
        for (int i = 0; i < (int)F.size(); i++) {
            FI = F[i][0];
            j_points = JuliaDrawingArea::calc_points(FI, xc, yc);
            cr->move_to(std::get<0>(j_points), std::get<1>(j_points));

            for (int j = 1; j < (int)F[i].size(); j++) {
                FI = F[i][j];
                j_points = JuliaDrawingArea::calc_points(FI, xc, yc);
                cr->line_to(std::get<0>(j_points), std::get<1>(j_points));
            }
            cr->stroke();
        }
    } else {
        // draw julia set pixel by pixel

        // step 1: determine behavior of critical points; do they escape, or do they
        // approach a periodic orbit?
        std::array<cpx, 2> C = critical_points(P, Q);
        std::vector<cpx> attracting_orbits;
        cpx z;
        bool converge_to_orbit;
        attracting_orbits.clear(); // initialize
        cr->set_line_width(1.0);
        for (int i = 0; i < 2; i++) {
            // for each critical point
            z = C[i];
            converge_to_orbit = true;
            for (int j = 0; j < 400; j++) {
                // iterate z -> f(z) until escape or periodic
                z = eval(P, Q, z);
                if (abs(z) > 5.0) { // escape
                    converge_to_orbit = false;
                    j = 400;
                }
            }

            if (converge_to_orbit) {
                attracting_orbits.push_back(z);
            }
        }

        // step 2: for each point in the drawing area, compute time either to escape
        // or to approach periodic orbit of critical point.
        int maxiter = 60;
        std::array<double, 3> H;
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                double xx = (double)(i - xc) / (double)(xc);
                double yy = (double)(j - yc) / (double)(yc);
                z = xx + I * yy; // initial value
                z = z * 2.0; // scale for window
                for (int k = 0; k < maxiter; k++) {
                    z = eval(P, Q, z);
                    if (abs(z) > 5.0) { // escape
                        H = color_code(k, maxiter, 0);
                        cr->set_source_rgb(H[0], H[1], H[2]);
                        cr->move_to(i, j);
                        cr->line_to(i + 1, j);
                        cr->stroke();
                        break;
                    } else {
                        for (int l = 0; l < (int)attracting_orbits.size(); l++) {
                            if (abs(z - attracting_orbits[l]) < 0.01) {
                                // converge to attracting orbit
                                H = color_code(k, maxiter, attracting_orbits.size());
                                cr->set_source_rgb(H[0], H[1], H[2]);
                                cr->move_to(i, j);
                                cr->line_to(i + 1, j);
                                cr->stroke();
                                k = maxiter;
                                l = 2;
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}

void JuliaDrawingArea::construct_drawing(int i, int xc, int yc, std::vector<cpx> attracting_orbits, std::promise<std::map<julia_pixels, color_value>>&& p, int width)
{
    color_value H;
    int maxiter = 60;
    cpx z;
    std::map<julia_pixels, color_value> drawing_point_promises;
    int orbit_size = (int)attracting_orbits.size();

    for (int j = 0; j < width; j++) {
        cpx xx = (double)(i - xc) / (double)(xc);
        cpx yy = (double)(j - yc) / (double)(yc);
        z = xx + I * yy; // initial value
        z = z * 2.0; // scale for window
        julia_pixels julia = std::make_tuple(i, j);

        for (int k = 0; k < maxiter; k++) {
            z = eval(P, Q, z);
            if (abs(z) > 5.0) { // escape
                H = color_code(k, maxiter, 0);
                drawing_point_promises.emplace(julia, H);
                break;
            } else {
                for (int l = 0; l < orbit_size; l++) {
                    if (abs(z - attracting_orbits[l]) < 0.01) {
                        // converge to attracting orbit
                        H = color_code(k, maxiter, attracting_orbits.size());
                        drawing_point_promises.emplace(julia, H);
                        break;
                    }
                }
            }
        }
    }
    p.set_value(drawing_point_promises);
}
