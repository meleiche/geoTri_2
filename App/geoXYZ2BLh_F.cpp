// geoXYZ2BLh_F.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include <stdlib.h>
#include <cmath>		 
#include <windows.h>
#include <Commdlg.h>	 
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <ctime>	 
#include "Node.h"
#include "Node_fi.h"	
#include "height.h"	
#include "Counter.h"
#include "WGS84.h"
#include "geoXYZ2BLh_F.h"
using namespace std;
#define MAX_LOADSTRING 100
// ----------------------------------------------------------------------------------------------------------
//									Global Variables
// ----------------------------------------------------------------------------------------------------------
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
char szFileName[MAX_PATH] ="";	  // input file name
Counter Con;					  // counter to count points
Node    *Node_Array;			  // Array of Node
Node_fi *Node_Array_fi;			  // Array of Node_fi
WGS84   WGS84_Datum;			  // Datum parameters Latest 
height  height_List; 
HWND hwnd;              // owner window
HANDLE hf;              // file handle
HWND g_hMainDialog = NULL;
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);   // Main window
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);	   // About dialogue window
INT_PTR CALLBACK	Geodetic(HWND, UINT, WPARAM, LPARAM);  // Geodetic dialogue window

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//							G L O B A L    F U N C T I O N S
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//Free char array
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
BOOL Free_Array_char(char s[], int size)
{
	int j;
	for(j=0; j < size; j++)
	{
		s[j] = '\0';
	}
	return TRUE;
}  
//-------------------------------------------------------
// Write the input read data to external file
//-------------------------------------------------------
BOOL Write_Input_To_File()
{
	int kk, l;
	char rr[300];
	FILE *log;
	char fn[] = OUTPUT_FILE ;
	size_t ll = 0;
	//log = fopen(fn, "w");
	fopen_s(&log, fn, "w");
	kk = Con.Nodes;
	l=0;
	sprintf_s( rr, 300,  "****************************************************************************************** \n\n");
	fputs(rr, log);
	sprintf_s( rr, 300,  "                  Trilateration Program  \n \nOct 2019       \n\n");
	fputs(rr, log);
	sprintf_s( rr, 300,  "****************************************************************************************** \n");
	fputs(rr, log);
	sprintf_s( rr, 300,  "\n\n\n------------------------------------------------------------------------------------------ \n");
	fputs(rr, log);
	sprintf_s( rr, 300,  "                    I N P U T      D A T A    \n\n\n");
	fputs(rr, log);
	sprintf_s( rr, 300,  "               Name of file data is %s    \n", szFileName);
	fputs(rr, log);
	sprintf_s( rr, 300,  "------------------------------------------------------------------------------------------ \n\n");
	fputs(rr, log);
	sprintf_s( rr, 300,  "Input Array of Nodes \n");
	fputs(rr, log);
	sprintf_s( rr, 300,  "****************************************************************************************** \n");
	fputs(rr, log);
	fclose (log) ;	
	//log = fopen(fn, "a"); // apend
	fopen_s(&log, fn, "a"); // apend
	sprintf_s( rr, 300,  "****************************************************************************************** \n");
	fputs(rr, log);
	sprintf_s( rr, 300,  "				End of  Write_Input_To_File() \n");
	fputs(rr, log);
	sprintf_s( rr, 300,  "****************************************************************************************** \n");
	fputs(rr, log);
	fclose (log) ;
	return TRUE;
}
// ----------------------------------------------------------------------------------------------------------
// Select Data file 
// ----------------------------------------------------------------------------------------------------------
void DoFileSelect(HWND hwnd)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ZeroMemory( &szFileName, sizeof( szFileName ) );
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;	
	ofn.lpstrFile = szFileName;	
	ofn.lpstrFile[0] = '\0';
	ofn.lpstrFilter = __T("Data Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");	
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "txt";
	
	if(GetOpenFileName(&ofn))
	{
		SetDlgItemText(g_hMainDialog, IDC_TEXT, szFileName);
	}
	//if (!(GetOpenFileName(&ofn)))
	//{
	//	//MessageBox(hwnd, "No File Selected!", NULL, NULL);
	//	return ;
	//}
	//MessageBox(hwnd, "Done File Select", NULL, NULL);
	return ;
}
// ----------------------------------------------------------------------------------------------------------
// Read Data file to count records ONLY
// ----------------------------------------------------------------------------------------------------------
BOOL Count_Records(Counter& Coun)
{
	FILE *inFile;
	errno_t err;
	char buff[MAX_REC_LEN];
	char yy[300];
	ofstream fn;	
	//char rr[100];
	char z[] = "z"; //separator between datasets
	int i, flag;
	i = flag = 0;	
	// --------------------------------------------------------------------
	// Open File and Count Number Of Records
	// --------------------------------------------------------------------	
	err  = fopen_s( &inFile, szFileName , "r" );
	if( err == 0 )
	{
		//sprintf_s(yy,100, "The file was opened\n");
		//MessageBox(hwnd, "The file was opened\n", "i",MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		sprintf_s(yy,100, "The file was not opened\n");
		MessageBox(hwnd, "The file was not opened\n", "i",MB_OK | MB_ICONINFORMATION);
		return TRUE;
	}

	while(fgets(buff,sizeof(buff),inFile) != NULL)
	{			
		i++;   		
	}		
	//sprintf_s(yy,250, "i = %d   nodes = %d", i,  Coun.Nodes);
	//MessageBox(hwnd, yy, "i",MB_OK | MB_ICONINFORMATION);
	Coun.Nodes = i ; // EOF	
	fclose(inFile);
	sprintf_s(yy,250, "i = %d    nodes = %d", i,  Coun.Nodes);	 	
	MessageBox(hwnd, yy, "i",MB_OK | MB_ICONINFORMATION);
	//fn.open(OUTPUT_FILE,ios::out); // new output file
	//fn.close();
	//Coun.write(OUTPUT_FILE);
	return TRUE;	
}
// ----------------------------------------------------------------------------------------------------------
//	HEader for FULL (OUTPUT_FILE)
// ----------------------------------------------------------------------------------------------------------
void write_Header_fi2X(char ff[20])
{
	ofstream fn;  
	fn.open(ff,ios::out); // new file 	
	fn << "********************************************************************\n";
	fn << "              Trilateration Algorithm  (Oct 2019)  \n\n";
	fn << " This is conversion of (fi, lamda, h) to (X,Y,Z) " << endl;		
	fn << "********************************************************************\n";  
	fn << "\n     Input File Name:  " <<  szFileName  << endl; 	
	fn << "All units in meters if not specified otherwise" << endl;
	fn << "********************************************************************\n";	  	
	fn.close();
	return;
};
// ----------------------------------------------------------------------------------------------------------
//	HEader for FULL (OUTPUT_FILE)
// ----------------------------------------------------------------------------------------------------------
void write_Header_X2fi(char ff[20])
{
	ofstream fn;  
	fn.open(ff,ios::out); // new file 	
	fn << "********************************************************************\n";
	fn << "              Trilateration Algorithm (Oct 2019)   \n\n";
	fn << " This is conversion of (X,Y,Z) to (fi, lamda, h) " << endl;
	fn << "		Given Point (P) as (X,Y,Z) " << endl;
	fn << "			Compute Points: P0, P1, P2 " << endl;
	fn << "			{P0} = (fi_w, lamda, hw)" << endl;
	fn << "			{P1} = (fi1, lamda, h1) " << endl;
	fn << "			{P2} = (fi2, lamda, h2) " << endl;	
	fn << "********************************************************************\n"; 
	fn << "\n     Input File Name:  " <<  szFileName  <<endl ;  	
	fn << "All units in meters if not specified otherwise" << endl;
	fn << "********************************************************************\n";	  	
	fn.close();
	return;
};
// ----------------------------------------------------------------------------------------------------------
//	HEader for FULL (OUTPUT_FILE)
// ----------------------------------------------------------------------------------------------------------
void write_Header_rZ2fi(char ff[20])
{
	ofstream fn;  
	fn.open(ff,ios::out); // new file 	
	fn << "********************************************************************\n";
	fn << "              Trilateration Algorithm (Oct 2019)   \n\n";
	fn << " This is conversion of (r,Z) to (fi, h) " << endl;
	fn << "		Given Point (P) as (r,Z) " << endl;
	fn << "			Compute Points: P0, P1, P2 " << endl;
	fn << "			{P0} = (fi_w, hw)" << endl;
	fn << "			{P1} = (fi1, h1) " << endl;
	fn << "			{P2} = (fi2, h2) " << endl;	
	fn << "********************************************************************\n"; 
	fn << "\n     Input File Name:  " <<  szFileName  <<endl ;  	
	fn << "All units in meters if not specified otherwise" << endl;
	fn << "********************************************************************\n";	  	
	fn.close();
	return;
};
// ----------------------------------------------------------------------------------------------------------
//	HEader for FULL (OUTPUT_FILE)
// ----------------------------------------------------------------------------------------------------------
void write_Header_validate(char ff[20])
{
	ofstream fn;  
	fn.open(ff,ios::out); // new file 	
	fn << "********************************************************************\n";
	fn << "              Trilateration Algorithm (Oct 2019)   \n\n";
	fn << " This is Validation for algorithm " << endl;		
	fn << "		Given Point (P) as (fi, lamda, h) " << endl;
	fn << "			Compute Points: P0, P1, P2 " << endl;
	fn << "			{err_0} = {P0} - {P} " << endl;
	fn << "			{err_1} = {P1} - {P} " << endl;
	fn << "			{err_2} = {P2} - {P} " << endl;	
	fn << "********************************************************************\n";  
	fn << "     Input File Name:  " <<  szFileName  <<endl << endl;
	Con.write(OUTPUT_FILE);
	fn << "All units in meters if not specified otherwise" << endl;
	fn << "********************************************************************\n";	  	
	fn.close();
	return;
};
// ----------------------------------------------------------------------------------------------------------
// header for (SOLUTION_FILE)
// ----------------------------------------------------------------------------------------------------------
void write_Header_Solution(char ff[20])	 
{
	ofstream fn;
	fn.open(ff,ios::app); // open file in append mode
	fn << std::fixed << std::setprecision(18) ;
	
	fn << "ID,fi,lamda,h," ;
	fn << "Xp,Yp,Zp,rp,";
	fn << "fi_w,hw," ;
	fn << "fi_err_SS,fi_err_DD,h_err,ddr,taw,";
	fn << "rQ,ZQ,rQw,ZQw,dr,dZ,";
	fn << "fi_new,h_new";
	fn << endl	;
	fn.close();
	return;
};	 
// ----------------------------------------------------------------------------------------------------------
//	HEader for  (XYZ_FILE) results of XYZ to fi
// ----------------------------------------------------------------------------------------------------------
void write_Header_XYZ(char ff[20])
{
	ofstream fn;	
	fn.open(ff,ios::out); // new file
	fn << "Xp,Yp,Zp" << endl ;	  		
	fn.close();
	return;	 	
};
// ----------------------------------------------------------------------------------------------------------
//	HEader for  (BLh_FILE) results of XYZ to fi
// ----------------------------------------------------------------------------------------------------------
void write_Header_BLh(char ff[20])
{
	ofstream fn;	
	fn.open(ff,ios::out); // new file
	fn << "B(fi) (DD),L(lamda) (DD),h (m)" << endl ;	  		
	fn.close();
	return;	 	
};
// ----------------------------------------------------------------------------------------------------------
//	HEader for  (rZ_FILE) results of (r,Z) to (fi.h)
// ----------------------------------------------------------------------------------------------------------
void write_Header_rZ(char ff[20])
{
	ofstream fn;	
	fn.open(ff,ios::out); // new file
	fn << "rp,Zp" << endl ;	  		
	fn.close();
	return;	
};
// ----------------------------------------------------------------------------------------------------------
//	HEader for (err_FILE) results of err file	
// ----------------------------------------------------------------------------------------------------------
void write_Header_err(char ff[20])
{
	ofstream fn;	
	fn.open(ff,ios::out); // new file
	fn << "fi_err,h_err,delta_r";	  	
	fn << endl	; 		
	fn.close();
	return;	 	
};
// ----------------------------------------------------------------------------------------------------------
//	HEader for (ERR_0_File) 
// ---------------------------------------------------------------------------------------------------------
void write_Header_err0(char ff[20])
{
	ofstream fn;	
	fn.open(ff,ios::out); // new file
	fn << std::fixed << std::setprecision(18) ;
	fn << "ID ,err_0_fi(sec),err_0_h(mm),err_0_X(mm),err_0_Y(mm),err_0_Z(mm)"  ;	  	
	fn << endl	;
	fn.close();
	return;	
}
// ----------------------------------------------------------------------------------------------------------
//	HEader for (ERR_1_File) 
// ---------------------------------------------------------------------------------------------------------
void write_Header_err1(char ff[20])
{
	ofstream fn;	
	fn.open(ff,ios::out); // new file
	fn << std::fixed << std::setprecision(18) ;  	
	fn << "ID ,err_1_fi(sec),err_1_h(mm),err_1_X(mm),err_1_Y(mm),err_1_Z(mm)"  ;
	fn << endl	;
	fn.close();
	return;	
}
// ----------------------------------------------------------------------------------------------------------
//	HEader for (ERR_2_File) 
// ---------------------------------------------------------------------------------------------------------
void write_Header_err2(char ff[20])
{
	ofstream fn;	
	fn.open(ff,ios::out); // new file
	fn << std::fixed << std::setprecision(18) ;	
	fn << "ID ,err_2_fi(sec),err_2_h(mm),err_2_X(mm),err_2_Y(mm),err_2_Z(mm)"  ;
	fn << endl	;
	fn.close();
	return;	
}
// ----------------------------------------------------------------------------------------------------------
//	HEader for (ERR_RESULTS) 	All err in one file
// ---------------------------------------------------------------------------------------------------------
void write_Header_err012(char ff[20])
{
	ofstream fn;	
	fn.open(ff,ios::out); // new file
	fn << std::fixed << std::setprecision(18) ;	
	fn << "ID ,err_0_fi(sec),err_0_h(mm),err_1_fi(sec),err_1_h(mm),err_2_fi(sec),err_2_h(mm)"  ;
	fn << endl	;
	fn.close();
	return;	
}
// ----------------------------------------------------------------------------------------------------------
//	HEader for (loc_File) 
// ---------------------------------------------------------------------------------------------------------
void write_Header_loc(char ff[20])
{
	ofstream fn;	
	fn.open(ff,ios::out); // new file
	fn << std::fixed << std::setprecision(18) ;
	fn << "ID,check_ellipse,loc_W"  ;	  	
	fn << endl	;
	fn.close();
	return;	
}
// ----------------------------------------------------------------------------------------------------------
//	HEader for results of data in required format
// ----------------------------------------------------------------------------------------------------------
void write_matrix(char ff[20])
{
	ofstream fn;
	int i, j;	
	i = j = 0;
	fn.open(ff,ios::app); // open file in append mode
	//fn << std::fixed << std::setprecision(10) ;  
	fn << "********************************************************************\n";
	fn << "              Trilateration Algorithm    \n\n\n";
	fn << " This is Sample data file " << endl;
	fn << "                                                   April 2018 " << endl;
	fn << "********************************************************************\n\n";
	fn << "\n\n     Input File Name:  " <<  szFileName  <<endl << endl;	
	fn << " Data format:  " << endl;
	fn << "ID,fi,lamda,h,Xp,Yp,Zp,fi_w,hw,fi_err,h_err,ddr_taw"  << endl << endl;	
	fn << "********************************************************************\n\n\n";

	 for (i = 0; i<Con.Nodes ; i++)
	 {
		fn << Node_Array[i].ID <<"," << Node_Array[i].fi <<"," << Node_Array[i].lamda <<"," << Node_Array[i].h <<"," ;
		fn << Node_Array[i].Xp <<"," << Node_Array[i].Yp <<"," << Node_Array[i].Zp <<"," ;
		fn << Node_Array[i].fi_w <<"," << Node_Array[i].hw << "," ;
		fn << Node_Array[i].fi_err <<"," << Node_Array[i].h_err <<"," << Node_Array[i].ddr <<"," << Node_Array[i].taw ;
		fn << endl ;
	 }
	fn << "********************************************************************\n";
	fn << "              Trilateration Algorithm    \n\n\n";
	fn << " ddr for all fi" << endl;
	fn << "                                                 " << endl;
	fn << "********************************************************************\n\n";
	 for (i = 0; i<Con.Nodes ; i++)
	 {	   
		 //fn << "i = " << i << "j = " << j << endl ;
		  fn << Node_Array[i].ddr << "," ;
		  j++;
		  if (j == 90)
		  {
			  j=0;
			   //fn << "i = " << i << "j = " << j << endl;
			   fn << endl ;
		  }
	 }	
	fn << "********************************************************************\n";
	fn << "              Trilateration Algorithm    \n\n\n";
	fn << " fi_err for all fi" << endl;
	fn << "                                                 " << endl;
	fn << "********************************************************************\n\n";
	 for (i = 0; i<Con.Nodes ; i++)
	 {	   
		 //fn << "i = " << i << "j = " << j << endl ;
		  fn << Node_Array[i].fi_err << "," ;
		  j++;
		  if (j == 90)
		  {
			  j=0;
			   //fn << "i = " << i << "j = " << j << endl;
			   fn << endl ;
		  }
	 }
	fn << "********************************************************************\n";
	fn << "              Trilateration Algorithm    \n\n\n";
	fn << " h_err for all fi" << endl;
	fn << "                                                 " << endl;
	fn << "********************************************************************\n\n";
	 for (i = 0; i<Con.Nodes ; i++)
	 {	   
		 //fn << "i = " << i << "j = " << j << endl ;
		  fn << Node_Array[i].h_err << "," ;
		  j++;
		  if (j == 90)
		  {
			  j=0;
			   //fn << "i = " << i << "j = " << j << endl;
			   fn << endl ;
		  }
	 }	
	fn.close();
	return;
};
void write_Data(char ff[20])
{
	ofstream fn;
	int i, j;	
	i = j = 0;
	fn.open(ff,ios::app); // open file in append mode
	fn << "********************************************************************\n";
	fn << "              Trilateration Algorithm    \n\n\n";
	fn << " This is Sample data file " << endl;
	fn << "                                                   April 2018 " << endl;
	fn << "********************************************************************\n\n"; 
	fn << " Data format:  " << endl;
	fn << "ID,fi,lamda,h"  << endl << endl;		
	fn << "********************************************************************\n\n\n";
		 for (i = 0; i<91 ; i++)
		 {
			 fn << i+1 <<",50," ;

		 }
}
 // export data which are input to algorithm: fi,50,h 
void write_List_fi(char ff[20])
{
    ofstream fn;
	int i, j;
	i = j = 0;
	fn.open(ff,ios::app); // open file in append mode
	fn << std::fixed << std::setprecision(18) ;	
	//fn.setf(ios::right);
	for(i=0; i<91;i++)
	{
		fn << i << ",50," << height_List.h1 << endl;
		fn << i << ",50," << height_List.h2 << endl;
		fn << i << ",50," << height_List.h3 << endl;
		fn << i << ",50," << height_List.h4 << endl;
		fn << i << ",50," << height_List.h5 << endl;
		fn << i << ",50," << height_List.h6 << endl;
		fn << i << ",50," << height_List.h7 << endl;
		fn << i << ",50," << height_List.h8 << endl;
		fn << i << ",50," << height_List.h9 << endl;
		fn << i << ",50," << height_List.h10 << endl;
		fn << i << ",50," << height_List.h11 << endl;
		fn << i << ",50," << height_List.h12 << endl;
		fn << i << ",50," << height_List.h13 << endl;
		fn << i << ",50," << height_List.h14 << endl;
		fn << i << ",50," << height_List.h15 << endl;
		fn << i << ",50," << height_List.h16 << endl;
		fn << i << ",50," << height_List.h17 << endl;
		fn << i << ",50," << height_List.h18 << endl;
		fn << i << ",50," << height_List.h19 << endl;
		fn << i << ",50," << height_List.h20 << endl;
		fn << i << ",50," << height_List.h21 << endl;
		fn << i << ",50," << height_List.h22 << endl;
		fn << i << ",50," << height_List.h23 << endl;
		fn << i << ",50," << height_List.h24 << endl;
		fn << i << ",50," << height_List.h25 << endl;
		fn << i << ",50," << height_List.h26 << endl;
		fn << i << ",50," << height_List.h27 << endl;
		fn << i << ",50," << height_List.h28 << endl;
		fn << i << ",50," << height_List.h29 << endl;
		fn << i << ",50," << height_List.h30 << endl;
		fn << i << ",50," << height_List.h31 << endl;
		fn << i << ",50," << height_List.h32 << endl;
		fn << i << ",50," << height_List.h33 << endl;
		fn << i << ",50," << height_List.h34 << endl;
		fn << i << ",50," << height_List.h35 << endl;  
	}
	//fn.close();
	return;
}
// ----------------------------------------------------------------------------------------------------------
// Fill ID, fi, lmada, h
// ----------------------------------------------------------------------------------------------------------
BOOL Fill_Input_Array_fi(int N)
{
	FILE *inFile;
	errno_t err;
	char line[MAX_REC_LEN];
	//char rr[100];
	int i, j, m, flag1, k2;
	i = j = flag1 = k2 = m = 0;
	// --------------------------------------------------------------------
	// Open  Data File
	// --------------------------------------------------------------------	
	err  = fopen_s( &inFile, szFileName , "r" );	
	i = 0;
	while(fgets(line, sizeof(line) ,inFile) != NULL)
	{	  		
		if ( i  < N  )
		{
			Node_Array[i].init();
			Node_Array[i].set_ID(i);
			Node_Array[i].fill_fi(line);	// put (fi, lamda, h) inside Node_Array			
			i++;
		} 		
	}	
	fclose(inFile);
	//sprintf( rr, "Read Finished \n Number of Nodes = %d" , N );
	//MessageBox(hwnd, rr, "Count" ,MB_OK | MB_ICONINFORMATION);
	return TRUE;											 
}
// ----------------------------------------------------------------------------------------------------------
// XYZ input
// ----------------------------------------------------------------------------------------------------------
BOOL Fill_Input_Array_XYZ(int N)
{
	FILE *inFile;
	errno_t err;
	char line[MAX_REC_LEN];
//	char rr[100];
	int i, j, m, flag1, k2;
	i = j = flag1 = k2 = m = 0;
	// --------------------------------------------------------------------
	// Open  Data File
	// --------------------------------------------------------------------
	err  = fopen_s( &inFile, szFileName , "r" ); 	
	i = 0;
	while(fgets(line, sizeof(line) ,inFile) != NULL)
	{	  		
		if ( i  < N  )
		{
			Node_Array[i].init();
			Node_Array[i].set_ID(i);
			Node_Array[i].fill_XYZ(line);			
			i++;
		} 		
	}	
	fclose(inFile);
	//sprintf( rr, "Read Finished \n Number of Nodes = %d" , N );
	//MessageBox(hwnd, rr, "Count" ,MB_OK | MB_ICONINFORMATION);
	return TRUE;											 
}
// ----------------------------------------------------------------------------------------------------------
// rZ input
// ----------------------------------------------------------------------------------------------------------
BOOL Fill_Input_Array_rZ(int N)
{
	FILE *inFile;
	errno_t err;
	char line[MAX_REC_LEN];
//	char rr[100];
	int i, j, m, flag1, k2;
	i = j = flag1 = k2 = m = 0;
	// --------------------------------------------------------------------
	// Open  Data File
	// --------------------------------------------------------------------	  
	err  = fopen_s( &inFile, szFileName , "r" );	  	
	i = 0;
	while(fgets(line, sizeof(line) ,inFile) != NULL)
	{	  		
		if ( i  < N  )
		{
			Node_Array[i].init();
			Node_Array[i].set_ID(i);
			Node_Array[i].fill_rZ(line);		
			i++;
		}  		
	}	
	fclose(inFile);						
	return TRUE;											 
}
// ----------------------------------------------------------------------------------------------------------
//	input XYZ list
// ----------------------------------------------------------------------------------------------------------
BOOL Fill_Input_Array_err(int N)
{
	FILE *inFile;
	errno_t err;
	char line[MAX_REC_LEN];
//	char rr[100];
	int i, j, m, flag1, k2;
	i = j = flag1 = k2 = m = 0;
	// --------------------------------------------------------------------
	// Open  Data File
	// --------------------------------------------------------------------	 		
	err  = fopen_s( &inFile, szFileName , "r" );  
	i = 0;
	while(fgets(line, sizeof(line) ,inFile) != NULL)
	{	  		
		if ( i  < N  )
		{
			Node_Array[i].init();
			Node_Array[i].set_ID(i);
			Node_Array[i].fill_err(line);			
			i++;
		}			
	}	
	fclose(inFile);		
	return TRUE;											 
}
// ----------------------------------------------------------------------------------------------------------
//(fi, lamda, h) to (X,Y,Z)
// ----------------------------------------------------------------------------------------------------------
void fi2XYZ(char out[], char rslt[])
{
	int i = 0 ;
	/*DoFileSelect(hwnd);	  // open input file
	Con.init();			  //
	Count_Records(Con);	  // count records and store in [Con.Nodes]	  	 
	Node_Array = new Node[Con.Nodes];	// create new array of nodes with size [Con.Nodes]
	*/
	
	//Con.write(OUTPUT_FILE);	//
	
	for(i=0; i<Con.Nodes; i++)
	{
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation
		Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].Exact_Q(WGS84_Datum.a,  WGS84_Datum.b);
		Node_Array[i].compute_delta_r();
		Node_Array[i].write(out);
		//Node_Array[i].write_map(SOLUTION_FILE);
		Node_Array[i].write_XYZ2(rslt);
		//Node_Array[i].write_rZ(rZ_FILE);
		//Node_Array[i].write_Results_err(err_FILE);
	}					   
	//MessageBox(hwnd, "Done", "Done",MB_OK | MB_ICONINFORMATION);
	//write_Matrix(SOLUTION_FILE);	
	//write_matrix(Data_FILE);
	return;
}
// ----------------------------------------------------------------------------------------------------------
//	(X,Y,Z) to (fi, lamda, h) 
// ----------------------------------------------------------------------------------------------------------
void XYZ2fi(char out[], char rslt[])
{
	int i = 0 ;
	int test = 0 ; 	
	for(i=0; i<Con.Nodes; i++)
	{
		Node_Array[i].XYZ2fi_S(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].write_XYZ(out);
		Node_Array[i].fill_fi_1();
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation
		Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].Exact_Q(WGS84_Datum.a,  WGS84_Datum.b);
		Node_Array[i].Compute_dXYZ();
		Node_Array[i].write(out);
		//Number of iteration (test)
		//test = 20 ;
		test = 2;
		while (test != 0)
		{
			Node_Array[i].fill_fi_2();
			Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation
			Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
			Node_Array[i].Exact_Q(WGS84_Datum.a,  WGS84_Datum.b);
			Node_Array[i].Compute_dXYZ();
			Node_Array[i].write(out);
			test--;
		}
		//Node_Array[i].write_map(SOLUTION_FILE);
		Node_Array[i].write_BLh(rslt);
		/*
		Node_Array[i].fill_fi_2();
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation
		Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].Exact_Q(WGS84_Datum.a,  WGS84_Datum.b);
		Node_Array[i].write(OUTPUT_FILE);
		Node_Array[i].fill_fi_2();
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation
		Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].Exact_Q(WGS84_Datum.a,  WGS84_Datum.b);
		Node_Array[i].write(OUTPUT_FILE);
		Node_Array[i].fill_fi_2();
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation
		Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].Exact_Q(WGS84_Datum.a,  WGS84_Datum.b);
		Node_Array[i].write(OUTPUT_FILE);
		Node_Array[i].fill_fi_2();
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation
		Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].Exact_Q(WGS84_Datum.a,  WGS84_Datum.b);
		Node_Array[i].write(OUTPUT_FILE);
		Node_Array[i].fill_fi_2();
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation
		Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].Exact_Q(WGS84_Datum.a,  WGS84_Datum.b);
		Node_Array[i].write(OUTPUT_FILE);
		*/
	}

	//MessageBox(hwnd, "Done", "Done",MB_OK | MB_ICONINFORMATION);
	//write_Matrix(SOLUTION_FILE);
	//write_matrix(Data_FILE);
	return;
}
// ----------------------------------------------------------------------------------------------------------
//	(r,Z) to (fi, h) 
// ----------------------------------------------------------------------------------------------------------
void rZ2fi()
{
	int i = 0 ;
	int test = 0 ;	
	
	for(i=0; i<Con.Nodes; i++)
	{
		Node_Array[i].rZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].write_XYZ(OUTPUT_FILE);
		/*
		Node_Array[i].fill_fi_1();
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation
		Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].Exact_Q(WGS84_Datum.a,  WGS84_Datum.b);
		Node_Array[i].Compute_dXYZ();
		Node_Array[i].write(OUTPUT_FILE);
		//Number of iteration (test)
		test = 20 ;
		test = 2;
		while (test != 0)
		{
			Node_Array[i].fill_fi_2();
			Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation
			Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
			Node_Array[i].Exact_Q(WGS84_Datum.a,  WGS84_Datum.b);
			Node_Array[i].Compute_dXYZ();
			Node_Array[i].write(OUTPUT_FILE);
			test--;
		}
		*/
		//Node_Array[i].write_map(SOLUTION_FILE);
		/*
		Node_Array[i].fill_fi_2();
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation
		Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].Exact_Q(WGS84_Datum.a,  WGS84_Datum.b);
		Node_Array[i].write(OUTPUT_FILE);
		Node_Array[i].fill_fi_2();
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation
		Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].Exact_Q(WGS84_Datum.a,  WGS84_Datum.b);
		Node_Array[i].write(OUTPUT_FILE);
		Node_Array[i].fill_fi_2();
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation
		Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].Exact_Q(WGS84_Datum.a,  WGS84_Datum.b);
		Node_Array[i].write(OUTPUT_FILE);
		Node_Array[i].fill_fi_2();
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation
		Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].Exact_Q(WGS84_Datum.a,  WGS84_Datum.b);
		Node_Array[i].write(OUTPUT_FILE);
		Node_Array[i].fill_fi_2();
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation
		Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].Exact_Q(WGS84_Datum.a,  WGS84_Datum.b);
		Node_Array[i].write(OUTPUT_FILE);
		*/
	}	 
	return;
}
void err_sum()
{
	int i = 0 ;
	//DoFileSelect(hwnd);	
	Con.init();
	Count_Records(Con);
	Con.write(OUTPUT_FILE);
	//write_Header_XYZ(OUTPUT_FILE);
	Node_Array = new Node[Con.Nodes];	
	Fill_Input_Array_err(Con.Nodes);
	for(i=0; i<Con.Nodes; i++)
	{
		Node_Array[i].write_err(OUTPUT_FILE);
	}		 	
	return;
}
// ----------------------------------------------------------------------------------------------------------
// Validate Algorithm
// input fi, lamda, h)
//(fi, lamda, h) to (X,Y,Z) 
//(X,Y,Z) to (fi, lamda, h)
//(fi_w, lamda, hw) to (X0,Y0,Z0) 
//(fi1, lamda, h1) to (X1,Y1,Z1) 
//(fi2, lamda, h2) to (X2,Y2,Z2) 
// ----------------------------------------------------------------------------------------------------------
void Validate_XYZ2fi(char out[200], char rslt[200])
{
	int i = 0; 
	for(i=0; i<Con.Nodes; i++)
	{	
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// 1) compute (Xp,Yp, Zp) from 	(fi, lamda, h)	(point P )
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); 
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// 2) Store original data inside: 	Node_Array_fi[i]
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 	 
		Node_Array_fi[i].ID    =  Node_Array[i].ID;
		Node_Array_fi[i].fi    =  Node_Array[i].fi;
		Node_Array_fi[i].lamda =  Node_Array[i].lamda;
		Node_Array_fi[i].h     =  Node_Array[i].h;
		Node_Array_fi[i].Xp    =  Node_Array[i].Xp;
		Node_Array_fi[i].Yp    =  Node_Array[i].Yp;
		Node_Array_fi[i].Zp    =  Node_Array[i].Zp;
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Start of validation - From 	(Xp,Yp, Zp)
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 		
		// 3) compute (fi_w, lamda, h_w) for point (P0) From (Xp,Yp, Zp)
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		Node_Array[i].init();
		Node_Array[i].ID   =   Node_Array_fi[i].ID ;
		Node_Array[i].Xp   =   Node_Array_fi[i].Xp ;
		Node_Array[i].Yp   =   Node_Array_fi[i].Yp ;
		Node_Array[i].Zp   =   Node_Array_fi[i].Zp ;

		Node_Array[i].XYZ2fi_S(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson); //Point P0 (fi_w, lamda, hw)
		//Node_Array[i].write_loc(loc_File);
		
		Node_Array_fi[i].fi_w       =  Node_Array[i].fi_w ;	 //fi0
		Node_Array_fi[i].lamda_XYZ  =  Node_Array[i].lamda ; // lamda from (Xp,Yp, Zp)
		Node_Array_fi[i].hw         =  Node_Array[i].hw ;  	 // h0
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// 4) compute (X0,Y0,Z0) for Point P0 from(fi_w, lamda, hw)  
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		Node_Array[i].init();
		Node_Array[i].ID    =  	Node_Array_fi[i].ID;
		Node_Array[i].fi    =  	Node_Array_fi[i].fi_w   ;
		Node_Array[i].lamda =  	Node_Array_fi[i].lamda_XYZ ;
		Node_Array[i].h     =   Node_Array_fi[i].hw   ;
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation	get(X0,Y0,Z0)
		Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].compute_h_err(); //hw0 - hw									   
		Node_Array[i].compute_fi_err() ;
		// store P0
		Node_Array_fi[i].X0	= Node_Array[i].Xp	;
		Node_Array_fi[i].Y0	= Node_Array[i].Yp	;
		Node_Array_fi[i].Z0	= Node_Array[i].Zp	; 		
		// Compute P1
		Node_Array_fi[i].fi1  = Node_Array_fi[i].fi_w - Node_Array[i].fi_err_DD ;  // fi1  	
		Node_Array_fi[i].h1   = Node_Array_fi[i].hw   - Node_Array[i].h_err   ;	 // h_w - h_err0
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// 5) compute err between point (P0) and (P)
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 	 
		Node_Array_fi[i].err_0_fi = Node_Array_fi[i].fi_w - Node_Array_fi[i].fi	 ; // fiw - fi = fi0 - fi
		Node_Array_fi[i].err_0_h  = Node_Array_fi[i].hw   - Node_Array_fi[i].h	 ; // hw - h = h0  - h
		Node_Array_fi[i].err_0_X  = Node_Array_fi[i].X0   - Node_Array_fi[i].Xp	 ; // X0 - Xp
		Node_Array_fi[i].err_0_Y  = Node_Array_fi[i].Y0   - Node_Array_fi[i].Yp	 ; 
		Node_Array_fi[i].err_0_Z  = Node_Array_fi[i].Z0   - Node_Array_fi[i].Zp	 ; 
		//Node_Array_fi[i].write_err_0(ERR_0_File) ;
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// 6) compute (fi1,lamda,h1) for Point P1 (fi_w+err0, lamda, hw-err0) 
		// Second Point P1  to (X1,Y1,Z1)
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		Node_Array[i].init();
		Node_Array[i].ID    =  	Node_Array_fi[i].ID;
		Node_Array[i].fi    =  	Node_Array_fi[i].fi1 ;
		Node_Array[i].lamda =  	Node_Array_fi[i].lamda_XYZ ;
		Node_Array[i].h     =   Node_Array_fi[i].h1 ;
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// 7) compute (X1, Y1, Z1) for Point P1 from(fi1,lamda,h1) 
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation	get(X1, Y1, Z1)
		Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		Node_Array[i].compute_h_err();
		Node_Array[i].compute_fi_err() ;
		// store P1			  
		Node_Array_fi[i].X1	  = Node_Array[i].Xp  ;
		Node_Array_fi[i].Y1	  = Node_Array[i].Yp  ;
		Node_Array_fi[i].Z1	  = Node_Array[i].Zp  ;	
		// Compute P2
		Node_Array_fi[i].fi2  = Node_Array_fi[i].fi_w - Node_Array[i].fi_err_DD ;  // fi1  	
		Node_Array_fi[i].h2   = Node_Array_fi[i].hw   - Node_Array[i].h_err   ;	 // h_w - h_err0
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// 8) compute err between point (P1) and (P)
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 	 
		Node_Array_fi[i].err_1_fi = Node_Array_fi[i].fi1  - Node_Array_fi[i].fi	 ; // fi1 - fi
		Node_Array_fi[i].err_1_h  = Node_Array_fi[i].h1   - Node_Array_fi[i].h	 ; // h1 - h
		Node_Array_fi[i].err_1_X  = Node_Array_fi[i].X1   - Node_Array_fi[i].Xp	 ; // X1 - Xp
		Node_Array_fi[i].err_1_Y  = Node_Array_fi[i].Y1   - Node_Array_fi[i].Yp	 ; 
		Node_Array_fi[i].err_1_Z  = Node_Array_fi[i].Z1   - Node_Array_fi[i].Zp	 ; 
		//Node_Array_fi[i].write_err_1(ERR_1_File) ;
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// 9) compute (fi2,lamda,h2) for Point P2 (fi_w+err1, lamda, hw-err1) 
		// Third Point P2 () to (X2,Y2,Z2)
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 		  
		Node_Array[i].init();
		Node_Array[i].ID    =   Node_Array_fi[i].ID  ; 
		Node_Array[i].fi    =  	Node_Array_fi[i].fi2 ; // fi2 
		Node_Array[i].lamda =  	Node_Array_fi[i].lamda_XYZ ;
		Node_Array[i].h     =   Node_Array_fi[i].h2 ;	// h2
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// 10) compute (X2, Y2, Z2) for Point P2 from(fi2,lamda,h2) 
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		Node_Array[i].fi2XYZ(WGS84_Datum.a,  WGS84_Datum.f, WGS84_Datum.ee); // geodesy computation	get(X2, Y2, Z2)
		Node_Array[i].XYZ2fi(WGS84_Datum.a,  WGS84_Datum.b, WGS84_Datum.epson);
		// store P2		
		Node_Array_fi[i].X2	  = Node_Array[i].Xp  ;
		Node_Array_fi[i].Y2	  = Node_Array[i].Yp  ;
		Node_Array_fi[i].Z2	  = Node_Array[i].Zp  ;	
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// 11) compute err between point (P2) and (P)
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 	   
		Node_Array_fi[i].err_2_fi = Node_Array_fi[i].fi2  - Node_Array_fi[i].fi	 ; 
		Node_Array_fi[i].err_2_h  = Node_Array_fi[i].h2   - Node_Array_fi[i].h	 ; 
		Node_Array_fi[i].err_2_X  = Node_Array_fi[i].X2   - Node_Array_fi[i].Xp	 ; 
		Node_Array_fi[i].err_2_Y  = Node_Array_fi[i].Y2   - Node_Array_fi[i].Yp	 ; 
		Node_Array_fi[i].err_2_Z  = Node_Array_fi[i].Z2   - Node_Array_fi[i].Zp	 ; 
		//Node_Array_fi[i].write_err_2(ERR_2_File) ;
		Node_Array_fi[i].write(out);
		Node_Array_fi[i].write_results(rslt);
	}
	return;
}
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GEOXYZ2BLH_F, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GEOXYZ2BLH_F));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GEOXYZ2BLH_F));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GEOXYZ2BLH_F);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Geodetic);
   return TRUE;
}
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;			
			break;	
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
// ----------------------------------------------------------------------------------------------------------
// Message handler for Dialog1.
// Key entry for solution
// ----------------------------------------------------------------------------------------------------------
 INT_PTR CALLBACK Geodetic(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{		
	char  major[100];	 // to store user input of semi-major axis (a) of used ellipse
	char  ff[100]   ;	 // to store user input of inverse-flattening (f) of used ellipse  	
	char  out_file[200]; // to store user input for output file name
	char  validate_result[200]; // to store user input for err-results file name

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:	   // initial value when creating dialog 	
		SetDlgItemText(hDlg, IDC_EDIT2, "6378137.0");		// default value for (a) WGS84
		SetDlgItemText(hDlg, IDC_EDIT3, "298.257223563");	// default value for (f) WGS84 
		SetDlgItemText(hDlg, IDC_EDIT4, OUTPUT_FILE);	// default value for (output file name)	
		SetDlgItemText(hDlg, IDC_EDIT5, "D:/Results.txt");	// default value for (output file name)	
		return (INT_PTR)TRUE;

	case WM_COMMAND:  
		if (LOWORD(wParam) == IDC_SELECT)	  // Select Input File
		{
			DoFileSelect(hwnd);	  // open input file
			SetDlgItemText(hDlg, IDC_EDIT1, szFileName);		// Input file name
			Con.init();			  //
			Count_Records(Con);	  // count records and store in [Con.Nodes]
			Node_Array    = new Node[Con.Nodes];	// create new array of nodes with size [Con.Nodes]
			return (INT_PTR)TRUE; 			 
		}
		if (LOWORD(wParam) == IDOK)
		{
			GetDlgItemText(hDlg, IDC_EDIT2, major, 100);		// new value for (a)  as char
			GetDlgItemText(hDlg, IDC_EDIT3, ff, 100);			// new value for (f)  as char
			GetDlgItemText(hDlg, IDC_EDIT4, out_file, 200);			// new value for (out_file)  as char
			GetDlgItemText(hDlg, IDC_EDIT5, validate_result, 200);			// new value for (out_file)  as char
			//WGS84_Datum.init();
			WGS84_Datum.a =  atof(major);  						//  new value for (a)  as double
			WGS84_Datum.f =  atof(ff);   						//  new value for (f)  as double
			WGS84_Datum.init_Ellipsoid();						//  compute values for datum 
			#undef OUTPUT_FILE
			#define OUTPUT_FILE out_file

			if (IsDlgButtonChecked(hDlg, IDC_RADIO1))		  //IDC_RADIO1	==>> selected  (fi2XYZ)
			{	
				write_Header_fi2X(out_file);
				Con.write(out_file);
				//write_Header_Solution(SOLUTION_FILE);
				write_Header_XYZ(validate_result);
				//write_Header_rZ(rZ_FILE);
				//write_Header_err(err_FILE);
				WGS84_Datum.write(out_file);
				Fill_Input_Array_fi(Con.Nodes);		// fill input data inside array
				fi2XYZ(out_file, validate_result);
			}
			if (IsDlgButtonChecked(hDlg, IDC_RADIO2))		  //IDC_RADIO2	==>> selected  (XYZ2fi)
			{  
				write_Header_X2fi(out_file);
				WGS84_Datum.write(out_file);
				Con.write(out_file);
				//write_Header_Solution(SOLUTION_FILE);
				//write_Header_XYZ(XYZ_FILE);
				write_Header_BLh(validate_result);
				Fill_Input_Array_XYZ(Con.Nodes);
				XYZ2fi(out_file, validate_result);				
			} 
			//if (IsDlgButtonChecked(hDlg, IDC_RADIO3))		  //IDC_RADIO3	==>> selected  (rZ2Lh)
			//{					
			//	write_Header_rZ2fi(out_file);
			//	WGS84_Datum.write(out_file);
			//	Fill_Input_Array_rZ(Con.Nodes);
			//	write_Header_rZ(validate_result);
			//	rZ2fi() ;
			//}
			if (IsDlgButtonChecked(hDlg, IDC_RADIO4))		  //IDC_RADIO4	==>> selected  (validate)
			{
				Node_Array_fi = new Node_fi[Con.Nodes];	// create new array of nodes with size [Con.Nodes]
				Fill_Input_Array_fi(Con.Nodes);		    // fill input data inside array
				//write_Header_err0(ERR_0_File);
				//write_Header_err1(ERR_1_File);
				//write_Header_err2(ERR_2_File);
				write_Header_validate(OUTPUT_FILE);
				WGS84_Datum.write(OUTPUT_FILE);
				write_Header_err012(validate_result);

				Validate_XYZ2fi(out_file, validate_result) ;
			}
			MessageBox(hwnd, "Done", "Done",MB_OK | MB_ICONINFORMATION);
			return (INT_PTR)TRUE;
		}
		
		if (LOWORD(wParam) == IDC_BUTTON1)
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, About); 			
			return (INT_PTR)TRUE; 			
		}
		//break;

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			DestroyWindow(hwnd);				
			//return (INT_PTR)TRUE; 
			PostQuitMessage(0);
		}
		break;
	}
	return (INT_PTR)FALSE;
}	