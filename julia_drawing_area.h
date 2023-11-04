#pragma once

#include <map>
#include <future>
#include <tuple>

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
    std::tuple calc_points(std::vector<std::vector<cpx>> F, int i)
    void construct_drawing(int i, int xc, int yc, std::vector<cpx> attracting_orbits, std::promise<std::map<julia_pixels, color_value>>&& p, int width);

private:
    bool green_mode;
    int width, height;
    int input_x, input_y;
    std::map<julia_pixels, color_value> drawing_points;
};