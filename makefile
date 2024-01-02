CXX = g++ -g -Wall
CXXFLAGS = -std=c++11 -pthread -I. `pkg-config --cflags gtkmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0` -pthread

SRCDIR = src
BUILDDIR = build

SOURCES = main.cc shift_application.cc elamination_drawing_area.cc pq_drawing_area.cc julia_drawing_area.cc polynomial.cc green.cc lamination.cc
OBJECTS = $(SOURCES:%.cc=$(BUILDDIR)/%.o)
EXECUTABLE = shift3

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(EXECUTABLE)

.PHONY: all clean
