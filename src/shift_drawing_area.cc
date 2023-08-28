#include "shift_drawing_area.h"

ShiftDrawingArea::ShiftDrawingArea() {
    add_events(Gdk::BUTTON_PRESS_MASK);
    green_mode = false;
    p = 0;
    q = 0;
    has_input_point = false;
    signal_draw().connect(sigc::mem_fun(*this, &ShiftDrawingArea::on_draw));
    signal_button_press_event().connect(sigc::mem_fun(*this, &ShiftDrawingArea::on_button_press));
}

void ShiftDrawingArea::set_green_mode(bool green) {
    green_mode = green;
    queue_draw();
}

bool ShiftDrawingArea::on_point_set(const Cairo::RefPtr<Cairo::Context>& cr) {
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

void ShiftDrawingArea::set_input_point(int x, int y) {
    has_input_point = true;
    input_x = x;
    input_y = y;
    queue_draw();
}

bool ShiftDrawingArea::on_button_press(GdkEventButton* event) {
    set_input_point(static_cast<int>(event->x), static_cast<int>(event->y));
    return true;
}



bool ShiftDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    if(has_input_point) {
        on_point_set(cr);
    } else {
        // coordinates for the center of the window
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
    }

    return true;
}




