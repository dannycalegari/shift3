/*	qtshift.cc	

	Copyright 2023 by Danny Calegari and Meg Ford
	
Informal description 

Libraries to include: 

	standard math
	complex numbers
	vectors
	complex polynomials (root finding up to degree 3)
	qt graphics 
	
Structures defined in other files:

	leaf is a pair of complex numbers with the same absolute value (which is > 1) and 
		different arguments
	
	LT is a 2x2 matrix of complex numbers
	

Functions defined in other files:

	void draw_p_slice(complex p, complex q)
	
		draws slice of cubic mandelbrot set with fixed p, and a "dot" at q
	
	void draw_q_slice(complex p, complex q)
	
		draws slice of cubic mandelbrot set with fixed q, and a "dot" at p
		
	void draw_julia(complex p, complex q, bool green_mode)
	
		draws julia set of polynomial f(p,q)(z):= z^3 + p*z + q
			if green_mode == false pixels are colored by attracting cycle with shading
		depending on time to convergence
			if green_mode == true draws gradient flow lines of greens function 
		
	leaf* compute_bottcher_coordinates(complex p, complex q)
	
		computes critical leaves b[0], b[1] associated to critical points c[0], c[1] of
		polynomial f (i.e. c[0], c[1] = +/- sqrt(-p/3))
	
	void draw_elamination(leaf b[0], leaf b[1])
	
		draws degree 3 elamination associated to pair of critical leaves
		
	LT inverse_derivative_bottcher(p,q)
	
		computes the derivative of the inverse of the map (p,q) -> (b[0],b[1])
		as a 2x2 complex matrix

Description of the program

	1. Initialization: launches graphics environment and opens a window 
		1250 pixels (horizontal) x 500 pixels (vertical)
		
	This is subdivided into 4 squares 
	
		1a. p-square 250 x 250 in top left
		1b. q-square 250 x 250 in bottom left
		1c. julia square 500 x 500 to the immediate right of the p- and q-squares
		1d. elamination square 500 x 500 to the far right

	p and q are initialized to 0 and 0 (say) and green_mode is initialized to false
	
	2. Interface loop: Alternate between Drawing Phase and Event Phase
	
		Drawing Phase:
	
			draw_p_slice(p,q) in p-square
			draw_q_slice(p,q) in q-square
			draw_julia(p,q,green_mode) in julia square
			b compute_bottcher_coordinates(p,q)
			draw_elamination(b[0],b[1]) in elamination square
			
		Event Phase:
		
			get event; this could be either a mouse click, arrow key, or asdwgq key
			case: event is mouse click in p-square
				adjust p-value
			case: event is mouse click in q-square
				adjust q-value
			case: event is arrow key or asdw keys
				using the function inverse_derivative_bottcher(p,q),
				adjust p and q values so as to deform b[0], b[1] in a predictable way
				(eg left-right adjusts argument of b[0], up-down adjusts absolute value
				of b[0], a-d adjusts argument of b[1], w-s adjusts absolute value of b[1])
			case: event is [g] key
				toggle value of green_mode
			case: event is [q] key
				close windows and exit program

*/