#pragma once

#include <cairomm/context.h>
#include <gtkmm.h>

#include "polynomial.h"

class PQDrawingArea : public Gtk::DrawingArea {
public:
    PQDrawingArea();
    virtual ~PQDrawingArea();
    void on_p_set();
    bool p_type;

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    bool on_point_set(const Cairo::RefPtr<Cairo::Context>& cr);

private:
    int width, height;
    int input_x, input_y;
};
