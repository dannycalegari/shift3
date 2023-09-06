/* pq_drawing_area.cc */

#include <iostream>

#include "green.h"
#include "polynomial.h"
#include "pq_drawing_area.h"


PQDrawingArea::PQDrawingArea() {
    add_events(Gdk::BUTTON_PRESS_MASK);
    signal_draw().connect(sigc::mem_fun(*this, &PQDrawingArea::on_draw));
}

PQDrawingArea::~PQDrawingArea() { }


bool PQDrawingArea::on_point_set(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gdk::Rectangle allocation = get_allocation();
    width = allocation.get_width();
    height = allocation.get_height();
    const int lesser = std::min(width, height);

    cr->save();
    cr->arc(input_x, input_y, lesser / 4.0, 0.0, 2.0 * M_PI); // full circle
    cr->set_source_rgba(0.0, 0.0, 0.8, 0.6);    // partially translucent
    cr->fill_preserve();
    cr->restore();  // back to opaque black
    cr->stroke();

    return true;
}

void PQDrawingArea::on_p_set() {
    queue_draw();
}

bool PQDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
        int xc, yc;
        xc = width / 2;
        yc = height / 2;
        cr->set_line_width(10.0);

        // draw red lines out from the center of the window
        cr->set_source_rgb(0.8, 0.0, 0.0);
        cr->move_to(0, 0);
        cr->line_to(xc, yc);
        cr->line_to(0, height);
        cr->move_to(xc, yc);
        cr->line_to(width, yc);
        cr->stroke();

    return true;
}
