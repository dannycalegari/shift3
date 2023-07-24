/* interface.cc */
/*
   interactive mouse and keyboard driven environments
*/

cvec user_interface(){
	bool finished;
	point p,q;
	int i;
	cpx c;
	cvec trajectory;
	polynomial P = {1.0, 0.0, 0.0, 0.0};;
	
	trajectory.clear();
	
	
	finished=false;
	
	while(finished==false){			
		XNextEvent(display, &report);
		switch(report.type) {
			case ButtonPress:	
				p=mouse_location();
				c=p_to_cpx(p);
				trajectory.push_back(c);
				i=trajectory.size();
				if(i>1){
					draw_edge(0.5*trajectory[i-2].real(),-0.5*trajectory[i-2].imag(),0.5*trajectory[i-1].real(),-0.5*trajectory[i-1].imag(),0xFF0000);
				};
				P[2]=c;
				draw_julia(P,0.0,2.0,400,800,0);
				break;
			case KeyPress:
			 //   erase_field();
			 	if(XLookupKeysym(&report.xkey, 0) == XK_q){ // quit  
			 		trajectory.push_back(trajectory[0]);	// close loop         
                    finished=true;
                    return(trajectory);
          //          XCloseDisplay(display);
          //          cout << "Quitting program. \n";
          //          exit(0);                
                    break;
                } else if(XLookupKeysym(&report.xkey, 0) == XK_Left){ 

                } else if(XLookupKeysym(&report.xkey, 0) == XK_Right){ 

                } else if(XLookupKeysym(&report.xkey, 0) == XK_Up){ 
   
                } else if(XLookupKeysym(&report.xkey, 0) == XK_Down){ 
  
                } 
            default:
            	break;
        };
    };
};

void poly_user_interface(){
	polynomial P = {1.0, 0.0, 3.0, 0.0};
	vector<leaf> D,L;
	bool finished;
	finished=false;
	
	while(finished==false){			
		XNextEvent(display, &report);
		switch(report.type) {
			case ButtonPress:	
				break;
			case KeyPress:
			 //   erase_field();
			 	if(XLookupKeysym(&report.xkey, 0) == XK_q){ // quit  
                    finished=true;      
                    break;
                } else if(XLookupKeysym(&report.xkey, 0) == XK_Left){ 
					P[3]=P[3]-0.025;
					D=critical_bottcher_coordinates(P);
					L=dynamical_lamination(3,5,D);
					erase_field();
					draw_lamination(L,600,800,100);
					draw_julia(P,0.0,2.0,400,0,0);	
					break;
                } else if(XLookupKeysym(&report.xkey, 0) == XK_Right){ 
					P[3]=P[3]+0.025;
					D=critical_bottcher_coordinates(P);
					L=dynamical_lamination(3,5,D);
					erase_field();
					draw_lamination(L,600,800,100);
					draw_julia(P,0.0,2.0,400,0,0);	
					break;
                } else if(XLookupKeysym(&report.xkey, 0) == XK_Up){ 
   					P[3]=P[3]+0.025*I;
					D=critical_bottcher_coordinates(P);
					L=dynamical_lamination(3,5,D);
					erase_field();
					draw_lamination(L,600,800,100);
					draw_julia(P,0.0,2.0,400,0,0);	
					break;
                } else if(XLookupKeysym(&report.xkey, 0) == XK_Down){ 
  					P[3]=P[3]-0.025*I;
					D=critical_bottcher_coordinates(P);
					L=dynamical_lamination(3,5,D);
					erase_field();
					draw_lamination(L,600,800,100);
					draw_julia(P,0.0,2.0,400,0,0);	
					break;
               } else if(XLookupKeysym(&report.xkey, 0) == XK_a){ 
					P[2]=P[2]-0.025;
					D=critical_bottcher_coordinates(P);
					L=dynamical_lamination(3,5,D);
					erase_field();
					draw_lamination(L,600,800,100);
					draw_julia(P,0.0,2.0,400,0,0);	
					break;
                } else if(XLookupKeysym(&report.xkey, 0) == XK_d){ 
					P[2]=P[2]+0.025;
					D=critical_bottcher_coordinates(P);
					L=dynamical_lamination(3,5,D);
					erase_field();
					draw_lamination(L,600,800,100);
					draw_julia(P,0.0,2.0,400,0,0);	
					break;
                } else if(XLookupKeysym(&report.xkey, 0) == XK_w){ 
   					P[2]=P[2]+0.025*I;
					D=critical_bottcher_coordinates(P);
					L=dynamical_lamination(3,5,D);
					erase_field();
					draw_lamination(L,600,800,100);
					draw_julia(P,0.0,2.0,400,0,0);	
					break;
                } else if(XLookupKeysym(&report.xkey, 0) == XK_s){ 
  					P[2]=P[2]-0.025*I;
					D=critical_bottcher_coordinates(P);
					L=dynamical_lamination(3,5,D);
					erase_field();
					draw_lamination(L,600,800,100);
					draw_julia(P,0.0,2.0,400,0,0);	
					break;
                } 
            default:
            	break;
        };
    };
};
