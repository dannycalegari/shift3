/* pq_drawing_area.cc */

#include <iostream>

#include "green.h"
#include "polynomial.h"
#include "pq_drawing_area.h"


PQDrawingArea::PQDrawingArea() {
    add_events(Gdk::BUTTON_PRESS_MASK);
    signal_draw().connect(sigc::mem_fun(*this, &PQDrawingArea::on_draw));
}

PQDrawingArea::~PQDrawingArea() { }


bool PQDrawingArea::on_point_set(const Cairo::RefPtr<Cairo::Context>& cr) {
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

void PQDrawingArea::on_p_set() {
    queue_draw();
}

bool PQDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
        int xc, yc, i, j, k, l;
        double xx, yy, ll, a, b, c;
        cpx w, z;
		std::array<cpx, 2> C; 
		bool converge_to_orbit;

        xc = width / 2;
        yc = height / 2;
        cr->set_line_width(1.0);
        
        for(i=0;i<width;i++){
        	for(j=0;j<height;j++){
				xx = (double) (i-xc) / (double) (xc);
				yy = (double) (j-yc) / (double) (yc);
				w=xx+I*yy;	// initial value
				w=w*2.0;	// scale for window
				if(p_type==true){
	        		C = critical_points(w,Q);
	        	} else {
	        		C = critical_points(P,w);
	        	};
        		for(k=0;k<2;k++){
        			z = C[k];	// initial value
        			converge_to_orbit=true;
        			for(l=0;l<20;l++){
        				if(p_type==true){
        					z = eval(w,Q,z);
        				} else {
        					z = eval(P,w,z);
        				};
        				if(abs(z)>5.0){	// escape
        					ll = (double) l;
							converge_to_orbit=false;
							if(k==0){
								a = 0.8*ll/20.0;
								b = 0.3*ll/20.0;
								c = 0.5*ll/20.0;
							} else {
								a = a + 0.2*ll/20.0;
								b = b + 0.7*ll/20.0;
								c = c + 0.5*ll/20.0;
							};
							l=20;
						};
        			};
        			if(converge_to_orbit==true){
        				if(k==0){
	      					a = 0.8;
    	   					b = 0.3;
        					c = 0.5;
        				} else {
        					a = a + 0.2;
        					b = b + 0.7;
        					c = c + 0.5;
        				};
        			};
        		};
        		cr->set_source_rgb(a,b,c);
        		cr->move_to(i,j);
    			cr->line_to(i+1,j);
    			cr->stroke();
        	};
        };


    return true;
}
