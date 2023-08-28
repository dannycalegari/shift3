#pragma once

#include <gtkmm.h>
#include "shift_drawing_area.h"

class ShiftApplication : public Gtk::Application {
public:
    static Glib::RefPtr<ShiftApplication> create(int& argc, char**& argv);

protected:
    ShiftApplication();

private:
    void on_activate() override;

    Gtk::Window* window;
    Gtk::Grid* grid;
    ShiftDrawingArea* p_square;
    ShiftDrawingArea* q_square;
    ShiftDrawingArea* julia_square;
    ShiftDrawingArea* elamination_square;
};