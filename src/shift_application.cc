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

ShiftApplication::ShiftApplication() : Gtk::Application("org.example") {
}

void ShiftApplication::on_activate() {
    window = new Gtk::Window();
    p_area = new PQDrawingArea();
    q_area = new PQDrawingArea();
    julia_area = new JuliaDrawingArea();
    elamination_area = new ShiftDrawingArea();

    p_area->set_name("p_area");
    q_area->set_name("q_area");
    julia_area->set_name("julia_area");
    elamination_area->set_name("elamination_area");

    p_area->set_size_request(250, 250);
    q_area->set_size_request(250, 250);
    julia_area->set_size_request(500, 500);
    elamination_area->set_size_request(500, 500);

    Gtk::Grid* grid = Gtk::manage(new Gtk::Grid());
    window->add(*grid);
    grid->attach(*p_area, 0, 0, 1, 1);
    grid->attach(*q_area, 0, 1, 1, 1);
    grid->attach(*julia_area, 1, 0, 2, 2);
    grid->attach(*elamination_area, 3, 0, 2, 2);
	grid->show_all();

    window->set_default_size(1250, 500);
    add_window(*window);
    window->show_all();
    p_area->signal_button_press_event().connect(sigc::mem_fun(*this, &ShiftApplication::on_button_press_p));
    std::cout << "P " << P << std::endl;
    q_area->signal_button_press_event().connect(sigc::mem_fun(*this, &ShiftApplication::on_button_press_q));
}

bool ShiftApplication::on_button_press_p(GdkEventButton* event) {
    std::cerr << "P " << P << "ughgh" << std::endl;
    P = static_cast<cpx>(static_cast<double>(event->x), static_cast<double>(event->y));
    std::cerr << "PQ " << static_cast<double>(event->x) << "ughgh" << static_cast<double>(event->y) << std::endl;
    std::cerr << "P " << P << "ughgh" << std::endl;
    julia_area->on_p_set();
    return true;
}

bool ShiftApplication::on_button_press_q(GdkEventButton* event) {
    std::cerr << "Q " << Q << "ughgh" << std::endl;
    P = static_cast<cpx>(static_cast<double>(event->x), static_cast<double>(event->y));
    std::cerr << "Q " << static_cast<double>(event->x) << "ughgh" << static_cast<double>(event->y) << std::endl;
    std::cerr << "q " << Q << "ughgh" << std::endl;
    julia_area->on_p_set();
    return true;
}