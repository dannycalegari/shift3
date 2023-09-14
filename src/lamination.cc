/* lamination.cc */
#include <iostream>

#include "polynomial.h"
#include "lamination.h"



leaf leaf_from_angles(double alpha, double beta, double h){
	leaf L;
	L.height=h;
	L.angle[0]=alpha;
	L.angle[1]=beta;
	return(L);
};

cpx zee(leaf l,int i){
	cpx w;
	w=l.height*exp(I*l.angle[i]);
	return(w);
};

bool link(leaf l, leaf m){
	// two leaves link if their chords intersect
	// equivalently, if the triangles l0,l1,m0 and l0,l1,m1 have opposite orientations
	cpx z[2],w[2],a,b;
	z[0]=zee(l,0)/l.height;
	z[1]=zee(l,1)/l.height;
	w[0]=zee(m,0)/m.height;
	w[1]=zee(m,1)/m.height;
	
	a=(z[1]-z[0])/(w[0]-z[0]);
	b=(z[1]-z[0])/(w[1]-z[0]);
	if(a.imag()*b.imag()>0){
		return(false);	// unlinked
	} else {
		return(true);
	};
};

double normalize_angle(double theta){
	double x,y,phi;
	x=cos(theta);
	y=sin(theta);
	phi=atan2(y,x);
	return(phi);
};

std::array<leaf,3> preimage(leaf L){
	std::array<leaf,3> M;
	int i;
	double t;
	t=exp(log(L.height)/3.0);
	
	for(i=0;i<3;i++){
		M[i].height = t;
		M[i].angle[0] = normalize_angle((L.angle[0]/3.0)+ ((double) i)*TWOPI/3.0);
		M[i].angle[1] = normalize_angle((L.angle[1]/3.0)+ ((double) i)*TWOPI/3.0);
	};
	return(M);
};

std::vector<leaf> dynamical_lamination(int depth, std::array<leaf,2> C){
	// given two critical leaves C, generates the associated dynamical elamination
	

	
	cpx a,b;
	a = zee(C[0],0)/zee(C[0],1);
	b = zee(C[1],0)/zee(C[1],1);
		
	// generating preimages
	
	std::vector<leaf> L,LL,M;
	std::array<leaf,3> N;
	double t;
	bool found_preimage;
	int i,j;
	
	L.clear();
	LL.clear();
	M.clear();
	
	// sanity check
	
	if(link(C[0],C[1])){
		std::cout << "problem! critical leaves are linked! \n";
		return(M);
	} else {
		std::cout << "critical leaves unlinked. \n";
	};
	
	// put critical leaves on list
	
	L.push_back(C[0]);
	L.push_back(C[1]);
	M=L;
	for(i=0;i<depth;i++){
		for(j=0;j<L.size();j++){
			// for each element L[j] of L
			// generate three preimages that don't link C[0] or C[1] and add them to LL	

			N=preimage(L[j]);
			
			found_preimage=false;
			while(found_preimage==false){
				if(link(N[0],C[0])==false && link(N[0],C[1])==false){
					LL.push_back(N[0]);	// add N[0]
					
					if(link(N[1],C[0])==false && link(N[1],C[1])==false){
						LL.push_back(N[1]);
						LL.push_back(N[2]);
						found_preimage=true;
					} else {
						// switch second angle
						t=N[2].angle[1];
						N[2].angle[1]=N[1].angle[1];
						N[1].angle[1]=t;
						LL.push_back(N[1]);
						LL.push_back(N[2]);
						found_preimage=true;
					};
				} else {			
					// cyclically permute second angle
					t=N[2].angle[1];
					N[2].angle[1]=N[1].angle[1];
					N[1].angle[1]=N[0].angle[1];
					N[0].angle[1]=t;
				};
			};
			
		};
		
		// append LL to M
		
		M.insert(M.end(),LL.begin(),LL.end());	// is this the syntax?
		
		// replace L with LL
		
		L=LL;
		
		// clear LL
		
		LL.clear();
	};
	
	return(M);
	
};

