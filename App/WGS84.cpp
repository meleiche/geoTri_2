#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "geoXYZ2BLh_F.h"
using namespace std	;
#include "WGS84.h"	
// -------------------------------------------------------------------------------------------------
// initialize Ellipsoid parameters 
//--------------------------------------------------------------------------------------------------
WGS84::WGS84(void)
{
	a = 0.0 ;	
	f = 0.0;	  
	b = 0.0;	  
	aa = 0.0;	  
	bb = 0.0;		
	epson2 = 0.0;
	epson = 0.0 ;
	ee = 0.0;  	
}  

WGS84::~WGS84(void)
{
}
// -------------------------------------------------------------------------------------------------
// initialize WGS84 parameters (default)
//--------------------------------------------------------------------------------------------------
bool WGS84::init_WGS84()
{
	a = 6378137.0 ;	//meters
	//a = 6378137000.0 ;	 //millimeters
	f = 298.257223563;	  
	b = a*(1 - 1/f);	  
	aa = a*a;	  
	bb = b*b;		
	epson2 = aa - bb;
	epson = sqrt (epson2) ;
	ee = epson2/aa;   
	return true;
};
// -------------------------------------------------------------------------------------------------
// initialize User defined Ellipsoid parameters 
// (a,f) are input from user
//--------------------------------------------------------------------------------------------------
bool WGS84::init_Ellipsoid()
{			  
	b = a*(1 - 1/f);	  
	aa = a*a;	  
	bb = b*b;		
	epson2 = aa - bb;
	epson = sqrt (epson2) ;
	ee = epson2/aa;   
	return true;
};

bool WGS84::write(char ff[20])
{
	ofstream fn;
	fn.open(ff,ios::app); // open file in append mode
	fn << std::fixed << std::setprecision(10) ;		
	fn.setf(ios::right);

	fn << "Ellipsoid parameters" << endl;
	fn << "-----------------------------------------------" << endl;
	fn << "semi major axis (a)    = " << a << endl  ;
	fn << "semi minor axis (b)    = " << b << endl  ;
	fn << "inverse flatting (f)   = " << f << endl  ;
	fn << "eccentricity square (ee)  = " << ee << endl  ;
	fn << "epson = " << epson << endl  ; 	
	fn.close();
	return true;
};