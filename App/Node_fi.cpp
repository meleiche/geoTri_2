#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h> 
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  	
#include "geoXYZ2BLh_F.h"
using namespace std;
#include "Node_fi.h"  

Node_fi::Node_fi(void)
{	  	
	ID        = 0  ;  	
	fi        = 0.0;    
	lamda     = 0.0; 
	h         = 0.0;
	Xp        = 0.0;
	Yp        = 0.0;
	Zp        = 0.0;

	lamda_XYZ = 0.0; 	
	fi_w      = 0.0;
	hw        = 0.0;
	X0        = 0.0;   
	Y0        = 0.0;   
	Z0        = 0.0;   
	         
	fi1       = 0.0;
	h1        = 0.0;
	X1        = 0.0;
	Y1        = 0.0;
	Z1        = 0.0; 
	
	fi2       = 0.0;
	h2        = 0.0;
	X2        = 0.0;
	Y2        = 0.0;
	Z2        = 0.0;   

	err_0_fi   = 0.0;
	err_0_h    = 0.0;
	err_0_X    = 0.0;
	err_0_Y    = 0.0;
	err_0_Z    = 0.0;
	
	err_1_fi   = 0.0;
	err_1_h    = 0.0;
	err_1_X    = 0.0;
	err_1_Y    = 0.0;
	err_1_Z    = 0.0;
	
	err_2_fi   = 0.0;
	err_2_h    = 0.0;
	err_2_X    = 0.0;
	err_2_Y    = 0.0;
	err_2_Z    = 0.0;
}

Node_fi::~Node_fi(void)
{
}

void Node_fi::write_err_0(char ff[200])
{
	ofstream fn;
	fn.open(ff,ios::app); // open file in append mode
	fn << std::fixed << std::setprecision(18) ;
	//fn << ID <<"," << err_0_fi  <<"," << 	err_0_h   <<"," << 	err_0_X   <<"," << 	err_0_Y  <<"," << 	err_0_Z  ;
	fn << ID <<"," << (err_0_fi * 3600)  <<"," << (err_0_h * 1000) <<"," <<	(err_0_X * 1000) ;  
	fn << "," << 	(err_0_Y * 1000)  <<"," << 	(err_0_Z * 1000)  ;	  	
	fn << endl	;
	fn.close();
	return;
}
void Node_fi::write_err_1(char ff[200])
{
	ofstream fn;
	fn.open(ff,ios::app); // open file in append mode
	fn << std::fixed << std::setprecision(18) ;
	//fn << ID <<"," << err_1_fi  <<"," << 	err_1_h   <<"," << 	err_1_X   <<"," << 	err_1_Y  <<"," << 	err_1_Z  ;
	fn << ID <<"," << (err_1_fi * 3600)  <<"," << (err_1_h * 1000) <<"," <<	(err_1_X * 1000) ;  
	fn << "," << 	(err_1_Y * 1000)  <<"," << 	(err_1_Z * 1000)  ;	  	
	fn << endl	;
	fn.close();
	return;
}
void Node_fi::write_err_2(char ff[200])
{
	ofstream fn;
	fn.open(ff,ios::app); // open file in append mode
	fn << std::fixed << std::setprecision(18) ;
	//fn << ID <<"," << err_2_fi  <<"," << 	err_2_h   <<"," << 	err_2_X   <<"," << 	err_2_Y  <<"," << 	err_2_Z  ;	 
	fn << ID <<"," << (err_2_fi * 3600)  <<"," << (err_2_h * 1000) <<"," <<	(err_2_X * 1000) ;  
	fn << "," << 	(err_2_Y * 1000)  <<"," << 	(err_2_Z * 1000)  ;	  	
	fn << endl	;
	fn.close();
	return;
}
// OUTPUTFILE * Full data of each node
void Node_fi::write(char ff[200])
{
	ofstream fn;
	fn.open(ff,ios::app); // open file in append mode
	fn << std::fixed << std::setprecision(18) ;
	fn.setf(ios::right);	
	fn << "**********************************************************************************************" << endl;
	fn << "                        Node ID = " << ID   << endl;
	fn << "**********************************************************************************************" << endl;
	fn << " Input Point (P)"  << endl;
	fn << "---------------------------------------" << endl;
	fn << "fi    = " << fi    << "  Xp = " << Xp << endl;
	fn << "lamda = " << lamda << "  Yp = " << Yp << endl;
	fn << "h     = " << h     << "  Zp = " << Zp << endl; 	      
	fn << "---------------------------------------" << endl;
	fn << " Twin-Point (P0)"  << endl;
	fn << "---------------------------------------" << endl;
	fn << "fi_w      = " << fi_w      << "  X0 = " << X0 << endl;
	fn << "lamda_XYZ = " << lamda_XYZ << "  Y0 = " << Y0 << endl;
	fn << "hw        = " << hw        << "  Z0 = " << Z0 << endl;
	fn << "---------------------------------------" << endl;
	fn << " Point (P1)"  << endl;
	fn << "---------------------------------------" << endl;
	fn << "fi1       = " << fi1       << "  X1 = " << X1 << endl;
	fn << "lamda_XYZ = " << lamda_XYZ << "  Y1 = " << Y1 << endl;
	fn << "h1        = " << h1        << "  Z1 = " << Z1 << endl;
	fn << "---------------------------------------" << endl;
	fn << " Point (P2)"  << endl;
	fn << "---------------------------------------" << endl;  	
	fn << "fi2       = " << fi2       << "  X2 = " << X2 << endl;
	fn << "lamda_XYZ = " << lamda_XYZ << "  Y2 = " << Y2 << endl;
	fn << "h2        = " << h2        << "  Z2 = " << Z2 << endl;
	fn << "---------------------------------------" << endl;
	fn << " ERROR"  << endl;
	fn << "---------------------------------------" << endl;
	fn << "err_0_fi(ss) = " << (err_0_fi * 3600)  << "   err_0_h(mm) = " << (err_0_h * 1000) << endl;
	fn << "err_1_fi(ss) = " << (err_1_fi * 3600)  << "   err_1_h(mm) = " << (err_1_h * 1000) << endl;
	fn << "err_2_fi(ss) = " << (err_2_fi * 3600)  << "   err_2_h(mm) = " << (err_2_h * 1000) << endl;	
	//fn << endl ;
	fn.close();
	return;
};
void Node_fi::write_results(char ff[200])
{
	ofstream fn;
	fn.open(ff,ios::app); // open file in append mode
	fn << std::fixed << std::setprecision(18) ;
	fn << ID <<","  ;
	fn << (err_0_fi * 3600)  << "," << (err_0_h * 1000) <<","  ;
	fn << (err_1_fi * 3600)  << "," << (err_1_h * 1000) <<","  ;  
	fn << (err_2_fi * 3600)  << "," << (err_2_h * 1000)        ; 
	fn << endl	;
	fn.close();
	return;
} ;