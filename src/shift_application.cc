#include "shift_application.h"

/*
    -----------------------------------------------------
    |   P   |       Julia       |       Elamination     |
    |       |                   |                       |
    --------|                   |                       |
    |   Q   |                   |                       |
    |       |                   |                       |
    -----------------------------------------------------
*/


Glib::RefPtr<ShiftApplication> ShiftApplication::create(int& argc, char**& argv) {
    return Glib::RefPtr<ShiftApplication>(new ShiftApplication());
}

ShiftApplication::ShiftApplication() : Gtk::Application("org.example") {}

void ShiftApplication::on_activate() {
    window = new Gtk::Window();
    p_square = new ShiftDrawingArea();
    q_square = new ShiftDrawingArea();
    julia_square = new ShiftDrawingArea();
    elamination_square = new ShiftDrawingArea();

    p_square->set_size_request(250, 250);
    q_square->set_size_request(250, 250);
    julia_square->set_size_request(500, 500);
    elamination_square->set_size_request(500, 500);

    Gtk::Grid* grid = Gtk::manage(new Gtk::Grid());
    window->add(*grid);
    grid->attach(*p_square, 0, 0, 1, 1);
    grid->attach(*q_square, 0, 1, 1, 1);
    grid->attach(*julia_square, 1, 0, 2, 2);
    grid->attach(*elamination_square, 3, 0, 2, 2);
	grid->show_all();

    window->set_default_size(1250, 500);
    add_window(*window);
    window->show_all();
}
