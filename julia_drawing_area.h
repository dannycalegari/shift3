#pragma once

#include <cairomm/context.h>
#include <gtkmm.h>

#include "polynomial.h"

class JuliaDrawingArea : public Gtk::DrawingArea {
public:
    JuliaDrawingArea();
    void on_pq_set();
    void set_green_mode(bool green);

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    bool on_button_press(GdkEventButton* event);
    bool on_point_set(const Cairo::RefPtr<Cairo::Context>& cr);

private:
    bool green_mode;
    int width, height;
    int input_x, input_y;
};