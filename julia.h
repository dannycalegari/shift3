#ifndef GTKMM_JULIA_DRAWING_H
#define GTKMM_JULIA_DRAWING_H

#include <gtkmm/drawingarea.h>

class JuliaDrawing : public Gtk::DrawingArea
{
public:
  JuliaDrawing();
  virtual ~JuliaDrawing();

protected:
  //Override default signal handler:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};

#endif // GTKMM_JULIA_DRAWING_H