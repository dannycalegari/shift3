/* julia_drawing_area.cc */

#include <iostream>

#include "green.h"
#include "julia_drawing_area.h"
#include "polynomial.h"

JuliaDrawingArea::JuliaDrawingArea() {
    add_events(Gdk::BUTTON_PRESS_MASK);
    green_mode = false;
    signal_draw().connect(sigc::mem_fun(*this, &JuliaDrawingArea::on_draw));
    signal_button_press_event().connect(sigc::mem_fun(*this, &JuliaDrawingArea::on_button_press));
}

void JuliaDrawingArea::set_green_mode(bool green) {
    green_mode = green;
    queue_draw();
}

bool JuliaDrawingArea::on_point_set(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gdk::Rectangle allocation = get_allocation();
    width = allocation.get_width();
    height = allocation.get_height();
    const int lesser = std::min(width, height);

    cr->save();
    cr->arc(input_x, input_y, lesser / 4.0, 0.0, 2.0 * M_PI); // full circle
    cr->set_source_rgba(0.0, 0.0, 0.8, 0.6);    // partially translucent
    cr->fill_preserve();
    cr->restore();  // back to opaque black
    cr->stroke();

    return true;
}

void JuliaDrawingArea::on_pq_set() {
    queue_draw();
}

bool JuliaDrawingArea::on_button_press(GdkEventButton* event) {
    // on_pq_set();
    return true;
}

bool JuliaDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    int xc, yc, i, j, a, b;
    double xx,yy,r;
    xc = width / 2;
    yc = height / 2;

    std::vector<std::vector<cpx>> F = Julia_green(P,Q);		// should be Julia_green(p,q)
    cr->set_line_width(1.0);
    cr->set_source_rgb(0.5, 0.0, 0.0);

	for(i=0;i<F.size();i++){
		xx = F[i][0].real()/2.0;
		yy = F[i][0].imag()/2.0;
		r = sqrt(xx*xx + yy*yy);
		if(r>3.0){
			xx=xx*3.0/r;
			yy=yy*3.0/r;
		};
		a = xc+xc*xx;
		b = yc+yc*yy;
		cr->move_to(a,b);
		for(j=1;j<F[i].size();j++){
			xx = F[i][j].real()/2.0;
			yy = F[i][j].imag()/2.0;
			r = sqrt(xx*xx + yy*yy);
			if(r>3.0){
				xx=xx*3.0/r;
				yy=yy*3.0/r;
			};
			a = xc+xc*xx;
			b = yc+yc*yy;
			cr->line_to(a,b);
		};
		cr->stroke();
	};

	return true;
}


