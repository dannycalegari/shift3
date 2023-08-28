CXX = g++
CXXFLAGS = -std=c++17 -I. `pkg-config --cflags gtkmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0`

SRCDIR = src
BUILDDIR = build

SOURCES = main.cc shift_application.cc shift_drawing_area.cc
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
