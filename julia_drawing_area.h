#pragma once

#include <cairomm/context.h>
#include <gtkmm.h>

#include "polynomial.h"

class JuliaDrawingArea : public Gtk::DrawingArea {
public:
    JuliaDrawingArea();
    void on_p_set();

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    bool on_button_press(GdkEventButton* event);
    bool on_point_set(const Cairo::RefPtr<Cairo::Context>& cr);

private:
    int width, height;
    int input_x, input_y;
    bool green_mode;
};