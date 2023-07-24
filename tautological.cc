/* tautological.cc

   computes the tautological lamination associated to
   a leaf with coordinate p/3^n to finite depth *exactly*
   using integer arithmetic.
   
   outputs data to .eps file or to (numerical floating point)
   lamination.
   
*/

bool link(int a, int b, int base){
	/* does (a,b) link (0-epsilon,base/3-epsilon) in [0,base)/~? */
	if((a < base/3)&&(b<base/3)){
		return(false);
	} else if((a >= base/3) && (b>= base/3)){
		return(false);
	} else return(true);
};

bool poslink(int a, int b, int base){
	/* does (a,b) link (0+epsilon,base/3+epsilon) in (0,base]/~? */
	int aa,bb;
	aa=a;
	bb=b;
	if(a==0){
		aa=base;
	};
	if(b==0){
		bb=base;
	};
	
	if((aa <= base/3)&&(bb<=base/3)){
		return(false);
	} else if((aa > base/3) && (bb > base/3)){
		return(false);
	} else return(true);
};

vector<leaf> tautological_lamination(int depth){
	vector<leaf> L;
	leaf C;
	int i,j,n,m,N,nn,nn0,nn1,nn2;
	vector<int> n_list;
	bool found_inverse;
	
	L.clear();
	
	for(i=2;i<depth;i++){
		N=pow(3,i);
		
		/* there is one leaf of depth i
		for each n = 0 mod 3 in the interval
		[N/3, 2N/3)				*/
		
//		cout << "depth i = " << i << " of " << depth << "\n";
//		cout << "N = " << N << "\n";
//		cout << "n runs from " << N/3 << " to <" << 2*N/3 << " in 3s \n";
		for(n=N/3;n<2*N/3;n=n+3){
		
//			cout << "n = " << n << "\n";
			
			/* the forward itinerary of n is
			n_list; this is normalized so that
			
			n_list[0]=n
			n_list[1]=3n mod N
			...
			n_list[j+1]=3 n_list[j] mod N
			...
			n_list[i-1]=0 mod N
			
			we must first generate n_list	*/
			
			n_list.clear();
			n_list.push_back(n);
//			cout << "n_list[0] = " << n << "\n";
			m=n;
			for(j=1;j<i;j++){
				m=(m*3)%N;
				n_list.push_back(m);
//				cout << "n_list[" << j << "] = " << m << "\n";
			};
			
			/* the backward itinerary of nn is:
			
			start with nn = N/3;
			start with pair
			n_list[i-1], nn = 0, N/3 
			
			inductively compute 
			n_list[j], nn preimage of previous n_list[j+1], nn
			that does not link 0, N/3 or n, n+N/3
			
			*/
			
			nn=N/3;
//			cout << "nn[" << i-1 << "] = " << nn << "\n";
			for(j=i-2;j>=0;j--){
				nn0=nn/3;
				nn1=nn0+N/3;
				nn2=nn0+(2*N/3);
				
				/* nn will be equal to the unique
				nnk so that (nnk, n_list[j]) does not
				link (0,N/3) or (n,n+N/3)	*/
				found_inverse=false;
				
				if(link(nn0,n_list[j],N)==false && poslink((nn0 + N-n)%N ,(n_list[j] + N-n)%N,N)==false){
					nn=nn0;
					found_inverse=true;
				};
				if(link(nn1,n_list[j],N)==false && poslink((nn1 + N-n)%N ,(n_list[j] + N-n)%N,N)==false){
					nn=nn1;
					found_inverse=true;
				};				
				if(link(nn2,n_list[j],N)==false && poslink((nn2 + N-n)%N ,(n_list[j] + N-n)%N,N)==false){
					nn=nn2;
					found_inverse=true;
				};				
//				cout << "nn[" << j << "] = " << nn << "\n";
			
			};
		
//			cout << "(n,nn) = (" << n << " , " << nn << ") \n";
//			cout << "leaf = (" << ((n*3) % N)/3 << "/" << N/3 << " , " << ((nn*3) % N)/3 << "/" << N/3 << ") \n";
			cout << ((n*3) % N)/3 << " " << N/3 << " div " << ((nn*3) % N)/3 << " " << N/3 << " div 1 " << N/9 << " div leaf2 \n";
		
		};
		
	
	};


	return(L);
};