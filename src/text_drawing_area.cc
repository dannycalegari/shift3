#include "text_drawing_area.h"
#include "shift_drawing_area.h"
#include "lamination.h"
#include "polynomial.h"
#include "green.h"

TextDrawingArea::TextDrawingArea() {
    add_events(Gdk::BUTTON_PRESS_MASK);
    // green_mode = false;
    // has_input_point = false;
    signal_draw().connect(sigc::mem_fun(*this, &TextDrawingArea::on_draw));
    signal_button_press_event().connect(sigc::mem_fun(*this, &TextDrawingArea::on_button_press));
}


bool TextDrawingArea::on_point_set(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gdk::Rectangle allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    const int lesser = std::min(width, height);

    return true;
}


bool TextDrawingArea::on_button_press(GdkEventButton* event) {
    return true;
}

void TextDrawingArea::on_p_set() {
    queue_draw();
}

bool TextDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    
    std::string S;
    
	cr->set_line_width(1.0);
	cr->set_source_rgb(0.0, 0.0, 0.0);
//	Cairo::FtScaledFont::font_extents_t fe;
	cr->set_font_size(25);
//	cr->select_font_face("Purisa", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
//	cr->get_font_extents(&fe);
	cr->move_to(20,55);
	cr->show_text("p value: ");
	S = std::to_string(P.real());
	cr->show_text(S);
	cr->show_text(" + ");
	S = std::to_string(P.imag());
	cr->show_text(S);
	cr->show_text(" i");
	cr->stroke();
	cr->move_to(500,55);
	cr->show_text("q value: ");
	S = std::to_string(Q.real());
	cr->show_text(S);
	cr->show_text(" + ");
	S = std::to_string(Q.imag());
	cr->show_text(S);
	cr->show_text(" i");
	cr->stroke();	
    return true;
}
