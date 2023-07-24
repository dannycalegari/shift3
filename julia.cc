/* julia.cc */

void draw_julia(polynomial P, cpx o, double w, int wi, int xo, int yo){
	/* draw julia set centered at o with width 2w
	in box on the screen with width wi and x/y offsets xo, yo */
	int xi,yi;
	double x,y,ww;
	cpx z,zz;
	int i,j,k,a,b,c;
	long col;
	cvec critical_points;
	cvec attracting_orbits;
	polynomial Q;
	bool bounded;
	
	Q=derivative(P);
	critical_points = roots(Q);
	attracting_orbits.clear();	// initialize
	
	for(i=0;i<critical_points.size();i++){
		// for each root
		z= critical_points[i];
		bounded=true;
		for(j=0;j<255;j++){
			z=eval(P,z);
			if(abs(z)>100.0){
				j=255;
				bounded=false;
			};
			for(k=0;k<attracting_orbits.size();k++){
				if(abs(z-attracting_orbits[k])<0.0001){
					// if we've seen this attracting orbit before
					j=255;
					bounded=false;
				};
			};
		};
		if(bounded==true){
			// genuinely new attracting orbit
			attracting_orbits.push_back(z);
	//		cout << "attracting orbit : " << z.real() << " + " << z.imag() << " i\n";
	//		cout << "attracting orbit image : " << (eval(P,z)).real() << " + << " << (eval(P,z)).imag() << " i\n";
		};
	};
	
	
	for(xi=0;xi<2*wi;xi++){
		for(yi=0;yi<2*wi;yi++){
			ww = (double) wi;
			x = (double) (xi-wi);
			x = w*x/ww;
			y = (double) (yi-wi);
			y = w*y/ww;
			z = o + x + I*y;
						
			for(i=0;i<256;i++){
				z=eval(P,z);
				if(abs(z)>100.0){
					a=32*i;
					if(a>255) 
						{ 
						a=255;
					};
					b=2*i;
					if(b>255){ 
						b=255;
					};
					c=i;
					col=0x010000*c + 0x000100*b + 0x000001*a;
					draw_point(xo+xi,yo+yi,col);
					i=256;
				};
				for(j=0;j<attracting_orbits.size();j++){
					zz=attracting_orbits[j];
					if(abs(z-zz)<0.0001){
						a=4*i;
						if(a>255) 
							{ 
							a=255;
						};
						b=2*i;
						if(b>255){ 
							b=255;
						};
						c=i;
						col=0x010000*a + 0x000100*c + 0x000001*b;
						if(j==1){
							col=0x010000*b + 0x000100*a + 0x000001*c;
						};
						draw_point(xo+xi,yo+yi,col);
						i=256;		
						j=attracting_orbits.size();			
					};
				};
			};
		};
	};
};

void draw_julia_dots(polynomial P, cpx z, int depth, int draw){

	cpx w;
	cvec r;
	polynomial Q;
	int i;
	
	if(depth>0){
			Q=P;
			Q[Q.size()-1]=Q[Q.size()-1]-z;
			r=roots(Q);
		if(draw>0){
			// find preimage
			w=r[0];		
			draw_julia_dots(P,w,depth-1,draw-1);
		} else {
			for(i=0;i<r.size();i++){
				w=r[i];
				
				draw_point(w.real(),w.imag(),0x000000);
				draw_julia_dots(P,w,depth-1,0);
			};
		};
	};
};


