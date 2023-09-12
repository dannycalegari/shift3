/* julia_drawing_area.cc */

#include <iostream>

#include "green.h"
#include "julia_drawing_area.h"
#include "polynomial.h"

JuliaDrawingArea::JuliaDrawingArea() {
    add_events(Gdk::BUTTON_PRESS_MASK);
    signal_draw().connect(sigc::mem_fun(*this, &JuliaDrawingArea::on_draw));
    signal_button_press_event().connect(sigc::mem_fun(*this, &JuliaDrawingArea::on_button_press));
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

void JuliaDrawingArea::on_p_set() {
    queue_draw();
}

bool JuliaDrawingArea::on_button_press(GdkEventButton* event) {
    on_p_set();
    green_mode=1-green_mode;
    return true;
}

bool JuliaDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    int xc, yc, i, j, k, l, a, b;
    double xx,yy,r;
    xc = width / 2;
    yc = height / 2;

	if(green_mode==true){
		// draw gradient flowlines of green function
		
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
	} else {
		// draw julia set pixel by pixel
		
		// step 1: determine behavior of critical points; do they escape, or do they
		// approach a periodic orbit?
		std::array<cpx, 2> C = critical_points(P,Q);
		std::vector<cpx> attracting_orbits;
		cpx z;
		bool converge_to_orbit;
		attracting_orbits.clear();	// initialize
	   	cr->set_line_width(1.0);
		for(i=0;i<2;i++){
			// for each critical point
			z=C[i];
			converge_to_orbit=true;
			for(j=0;j<400;j++){
				// iterate z -> f(z) until escape or periodic
				z=eval(P,Q,z);
				if(abs(z)>5.0){	// escape
					converge_to_orbit=false;
					j=400;
				};
			};
			if(converge_to_orbit){
				std::cout << "critical point " << i << " converged to " << z << "\n";
				// add value to vector of attracting orbits
				attracting_orbits.push_back(z);
				// maybe output period and multiplier?
			};
		};
		
		// step 2: for each point in the drawing area, compute time either to escape
		// or to approach periodic orbit of critical point.
		
		for(i=0;i<width;i++){
			for(j=0;j<height;j++){
				xx = (double) (i-xc) / (double) (xc);
				yy = (double) (j-yc) / (double) (yc);
				z=xx+I*yy;	// initial value
				z=z*2.0;	// scale for window
				converge_to_orbit=false;
				for(k=0;k<50;k++){
					z=eval(P,Q,z);
					if(abs(z)>5.0){ // escape
						// draw a gray dot at (i,j) which is darker the smaller k is
						converge_to_orbit=true;
					    cr->set_source_rgb(k/20.0, k/20.0, k/20.0);	
						cr->move_to(i,j);
						cr->line_to(i+1,j);
						cr->stroke();
						k=50;
					} else {
						for(l=0;l<attracting_orbits.size();l++){
							if(abs(z-attracting_orbits[l])<0.01){
								converge_to_orbit=true;
								if(l==0){
									 cr->set_source_rgb(k/50.0, 0.0, 1.0);	
								} else {
									 cr->set_source_rgb(0.0, k/50.0, 1.0);	
								};
								// draw a dot at (i,j) in color l which is darker the smaller k is
								cr->move_to(i,j);
								cr->line_to(i+1,j);
								cr->stroke();
								k=50;
							};
						};
					};
				};
				if(converge_to_orbit==false){
		//			std::cout << "didn't converge: " << xx+I*yy << " goes to " << z << "\n";
				};
			};
		};
	};

	return true;
}


