/* green.cc 

bottcher coordinates for cubic depressed polynomial

also routine to draw green lines

*/

double pow(double d, int i){
	// returns d^i
	if(i==0){
		return(1.0);
	} else {
		return(d*pow(d,i-1));
	};
};


cpx bottcher(cpx p, cpx q, cpx z){
	/* returns bottcher coordinate of z for the polynomial
	z^3 + z*p + q
	
	Well-defined iff z is in the attracting basin of infinity;
	otherwise by default return 1.0;
	*/
	
	int i, escape_iterate, max_iterate;
	cpx w,b;
	double T,b_abs,b_arg, maxsize;
	bool escapes;
	
	maxsize=1000.0;
	max_iterate=20;
	w=z;
	escapes=false;
	
	// determine absolute value b_abs of bottcher coordinate
	
	for(i=0;i<max_iterate;i++){
		w=eval(p,q,w);
		if(abs(w)>maxsize){
			T=log(abs(w));
			b_abs=exp(T/pow(3.0,i+1));
			escapes=true;
			escape_iterate=i+1;
			i=max_iterate;	// escape loop; is there a more elegant way?
		};
	};
	if(escapes==false){
		return(1.0);
	};
	
	// determine argument b_arg of bottcher coordinate
	
	double alpha;
	cpx ww,wd,zz,y;
	
	alpha=arg(w);	// argument of f^escape_iterate(z)

	zz=z;
	while(abs(zz)<maxsize){

		w=eval_iterate(p,q,zz,escape_iterate);
		ww=eval_iterate(p,q,zz+0.0001,escape_iterate);
		wd=(ww-w)/0.0001;	// approximate derivative of w(z)
		
		/* head in the direction of y
			which has abs(w')=abs(w)*1.01 and
			arg(w')=alpha */
			
		y=abs(w)*1.0001*exp(I*alpha);
		zz=zz+((y-w)/wd);
		
		
		if(abs(w)>maxsize*maxsize){
			escape_iterate=escape_iterate-1;
			w=eval_iterate(p,q,zz,escape_iterate);
			alpha=arg(w);
		};
	};
	
	b_arg=arg(zz);
	
	w=b_abs*exp(I*b_arg);
	return(w);
};

/*

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

*/
