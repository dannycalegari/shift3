/* green.cc */

double green(polynomial P, cpx z){
	/* returns exponential of Green function; 
	well-defined iff z is in
	the attracting basin of infinity. 
	This is the absolute value of the
	bottcher coordinate. */
	int i,maxiter;
	cpx w;
	double T,d,maxsize;
	
	d=(double) degree(P);
	w=z;
	maxiter=255;
	maxsize=1000.0;
	for(i=0;i<maxiter;i++){
		w=eval(P,w);
		if(abs(w)>maxsize){
	//		cout << "f^" << i+1 << "(c)=" << w << "\n";
			T=log(abs(w));
			return(exp(T/pow(d,i+1)));
		};
	};
	
	return(1.0);
};

cpx bottcher_coordinate(polynomial P, cpx z){
	/* returns the bottcher coordinate of z;
	well-defined iff z is in the attracting
	domain of infinity and not on a segment 
	tipped by a critical point. */
	
	double maxsize,alpha,beta;
	int i,maxiter,d;
	cpx w,ww,wd,zz,y;
	bool escaped;
	escaped=false;
	
	maxiter=255;
	maxsize=100.0;
	w=z;
	
	for(i=0;i<maxiter;i++){
		w=eval(P,w);
		if(abs(w)>maxsize){
	//		cout << "f^" << i+1 << "(c)=" << w << "\n";
			d=i+1;
			escaped=true;
			i=maxiter;
		};
	};
	
	if(escaped==false){
		return(1.0);
	};
	
	
	// f^d(z)=w;
	alpha=arg(w);

//	cout << "target argument: " << alpha << "\n";

	zz=z;
	while(abs(zz)<maxsize){

		w=eval_iterate(P,zz,d);
		ww=eval_iterate(P,zz+0.0001,d);
		wd=(ww-w)/0.0001;	// approximate derivative of w(z)
		
		/* head in the direction of y
			which has abs(w')=abs(w)*1.01 and
			arg(w')=alpha */
			
		y=abs(w)*1.0001*exp(I*alpha);
		zz=zz+((y-w)/wd);
		
	//	cout << abs(zz) << " , " << arg(zz) << "\n";
	//	cout << "wd: " << wd << "\n";
		
		if(abs(w)>maxsize*maxsize){
			d=d-1;
			w=eval_iterate(P,zz,d);
			alpha=arg(w);
		};
	};
	
	beta=arg(zz);
	
	/*
	cout << "seed gamma is " << gamma << "\n";
	cout << "beta is " << beta << "\n";
	cout << "beta times " << degree(P) << "^" << d << "=" << beta*pow(degree(P),d) << "\n";
	cout << w << "\n";
	*/
	w=green(P,z)*exp(I*beta);
//	cout << abs(w) << " , " << arg(w) << "\n";
	return(w);
};

// vector<leaf>

vector<leaf> critical_bottcher_coordinates(polynomial P){
	vector<leaf> CL;
	leaf L;
	CL.clear();
	
	cvec C=critical_points(P);
	int i,j,d;
	cpx z,w,bw,bww,wd,bwd,y,yy;
	cpx sec_der;
	double alpha;
	vector<double> angle_list;
	bool escaped;
	int maxiter;
	double maxsize;

	
	for(i=0;i<C.size();i++){
		z=C[i];
		angle_list.clear();
		w=eval(P,z);	// critical value
		escaped=false;
	
		maxiter=255;
		maxsize=10000.0;

	
		for(j=0;j<maxiter;j++){
			w=eval(P,w);
			if(abs(w)>maxsize){
	//		cout << "f^" << i+1 << "(c)=" << w << "\n";
				d=j+1;
				escaped=true;
				j=maxiter;
			};
		};
		w=eval(P,z); // reset critical value
		bw=eval_iterate(P,w,d);
		bww=eval_iterate(P,w+0.0001,d);
		bwd=(bww-bw)/0.0001;
		y=abs(bw)*1.001*exp(I*arg(bw));
		wd=(y-bw)/bwd;	// direction to perturb w to make f^d(w) increase radially
		
//		cout << "w = " << w << "\n";
//		cout << "wd = " << wd << "\n";
		
		sec_der=eval(derivative(derivative(P)),z);	// f''(z)
		
		yy=sqrt(wd*2.0/sec_der);
//		cout << "yy = " << yy << "\n";
//		cout << "z+yy = " << z+yy << " f(c+yy) = " << eval(P,z+yy) << " versus " << "w+wd = " << w+wd << "\n";
		
		w=z+yy;	// perturbation of critical point
		alpha=arg(bottcher_coordinate(P,w));
		angle_list.push_back(alpha);
		w=z-yy; // opposite perturbation of critical point
		alpha=arg(bottcher_coordinate(P,w));
		angle_list.push_back(alpha);
		
//		cout << "critical point " << i << " bottcher coords abs = " << green(P,z) << " arg = " << angle_list[0] << " , " << angle_list[1] << "\n";
		L.height = green(P,z);
		L.angle = angle_list[0]/TWOPI;
		L.offset = (angle_list[1]-angle_list[0])/TWOPI;
		
		if(L.angle<0.0){
			L.angle=L.angle+1.0;
		};
		if(L.offset<0.0){
			L.offset=L.offset+1.0;
		};
		if(L.offset>0.5){
			L.angle=L.angle+L.offset;
			L.offset=1.0-L.offset;
		};
		L.offset=1.0/3.0;
		if(L.angle>1.0){
			L.angle=L.angle-1.0;
		};
//		cout << "critical point " << i << " height = " << L.height << " angle = " << L.angle << " offset = " << L.offset << "\n";
		CL.push_back(L);
	};
	return(CL);
};

