/* julia_drawing_area.cc */

#include <iostream>
#include <thread>
#include <future>
// #include <utility>
// #include <vector>
// #include <tuple>

#include "green.h"
#include "julia_drawing_area.h"
// #include "polynomial.h"


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
    on_pq_set();
    green_mode=1-green_mode;
    return true;
}

std::tuple JuliaDrawingArea::calc_points(std::vector<std::vector<cpx>> F, int i):
	xx = F[i][0].real()/2.0;
	yy = F[i][0].imag()/2.0;
	r = sqrt(xx*xx + yy*yy);

	if(r > 3.0) {
		xx = xx * 3.0/r;
		yy = yy * 3.0/r;
	};
	a = xc+xc*xx;
	b = yc+yc*yy;
	return make_tuple(a,b)


bool JuliaDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gtk::Allocation allocation = get_allocation();
	int width = 0;
	int height = 0;
	if(allocation.get_width() > 0 && allocation.get_height() > 0) {
		width = allocation.get_width();
		height = allocation.get_height();
	}

    int xc, yc, i, j, k, l, a, b;
    double xx,yy,r;
    xc = width / 2;
    yc = height / 2;
	std::array<double,3> H;
	int maxiter;


	if(green_mode==true) {
		// draw gradient flowlines of green function

 		std::vector<std::vector<cpx>> F = Julia_green(P,Q);		// should be Julia_green(p,q)
   		cr->set_line_width(1.0);
    	cr->set_source_rgb(0.5, 0.0, 0.0);

		for(int i = 0; i <F.size(); i++) {
			std::tuple points = JuliaDrawingArea::calc_points(F, i);
			cr->move_to(points.first,points.second);
			for(int j=1 ; j<F[i].size(); j++){
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
		//		std::cout << "critical point " << i << " converged to " << z << "\n";
		//		int period;
		//		cpx multiplier;
		//		compute_period_and_multiplier(P, Q, z, 0.00001, period, multiplier);
		//		std::cout << "period: " << period << " ; multiplier: " << multiplier << " ; absolute value: " << abs(multiplier) << "\n";
				// add value to vector of attracting orbits
				attracting_orbits.push_back(z);
			}

		}

		/* step 2: for each point in the drawing area, compute time either to escape
		or to approach periodic orbit of critical point. */

		// Create threads
		std::vector<std::pair<std::thread, std::future<std::map<julia_pixels, color_value>>>> threads;
		std::cout << threads.max_size();
		for(int i=0; i<height; i++){
			std::promise<std::map<julia_pixels, color_value>> prom;
			std::future<std::map<julia_pixels, color_value>> futo = prom.get_future();
			std::thread th(&JuliaDrawingArea::construct_drawing, this, i, xc, yc, attracting_orbits, std::move(prom), width);
			threads.push_back(make_pair(std::move(th), std::move(futo)));
		}

		// Get results from threads
		for(auto& th : threads){
			auto t = move(th.first);
			auto prom_result = move(th.second);
			std::map<julia_pixels, color_value> results = prom_result.get();
			for ( const auto &iter : results ) {
				julia_pixels k =  iter.first;
				color_value v = iter.second;
				drawing_points.emplace(k, v);
    		}
			t.join();
		}

		for(int w = 0; w < height; w++){
			for(int j = 0; j < width; j++){
				color_value H = drawing_points[std::make_tuple(i,j)];
				cr->set_source_rgb(H[0], H[1], H[2]);
				cr->move_to(i, j);
				cr->line_to(i+1, j);
				cr->stroke();
			}
		}
	}
	threads.clear();
	return true;
}


void JuliaDrawingArea::construct_drawing(int i, int xc, int yc, std::vector<cpx> attracting_orbits, std::promise<std::map<julia_pixels, color_value>>&& p, int width) {
	color_value H;
	int maxiter = 60;
	cpx z;
	std::map<julia_pixels, color_value> drawing_point_promises;

	for(int j = 0; j < width; j++) {
		cpx xx = (double) (i-xc) / (double) (xc);
		cpx yy = (double) (j-yc) / (double) (yc);
		z = xx + I * yy;	// initial value
		z = z * 2.0;	// scale for window
		julia_pixels  julia = std::make_tuple(i, j);
		for(int k  = 0; k < maxiter; k++) {
			z = eval(P, Q, z);
			if(abs(z) > 5.0) { // escape
				H = color_code(k, maxiter, 0);
				drawing_point_promises.emplace(julia, H);
				std::cout << "assiugning vaLUE" << k <<"\n";
				break;
			} else {
				for(int l = 0; l <attracting_orbits.size(); l++) {
					if(abs(z - attracting_orbits[l]) < 0.01) {
						// converge to attracting orbit
						H = color_code(k, maxiter, attracting_orbits.size());
						drawing_point_promises.emplace(julia, H);
						std::cout << "assiugning vaLUE 2" << k << "\n";
						break;
					}
				}
			}
		}
	}
	p.set_value(drawing_point_promises);
}



