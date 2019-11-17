// WGS84.h	 
#pragma once
#ifndef _WGS84_
#define _WGS84_

#include <math.h>

class WGS84
{
public:
	long double a;	     // semi major axis
	long double f;	     // inverse flatting
	long double b;	     // semi major axis
	long double aa;	     // aa = a*a
	long double bb;		 // bb = b * b
	long double epson2;	 // epslon square
	long double epson;	 // epslon 
	long double ee;      // eccentricity square 

public:
	bool init_WGS84();
	bool init_Ellipsoid();
	bool write(char ff[20]);
	WGS84(void);
	~WGS84(void);
};	
#endif 