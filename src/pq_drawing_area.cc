/* pq_drawing_area.cc */

#include <iostream>

#include "green.h"
#include "polynomial.h"
#include "pq_drawing_area.h"

PQDrawingArea::PQDrawingArea()
{
    add_events(Gdk::BUTTON_PRESS_MASK);
    signal_draw().connect(sigc::mem_fun(*this, &PQDrawingArea::on_draw));
}

PQDrawingArea::~PQDrawingArea()
{

}

bool PQDrawingArea::on_point_set(const Cairo::RefPtr<Cairo::Context>& cr)
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

void PQDrawingArea::on_pq_set()
{
    queue_draw();
}

bool PQDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    int xc, yc, i, j, k, l, maxiter, cumulative_iter;
    double xx, yy;
    cpx w, z, zz;
    std::array<cpx, 2> C;
    int convergent_orbits;
    int converge_to_orbit;
    std::array<double, 3> H;

    maxiter = 100;
    xc = width / 2;
    yc = height / 2;
    cr->set_line_width(1.0);

    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            xx = (double)(i - xc) / (double)(xc);
            yy = (double)(j - yc) / (double)(yc);
            w = xx + I * yy; // initial value
            w = w * zoom; // scale for window
            if (p_type == true) {
                w = w + P_center; // translate for window
                C = critical_points(w, Q);
            } else {
                w = w + Q_center; // translate for window
                C = critical_points(P, w);
            };
            cumulative_iter = 0; // initial value
            convergent_orbits = 0; // initial value
            for (k = 0; k < 2; k++) {
                z = C[k]; // initial value
                zz = C[k];
                converge_to_orbit = 2;
                for (l = 0; l < maxiter; l++) {
                    if (p_type == true) {
                        z = eval(w, Q, z);
                    } else {
                        z = eval(P, w, z);
                    };
                    if (l % 2 == 1) {
                        if (p_type == true) {
                            zz = eval(w, Q, zz);
                        } else {
                            zz = eval(P, w, zz);
                        };
                        if (abs(z - zz) < 0.001) {
                            //			std::cout << "convergence! " << l << "\n";
                            converge_to_orbit = 1;
                            cumulative_iter = cumulative_iter + l;
                            convergent_orbits++;
                            l = maxiter;
                        };
                    };
                    if (abs(z) > 5.0) { // escape
                        converge_to_orbit = 0;
                        cumulative_iter = cumulative_iter + l;
                        l = maxiter;
                    };
                };
                if (converge_to_orbit == 2) {
                    cumulative_iter = cumulative_iter + l;
                    convergent_orbits++;
                };
            };
            H = color_code(cumulative_iter, maxiter * 2, convergent_orbits);
            cr->set_source_rgb(H[0], H[1], H[2]);
            cr->move_to(i, j);
            cr->line_to(i + 1, j);
            cr->stroke();
        };
    };

    return true;
}
