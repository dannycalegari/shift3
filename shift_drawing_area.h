#pragma once

#include <cairomm/context.h>
#include <gtkmm.h>

class ShiftDrawingArea : public Gtk::DrawingArea {
public:
    ShiftDrawingArea();
    void set_input_point(int x, int y);
	void on_p_set();

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    void on_draw_requested(const Glib::ustring& area_name, int x, int y);
    bool on_button_press(GdkEventButton* event);
    bool on_point_set(const Cairo::RefPtr<Cairo::Context>& cr);
    int p, q;
    int width, height;
    int input_x, input_y;
};