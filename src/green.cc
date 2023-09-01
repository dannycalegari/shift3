/* green.cc

bottcher coordinates for cubic depressed polynomial

also routine to draw green lines

*/
#pragma once
#include "polynomial.h"
#include "green.h"



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

std::vector<std::vector<cpx>> Julia_green(cpx p, cpx q){
	/* returns approximate green gradient flowlines (ie lines whose bottcher coordinates
	have constant argument) for cubic polynomial z^3 + p*z + q. Along the way we get
	an estimate of the Bottcher coordinates of the critical points; probably this is
	faster than the method above, so some future version of this program should return
	these coordinates. */

	std::vector<std::vector<cpx>> flow_collection;
	std::vector<cpx> flow_segment, flow_segment_2;
	cpx a, b, b1, b2, z;
	std::array<cpx, 3> c;
	double L,LL;	// large real number, eg 100

	L=100.0;
	LL=L*L*L;

	flow_collection.clear();

	/* initializing flow segment */

	flow_segment.clear();

	flow_segment.push_back(LL);
	c = preimage(p,q,LL);	// array of 3 preimages

	b = newton_preimage(p,q,LL,L);	// b is preimage closest to L

	// b1 and b2 are the other two preimages

	if(abs(b-c[0])<0.01){
		b1=c[1];
		b2=c[2];
	} else if(abs(b-c[1])<0.01){
		b1=c[0];
		b2=c[2];
	} else {
		b1=c[0];
		b2=c[1];
	};

	/* initial segment is a straight line consisting of 10 segments
	subdivided logarithmically from LL to b */

	double S,T;

	for(S=0.9;S>=0.0;S=S-0.1){
		T=exp(log(L)*exp(S*log(3.0)));	// from 100 to 1000000
		z=b+((T-L)/(LL-L))*(LL-b);
		flow_segment.push_back(z);
	};

	/* continuing flow segment by inverse image,
	obtained by Newton's method for continuity */

	int i,j;
	j=10;	// initial value

	for(i=0;i<500;i++){
		z=newton_preimage(p,q,flow_segment[j-9],flow_segment[j]);
		flow_segment.push_back(z);
		j++;
		if(abs(flow_segment[j-1]-flow_segment[j])<0.0001){	// keep going until segment is small
			i=500;
		};
	};

	flow_collection.push_back(flow_segment);	// add segment to the collection

	/* compute two other preimages of initial flow_segment */

	flow_segment_2.clear();
	L=abs(b1);
	LL=L*L*L;
	for(S=1.0;S>=0.0;S=S-0.1){
		T=exp(log(L)*exp(S*log(3.0)));	// from 100 to 1000000
		z=L+((T-L)/(LL-L))*(LL-L);
		z=z*exp(I*arg(b1));
		flow_segment_2.push_back(z);
	};

	j=1;	// initial value
	for(i=1;i<flow_segment.size()-10;i++){
		z=newton_preimage(p,q,flow_segment[j],flow_segment_2[10+j-1]);
		flow_segment_2.push_back(z);
		j++;
	};

	flow_collection.push_back(flow_segment_2);	// add segment to the collection

	flow_segment_2.clear();
	L=abs(b2);
	LL=L*L*L;
	for(S=1.0;S>=0.0;S=S-0.1){
		T=exp(log(L)*exp(S*log(3.0)));	// from 100 to 1000000
		z=L+((T-L)/(LL-L))*(LL-L);
		z=z*exp(I*arg(b2));
		flow_segment_2.push_back(z);
	};

	j=1;	// initial value
	for(i=1;i<flow_segment.size()-10;i++){
		z=newton_preimage(p,q,flow_segment[j],flow_segment_2[10+j-1]);
		flow_segment_2.push_back(z);
		j++;
	};

	flow_collection.push_back(flow_segment_2);	// add segment to the collection

	/* recursively add all preimages of last added flow segments */

	int l,k;

	for(l=1;l<81;l++){
		c=preimage(p,q,flow_collection[l][0]);
		for(k=0;k<3;k++){
			flow_segment.clear();

			if(l<9){
				L=abs(c[k]);
				LL=L*L*L;
				for(S=1.0;S>=0.0;S=S-0.1){
					T=exp(log(L)*exp(S*log(3.0)));	// from 100 to 1000000
					z=L+((T-L)/(LL-L))*(LL-L);
					z=z*exp(I*arg(c[k]));
					flow_segment.push_back(z);
				};

				j=1;	// initial value
				for(i=1;i<flow_collection[l].size()-10;i++){
					z=newton_preimage(p,q,flow_collection[l][j],flow_segment[10+j-1]);
					flow_segment.push_back(z);
					j++;
				};
			} else {
				flow_segment.push_back(c[k]);
				j=1;
				for(i=1;i<flow_collection[l].size()-10;i++){
					z=newton_preimage(p,q,flow_collection[l][j],flow_segment[j-1]);
					flow_segment.push_back(z);
					j++;
				};
			};

			flow_collection.push_back(flow_segment);
		};
	};


	return(flow_collection);

	// draw routine
	// draw line segment flow_segment point by point
	// (only the part of the segment that appears on the screen, i.e. with
	// absolute value < 2 or so
	//
	// TO DO
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
