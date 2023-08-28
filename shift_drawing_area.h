#pragma once

#include <gtkmm.h>
#include <cairomm/context.h>

class ShiftDrawingArea : public Gtk::DrawingArea {
public:
    ShiftDrawingArea();

    void set_green_mode(bool green);
    void set_input_point(int x, int y);

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    bool on_button_press(GdkEventButton* event);
    bool on_point_set(const Cairo::RefPtr<Cairo::Context>& cr);

private:
    bool green_mode;
    int p, q;
    int width, height;
    bool has_input_point;
    int input_x, input_y;
};