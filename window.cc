#include "julia.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>


int main(int argc, char *argv[])
{
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");
	Gtk::Window window;
	window.set_default_size(200, 200);
	window.set_title("Julia Set");
  
	JuliaDrawing jDraw;
	window.add(jDraw);
	jDraw.show();

  return app->run(window);
}