#pragma once

#include <cairomm/context.h>
#include <gtkmm.h>

class TextDrawingArea : public Gtk::DrawingArea {
public:
    TextDrawingArea();
	void on_p_set();

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    void on_draw_requested(const Glib::ustring& area_name, int x, int y);
    bool on_button_press(GdkEventButton* event);
    bool on_point_set(const Cairo::RefPtr<Cairo::Context>& cr);
};