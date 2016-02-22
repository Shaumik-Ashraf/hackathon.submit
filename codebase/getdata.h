//getdata.h
//defines getdata function
//processes form request and cgi data to double data matrix

#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
//#include<math.h>

#ifndef _GETDATA_H_
#define _GETDATA_H_

int getdata(double*** matrixp, int** rowsizesp, int* rownump, char* error_msg);

#endif

