//Node_fi.h
#pragma once 
#ifndef _NODE_FI_
#define _NODE_FI_

class Node_fi
{
public:
    int ID   ;	  
	long double fi   ;  // input	 latitude (decimal degree)
    long double lamda;  // input	 longitude (decimal degree)
    long double h    ;  // input   h (ellipsoidal height m)
    long double Xp   ;  // computed from (fi, lamda, h)
    long double Yp   ;  // computed from (fi, lamda, h)
    long double Zp   ;  // computed from (fi, lamda, h)	  
	// Twin-Point (P0) (fi_w,hw)
	long double lamda_XYZ;   //  computed from (Xp, Yp)	 longitude (decimal degree)
	long double fi_w     ;   // latitude (decimal degree)
	long double hw       ;   // (ellipsoidal height m)
	long double X0       ;   // computed from (fi_w, lamda_XYZ, hw)
    long double Y0       ;   // computed from (fi_w, lamda_XYZ, hw)
    long double Z0       ;   // computed from (fi_w, lamda_XYZ, hw)
	// Point (P1)
	long double fi1  ;  // P1	 latitude (decimal degree)	   
    long double h1   ;   // h1   h (ellipsoidal height m)
    long double X1   ;   // computed from (fi1, lamda_XYZ, h1)
    long double Y1   ;   // computed from (fi1, lamda_XYZ, h1)
    long double Z1   ;   // computed from (fi1, lamda_XYZ, h1)
	// Point (P2)
	long double fi2  ;  // P2	 latitude (decimal degree)	   
    long double h2   ;   // h2   h (ellipsoidal height m)
    long double X2   ;   // computed from (fi2, lamda_XYZ, h2)
    long double Y2   ;   // computed from (fi2, lamda_XYZ, h2)
    long double Z2   ;   // computed from (fi2, lamda_XYZ, h2)
	// absolute error
	long double err_0_fi   ; // fi_w - fi	= fi0 - fi
	long double err_0_h    ; // hw - h		= h0 - h
	long double err_0_X    ; // X0 - Xp
	long double err_0_Y    ; // Y0 - Yp
	long double err_0_Z    ; // Z0 - Zp

	long double err_1_fi   ;  // f1 - fi
	long double err_1_h    ;  // h1 - h
	long double err_1_X    ;  // X1 - Xp
	long double err_1_Y    ;  // Y1 - Yp
	long double err_1_Z    ;  // Z1 - Zp

	long double err_2_fi   ; // fi2 - fi
	long double err_2_h    ; // h2 - h
	long double err_2_X    ; // X2 - Xp
	long double err_2_Y    ; // Y2 - Yp
	long double err_2_Z    ; // Z2 - Zp
   	
public:
	Node_fi(void);
	~Node_fi(void);
	void write_err_0(char ff[200]);
	void write_err_1(char ff[200]);
	void write_err_2(char ff[200]);
	void write(char ff[200])	;
	void write_results(char ff[200]);
}; 
#endif	   