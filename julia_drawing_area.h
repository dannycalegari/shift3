#pragma once

#include <cairomm/context.h>
#include <future>
#include <gtkmm.h>
#include <map>
#include <tuple>

#include "polynomial.h"

class JuliaDrawingArea : public Gtk::DrawingArea {
public:
    JuliaDrawingArea();
    virtual ~JuliaDrawingArea();
    void on_pq_set();
    void set_green_mode(bool green);

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    bool on_button_press(GdkEventButton* event);
    bool on_point_set(const Cairo::RefPtr<Cairo::Context>& cr);
    julia_pixels calc_points(cpx FI, int xc, int yc);
    void construct_drawing(int i, int xc, int yc, std::vector<cpx> attracting_orbits, std::promise<std::map<julia_pixels, color_value>>&& p, int width);

private:
    bool green_mode;
    int width, height;
    int input_x, input_y;
    std::map<julia_pixels, color_value> drawing_points;
};