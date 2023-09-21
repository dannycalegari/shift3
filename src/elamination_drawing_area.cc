#include "elamination_drawing_area.h"
#include "green.h"
#include "lamination.h"
#include "polynomial.h"


ElaminationDrawingArea::ElaminationDrawingArea() {
    add_events(Gdk::BUTTON_PRESS_MASK);
    signal_draw().connect(sigc::mem_fun(*this, &ElaminationDrawingArea::on_draw));
    signal_button_press_event().connect(sigc::mem_fun(*this, &ElaminationDrawingArea::on_button_press));
}

bool ElaminationDrawingArea::on_point_set(const Cairo::RefPtr<Cairo::Context>& cr) {
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

void ElaminationDrawingArea::set_input_point(int x, int y) {
    input_x = x;
    input_y = y;
    queue_draw();
}

bool ElaminationDrawingArea::on_button_press(GdkEventButton* event) {
    set_input_point(static_cast<int>(event->x), static_cast<int>(event->y));
    return true;
}

void ElaminationDrawingArea::on_pq_set() {
    queue_draw();
}

bool ElaminationDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    int xc, yc, i, a, b, c;
    double xx,yy,x0,y0,r,rr;
    xc = width / 2;
    yc = height / 2;
    std::array<leaf,2> C;	// critical leaves
    C = critical_bottcher_coordinates(P,Q);

    if(C[0].height==1.0 || C[1].height==1.0) {
        cr->set_line_width(1.0);
        cr->set_source_rgb(0.0, 0.0, 0.0);
        cr->arc(xc, yc, 0.5*xc,0.0, 2.0*M_PI); // unit circle
        cr->stroke();
    } else {

        std::vector<leaf> LL;
        LL = dynamical_lamination(5, C);

        cr->set_line_width(1.0);
        cr->set_source_rgb(0.0, 0.0, 0.0);


        // draw leaves of lamination

        cr->arc(xc, yc, 0.5*xc,0.0, 2.0*M_PI);	// set clip region
        cr->clip();								// to unit circle

        for(i=0; i<LL.size(); i++) {
        	// draw circular chords as circles clipped to the unit circle
       		x0 = cos(LL[i].angle[0]);
        	y0 = sin(LL[i].angle[0]);
        	xx = 0.5*(cos(LL[i].angle[0])+cos(LL[i].angle[1]));
            yy = 0.5*(sin(LL[i].angle[0])+sin(LL[i].angle[1]));
            r = xx*xx+yy*yy;
            if(r > 0.01) {
	            xx = xx/r;
    	        yy = yy/r;
        	    rr = sqrt((xx-x0)*(xx-x0)+(yy-y0)*(yy-y0));
           		a = xc + 0.5*xc*xx;
            	b = yc + 0.5*yc*yy;
            	c = 0.5*xc*rr;

            	cr->arc(a,b,c,0.0, 2.0 * M_PI);
            	cr->stroke();
            } else {
                a=xc+0.5*xc*cos(LL[i].angle[0]);
        		b=yc+0.5*yc*sin(LL[i].angle[0]);
            	cr->move_to(a,b);
                a=xc+0.5*xc*cos(LL[i].angle[1]);
        		b=yc+0.5*yc*sin(LL[i].angle[1]);
            	cr->line_to(a,b);
            	cr->stroke();
            };
        };

 		cr->reset_clip();
        cr->arc(xc, yc, 0.5*xc, 0.0, 2.0*M_PI); // unit circle
		cr->stroke();

        for(i=0; i<LL.size(); i++){
        	// draw peripheral segments
        	a=xc+0.5*xc*cos(LL[i].angle[0])*LL[i].height;
        	b=yc+0.5*yc*sin(LL[i].angle[0])*LL[i].height;
        	cr->move_to(a,b);
        	a=xc+0.5*xc*cos(LL[i].angle[0]);
        	b=yc+0.5*yc*sin(LL[i].angle[0]);
        	cr->line_to(a,b);
  			cr->stroke();

           	a=xc+0.5*xc*cos(LL[i].angle[1]);
        	b=yc+0.5*yc*sin(LL[i].angle[1]);
        	cr->move_to(a,b);
          	a=xc+0.5*xc*cos(LL[i].angle[1])*LL[i].height;
        	b=yc+0.5*yc*sin(LL[i].angle[1])*LL[i].height;
        	cr->line_to(a,b);
        	cr->stroke();
        };

	};

    return true;
}
