/* mandelbrot.cc */


void draw_mandelbrot(){
	// draw mandelbrot set
	// critical points at 2.0 and w
	// P' = (z-2)(z-w) = 3z^2 -3(2+w)z + 6w
	// P = z^3 - 3/2(2+w)z^2 + 6wz + 0
		
	int xi,yi;
	double x,y;
	cpx z,w;
	int i,ii,J,a,b,c;
	polynomial P = { 1.0, 0.0, 2.0, 1.0};
	cvec C;
	long col;
	
	x=-4.0;
	for(xi=0;xi<800;xi++){
		y=-4.0;
		for(yi=0;yi<800;yi++){
			z = x+y*I;
			P[2]=z;		// P = z^3 + pz + 1
			/*
			P[1]=-1.5*(2.0+z);
			P[2]=6.0*z;
			P[3]=0.0;*/
			C=critical_points(P);
	//		cout << C[0] << " " << C[1] << "\n";
			w=C[0];
			ii=0;
			for(i=0;i<255;i++){
				w=eval(P,w);
				ii=i;
				if(abs(w)>100.0){
					i=255;
				};
			};
			w=C[1];
			J=0;
			for(i=0;i<255;i++){
				w=eval(P,w);
				J=i;
				if(abs(w)>100.0){
					i=255;
				};
			};
	//		cout << z << " " << ii << " " << J << "\n";
			a=16*ii+J;
			b=4*ii+4*J;
			c=ii+16*J;
			if(a>255) { 
				a=255;
			};
			if(b>255){ 
				b=255;
			};
			if(c>255){
				c=255;
			};
			col=0x010000*a + 0x000100*b + 0x000001*c;
			draw_point(xi,yi,col);
			y=y+0.01;
		};
		x=x+0.01;
	};
};

void draw_mandelbrot_slice(cpx q, cpx p_cen, double width, bool write_to_file, string S){
	/* draw mandelbrot set
	draws slice of the cubic shift locus
	polynomials P = z^2 + pz + q for q fixed
	and p in the box of sidelength 2*width
	centered at p_cen */
	
		
	int xi,yi;
	double x,y;
	cpx z,w;
	int i,ii,J,a,b,c;
	polynomial P = { 1.0, 0.0, p_cen, q};
	cvec C;
	long col;
	ofstream my_file;
	
	if(write_to_file){
		my_file.open(S);
		my_file << "P3 \n";
		my_file << "800 800 \n";
		my_file << "255 \n";
		my_file << "# pixmap output from program shift3 \n";
	};
	
	x=p_cen.real()-width;
	for(xi=0;xi<800;xi++){
		y=p_cen.imag()-width;
		for(yi=0;yi<800;yi++){
			z = x+y*I;
			P[2]=z;	
			C=critical_points(P);
			w=C[0];
			ii=0;
			for(i=0;i<255;i++){
				w=eval(P,w);
				ii=i;
				if(abs(w)>100.0){
					i=255;
				};
			};
			w=C[1];
			J=0;
			for(i=0;i<255;i++){
				w=eval(P,w);
				J=i;
				if(abs(w)>100.0){
					i=255;
				};
			};
			if(ii>J){
				J=ii;
			};
			b=J;
			a=J*J;
			c=3*J*J;
	//		b=J*2;
	//		a=J*J*2;
	//		c=J*J*J;
			if(a>255) { 
				a=255;
			};
			if(b>255){ 
				b=255;
			};
			if(c>255){
				c=255;
			};
			col=0x010000*a + 0x000100*b + 0x000001*c;
			if(write_to_file){
				my_file << a << " " << b << " " << c << "\n";
			};
			draw_point(xi,yi,col);
			y=y+width/400.0;
		};
		x=x+width/400.0;
	};
	if(write_to_file){
		my_file.close();
	};
};


