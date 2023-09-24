#include "shift_application.h"
#include <cairomm/context.h>
#include <gtkmm.h>
#include <chrono>
#include <thread>

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
	interpolate_mode = false;
}

void ShiftApplication::on_activate() {
    window = new Gtk::Window();
    window->add_events(Gdk::KEY_PRESS_MASK);
    window->signal_key_press_event().connect(sigc::mem_fun(*this, &ShiftApplication::on_key_press));
    p_area = new PQDrawingArea();
    q_area = new PQDrawingArea();
    p_area->p_type = true;
    q_area->p_type = false;
    julia_area = new JuliaDrawingArea();
    elamination_area = new ElaminationDrawingArea();

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
    q_area->signal_button_press_event().connect(sigc::mem_fun(*this, &ShiftApplication::on_button_press_q));
}

bool ShiftApplication::on_button_press_p(GdkEventButton* event) {
    double a,b;
    cpx u,v;
    int i;
    
	a = static_cast<double>(event->x);
	b = static_cast<double>(event->y);
	
	if(interpolate_mode){
	
		std::cout << "interpolate mode on! \n";
		u = P;
		v = P_center + zoom*(a-125.0)/125.0 + zoom*(b-125.0)*I/125.0;
		for(i=0;i<=100;i++){
			P = (v*((double) i) + u*((double) 100-i))/100.0;
 	//		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
 			julia_area->on_pq_set();
		};
		P = v;
		P_center = P;
		
	} else {
		P = P_center + zoom*(a-125.0)/125.0 + zoom*(b-125.0)*I/125.0;
		P_center = P;
		julia_area->on_pq_set();
	};
    std::string p_val = "p value: " + std::to_string(P.real()) + " + " + std::to_string(P.imag()) + " i";
	std::string q_val = "q value: " + std::to_string(Q.real()) + " + " + std::to_string(Q.imag()) + " i";
	std::string zoom_val = "zoom: " + std::to_string(zoom);
    std::string title = p_val + " ; " + q_val + " ; " + zoom_val;
    window->set_title(title);
    window->show_all();
//    julia_area->on_pq_set();
    p_area->on_pq_set();
    q_area->on_pq_set();
    elamination_area->on_pq_set();
    return true;
}

bool ShiftApplication::on_button_press_q(GdkEventButton* event) {
    double a,b;
	a = static_cast<double>(event->x);
	b = static_cast<double>(event->y);
	Q = Q_center + zoom*(a-125.0)/125.0 + zoom*(b-125.0)*I/125.0;
	Q_center = Q;
    std::string p_val = "p value: " + std::to_string(P.real()) + " + " + std::to_string(P.imag()) + " i";
	std::string q_val = "q value: " + std::to_string(Q.real()) + " + " + std::to_string(Q.imag()) + " i";
	std::string zoom_val = "zoom: " + std::to_string(zoom);
    std::string title = p_val + " ; " + q_val + " ; " + zoom_val;
    window->set_title(title);
    window->show_all();
    julia_area->on_pq_set();
    p_area->on_pq_set();
    q_area->on_pq_set();
    elamination_area->on_pq_set();
    return true;
}

bool ShiftApplication::on_key_press(GdkEventKey* event) {	
	if(event->keyval == GDK_KEY_z){
		zoom = zoom*1.1;
	} else if(event->keyval == GDK_KEY_x){
		zoom = zoom/1.1;
	} else if(event->keyval == GDK_KEY_q){ 	
		this->quit();		
	} else if(event->keyval == GDK_KEY_i){ 	
		interpolate_mode = 1-interpolate_mode;
	} 	 	
	std::string p_val = "p value: " + std::to_string(P.real()) + " + " + std::to_string(P.imag()) + " i";
	std::string q_val = "q value: " + std::to_string(Q.real()) + " + " + std::to_string(Q.imag()) + " i";
	std::string zoom_val = "zoom: " + std::to_string(zoom);
    std::string title = p_val + " ; " + q_val + " ; " + zoom_val;
    window->set_title(title);
    window->show_all();
    p_area->on_pq_set();
    q_area->on_pq_set();
    return true;
}

