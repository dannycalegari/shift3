#include "shift_drawing_area.h"
#include "lamination.h"
#include "polynomial.h"

ShiftDrawingArea::ShiftDrawingArea() {
    add_events(Gdk::BUTTON_PRESS_MASK);
    // green_mode = false;
    // has_input_point = false;
    signal_draw().connect(sigc::mem_fun(*this, &ShiftDrawingArea::on_draw));
    signal_button_press_event().connect(sigc::mem_fun(*this, &ShiftDrawingArea::on_button_press));
}

// void ShiftDrawingArea::set_green_mode(bool green) {
//     green_mode = green;
//     queue_draw();
// }

bool ShiftDrawingArea::on_point_set(const Cairo::RefPtr<Cairo::Context>& cr) {
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

void ShiftDrawingArea::set_input_point(int x, int y) {
    // has_input_point = true;
    input_x = x;
    input_y = y;
    queue_draw();
}

bool ShiftDrawingArea::on_button_press(GdkEventButton* event) {
    set_input_point(static_cast<int>(event->x), static_cast<int>(event->y));
    return true;
}

bool ShiftDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    // if(has_input_point) {
    //     on_point_set(cr);
    // } else {
        // coordinates for the center of the window
        int xc, yc, i, a, b;
        double xx,yy;
        xc = width / 2;
        yc = height / 2;
        std::array<leaf,2> C;	// critical leaves
        C[0].height=1.7;
        C[0].angle[0]=0.1;
        C[0].angle[1]=0.1+TWOPI/3.0;
        
        C[1].height=1.5;
        C[1].angle[0]=0.1-1.0*PI;
        C[1].angle[1]=0.1-1.0*PI+TWOPI/3.0;
        
        std::vector<leaf> LL;
        LL = dynamical_lamination(5, C);

        cr->set_line_width(1.0);
        cr->set_source_rgb(0.0, 0.0, 0.0);

        
        // draw leaves of lamination
 
        for(i=0;i<LL.size();i++){
        	a=xc+0.5*xc*cos(LL[i].angle[0])*LL[i].height;
        	b=yc+0.5*yc*sin(LL[i].angle[0])*LL[i].height;
        	cr->move_to(a,b);        
        	a=xc+0.5*xc*cos(LL[i].angle[0]);
        	b=yc+0.5*yc*sin(LL[i].angle[0]);
        	cr->line_to(a,b);
        	
        	// at the moment: draws straight chord joining points on unit circle.
        	// to do: draw arc of perpendicular circle joining points on unit circle.
        	
         	a=xc+0.5*xc*cos(LL[i].angle[1]);
        	b=yc+0.5*yc*sin(LL[i].angle[1]);  
        	cr->line_to(a,b);
        	
        	
          	a=xc+0.5*xc*cos(LL[i].angle[1])*LL[i].height;
        	b=yc+0.5*yc*sin(LL[i].angle[1])*LL[i].height;  
        	cr->line_to(a,b);       	
        	cr->stroke();     	
        };

    // }

    return true;
}
