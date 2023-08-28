/* tautological.cc */

vector<leaf> tautological_lamination(int depth, leaf C){
	// generates degree 3 tautological lamination associated to C to depth 
	int d,i;
	vector<leaf> L;
	leaf X,Y,Z;
	double h,alpha,beta,gamma,gamma2,gamma3,gamma_next,angle_start,increment;
	vector<double> alpha_list;
	bool found_gamma_next;
	
	L.clear();
	
	h=log(C.height);
	C.offset=1.0/3.0;
	
	
	for(d=1;d<depth;d++){
		increment = (double) pow(3.0,d);
		increment =1.0/increment;
		angle_start=C.angle+C.offset+C.angle*increment+0.000001;
		for(alpha=angle_start;alpha<C.angle+C.offset+1.0/3.0;alpha=alpha+increment){
			// alpha is the initial value of leaves X and Y
			X.angle=alpha-0.000001;
			X.offset=1.0/3.0;
			
			// we want to find a leaf Y = {alpha,gamma} such that 3^iY does not link X or C for 0<=i < n
			// and 3^nY=C
			
			// step 1: generate list of values 3^i alpha
			
			alpha_list.clear();
			beta=alpha;
			alpha_list.push_back(beta);
			for(i=0;i<d;i++){
				beta=beta*3.0;
				while(beta>1.0){
					beta=beta-1.0;
				};
				while(beta<0.0){
					beta=beta+1.0;
				};
				alpha_list.push_back(beta);
			};
			
			// step 2: inductively find 3^i gamma for i=n back to 0
			
			alpha=C.angle;
			gamma=C.angle+C.offset;
	//		cout << "d = " << d << "\n";
	//		cout << "i = " << d << " alpha = " << alpha << " gamma = " << gamma << "\n";
			for(i=d-1;i>=0;i--){
				alpha=alpha_list[i];
				gamma=gamma/3.0;
				gamma2=gamma+(1.0/3.0);
				gamma3=gamma+(2.0/3.0);
				while(gamma2>1.0){
					gamma2=gamma2-1.0;
				};
				while(gamma3>1.0){
					gamma3=gamma3-1.0;
				};
				
				// need to test: does {alpha,gamma} link C or X?
				found_gamma_next=false;
				
				Z=leaf_from_angles(alpha,gamma,1.1);
				if(link(X,Z)==false && link(C,Z)==false){
					// gamma is the correct choice
					gamma_next=gamma;
					found_gamma_next=true;
				};
				Z=leaf_from_angles(alpha,gamma2,1.1);
				if(link(X,Z)==false && link(C,Z)==false){
					// gamma2 is the correct choice
					gamma_next=gamma2;
					found_gamma_next=true;
				};
				Z=leaf_from_angles(alpha,gamma3,1.1);
				if(link(X,Z)==false && link(C,Z)==false){
					// gamma3 is the correct choice
					gamma_next=gamma3;
					found_gamma_next=true;
				};
				if(found_gamma_next==false){
					cout << "error! couldn't find an unlinked preimage!! \n";
				//	break;
				};
				gamma=gamma_next;
	//			cout << "i = " << i << " alpha = " << alpha << " gamma = " << gamma << "\n";
			};
			Z=leaf_from_angles(alpha*3.0,gamma*3.0,exp(h*increment));
	//		cout << "leaf; angle = " << Z.angle << " offset = " << Z.offset << "\n";
			L.push_back(Z);
		};
	};
	
	
	return(L);
};