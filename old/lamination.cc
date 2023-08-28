/* lamination.cc */

struct leaf {
	double height;
	double angle;	// should be in the interval [0,1)
	double offset;	// should really be rational; in (0,1/2]
};

leaf leaf_from_angles(double alpha, double beta, double h){
	leaf L;
	double off;
	
	L.height=h;
	while(alpha>=1.0){
		alpha=alpha-1.0;
	};
	while(alpha<0.0){
		alpha=alpha+1.0;
	};
	while(beta>=1.0){
		beta=beta-1.0;
	};
	while(beta<0.0){
		beta=beta+1.0;
	};
	off=beta-alpha;
	while(off>0.5){
		off=off-1.0;
	};
	while(off<-0.5){
		off=off+1.0;
	};
	
	// alpha, beta in [0,1] and off in [-0.5,0.5]
	if(off<0.0){
		L.angle=beta;
		L.offset=-1.0*off;
	} else {
		L.angle=alpha;
		L.offset=off;
	};
	return(L);
};

bool link(leaf l, leaf m){
	// two leaves link if their chords intersect
	// equivalently, if the triangles l0,l1,m0 and l0,l1,m1 have opposite orientations
	double x1,x2,y1,y2;
	double a1,a2,b1,b2;
	double o1,o2;
	
	x1=cos(l.angle*TWOPI);
	y1=sin(l.angle*TWOPI);
	x2=cos((l.angle+l.offset)*TWOPI);
	y2=sin((l.angle+l.offset)*TWOPI);
	
	a1=cos(m.angle*TWOPI);
	b1=sin(m.angle*TWOPI);
	a2=cos((m.angle+m.offset)*TWOPI);
	b2=sin((m.angle+m.offset)*TWOPI);
	
	o1=((x2-x1)*(b1-y1))-((y2-y1)*(a1-x1));
	o2=((x2-x1)*(b2-y1))-((y2-y1)*(a2-x1));
	if(o1*o2<0.0){
		return(true);
	} else {
		return(false);
	};
};

point v_to_p(double X, double Y, int wid, int xoff, int yoff){
	point p;
	double dwid;
	dwid = (double) wid;
	p.x = xoff + (int) ((X*dwid/4.0)+dwid/2.0);
	p.y = yoff + (int) ((Y*dwid/4.0)+dwid/2.0);
	return(p);
};

int r_to_i(double R,int wid){
	int i;
	double dwid;
	dwid = (double) wid;
	i = (int) (R*dwid/4.0);
	return(i);
};

void draw_edge(double X1, double Y1, double X2, double Y2, int wid, int xoff, int yoff){
	point p1,p2;
	p1=v_to_p(X1,Y1,wid,xoff,yoff);
	p2=v_to_p(X2,Y2,wid,xoff,yoff);
    XSetForeground(display, gc, 0x000000);
    XSetLineAttributes(display, gc, 1, LineSolid, 1, 1);
    XDrawLine(display, win, gc, p1.x, p1.y, p2.x, p2.y);	
};

void draw_arc(double X, double Y, double R, double alpha, double beta, int wid, int xoff, int yoff){
	point p;
	int r;
	int A,B;
	p=v_to_p(X,Y,wid,xoff,yoff);
	r=r_to_i(R,wid);
	A = (23040.0*alpha/TWOPI);
	B = (23040.0*beta/TWOPI);
    XSetForeground(display, gc, 0x000000);
    XSetLineAttributes(display, gc, 1, LineSolid, 1, 1);
    XSetFillStyle(display, gc, FillSolid);
    XDrawArc(display, win, gc, p.x-r, p.y-r, 2*r, 2*r, A, B);
};


void draw_leaf(leaf l, int wid, int xoff, int yoff){ 
	// 
	
	double alpha, theta, x, y, r, a, b, h;
	bool normalized;
	theta=TWOPI*l.angle;
	if(theta<0.0 || theta>TWOPI){
		cerr << "angle theta " << l.angle << " out of bounds! \n";
	};
	alpha=TWOPI*l.offset;
	if(alpha<0.0 || alpha>PI){
		cerr << "offset alpha " << l.offset << " out of bounds! \n";
	};
	h=l.height;
	if(h<1.0){
		cerr << "height " << l.height << " out of bounds! \n";
	};
//	cout << "angle theta " << theta << " offset alpha " << alpha << " height " << h << "\n";
	if(l.offset==0.5){	// diagonal
		
	
		draw_edge(cos(theta),-1.0*sin(theta),cos(theta+PI),-1.0*sin(theta+PI), wid, xoff, yoff);
	} else {
		x=cos(theta)-sin(theta)*tan(alpha/2.0);
		y=-1.0*(sin(theta)+cos(theta)*tan(alpha/2.0));
//		draw_arc(x,y,0.05,0.0,TWOPI);
		r=tan(alpha/2.0);
		a=(PI/2.0)+alpha+theta;	// a is the initial point on the arc
		normalized=false;
		while(normalized==false){
			if(a>TWOPI){
				a=a-TWOPI;
			} else if(a<0.0){
				a=a+TWOPI;
			} else {
				normalized=true;
			};
		};
		b=PI-alpha;		// b is the arclength
		draw_arc(x,y,r,a,b,wid,xoff,yoff);
	};
	draw_edge(cos(theta),-1.0*sin(theta),h*cos(theta),-1.0*h*sin(theta),wid,xoff,yoff);
	draw_edge(cos(theta+alpha),-1.0*sin(theta+alpha),h*cos(theta+alpha),-1.0*h*sin(theta+alpha),wid,xoff,yoff);
};

vector<leaf> dynamical_lamination(int degree, int depth, vector<leaf> C){
	// given a vector of critical leaves C, generates the associated dynamical elamination
	if(degree!=C.size()+1){
		cerr << "degree should be one more than the number of critical leaves! \n";
		return(C);
	};
	
	int i,j;
	double k,d,l,m,theta,alpha;
	d = (double) degree;
	vector<leaf> U,V,W;
	leaf L;
	bool unlinked;
	
	U=C;	// depth 0 leaves are the critical leaves
	V=C;
	for(i=1;i<=depth;i++){
		// at every stage V is the depth-1 leaves
	//	cout << "generating leaves at stage " << i << "\n";
		W.clear();
	//	h=exp(log(h)/d);

		for(j=0;j<V.size();j++){
		// generate new family W from V
	//		cout << "leaf " << j << " from stage " << i-1 << "\n";
			for(k=0.0;k<d-0.1;k=k+1.0){
	//			cout << "child " << k << "\n";
				theta=(V[j].angle+k)/d;
	//			cout << "angle theta is " << theta << "\n";
				L.angle=theta;
				L.height=exp(log(V[j].height)/d);
				for(l=0.0;l<d-0.1;l=l+1.0){
					alpha=(V[j].offset+l)/d;	// guess for alpha
	//				cout << "offset " << alpha << "\n";
					L.offset=alpha;
					unlinked=true;
					for(m=0;m<C.size();m++){
						if(link(L,C[m])){	// does this guess for L.offset link C?
							unlinked=false;
						};
					};
					if(unlinked==true){
						if(L.offset>0.5){
							L.angle=L.angle+L.offset;
							L.offset=1.0-L.offset;
						};
						if(L.angle>1.0){
							L.angle=L.angle-1.0;
						};
						W.push_back(L);	// add it to the list!
	//					cout << "unlinked! \n";
						l=d;
					} else {
	//					cout << "linked \n";
					};
				};
			};
		};
		// append W to U
		U.insert(U.end(), W.begin(), W.end());
		// set V=W
		V=W;
	};
	
//	cout << "generated lamination; " << U.size() << " leaves \n";
	return(U);
};

void draw_lamination(vector<leaf> L, int wid, int xoff, int yoff){
	draw_arc(0.0,0.0,1.0,0.0,TWOPI,wid,xoff,yoff);
	int i;
	for(i=0;i<L.size();i++){
		draw_leaf(L[i],wid,xoff,yoff);
	//	cout << "drew leaf " << i << "\n";
	};
};
