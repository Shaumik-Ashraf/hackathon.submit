//main.c 
//main cgi script will accept form and return html with processed data

#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<errno.h>


#include"getdata.h"
//#include"mathstats.h"
//#include"crthtml.h"

int main(void) {
	
	//printf("Content-type:text/html\n\n");
	printf("Content-type:text/plain\n\n");
	
	//vars----------------------
	double** data;
	int* rowsizes;
	int rownum;
	
	double* row_mean;
	double* row_median;
	int* row_ismode;
	double* row_mode;
	double* row_variance;
	double* row_standard_deviation;
	
	double total_mean;
	double total_median;
	int total_ismode;
	double total_mode;
	double total_variance;
	double total_standard_deviation;
	
	char errorbuf[256];
	
	int i, j;
	
	char *s;
	
	printf("Hello world\n");
	printf("req_meth:%s\n", s=getenv("REQUEST_METHOD"));
	

	//----------------------
	if( getdata(&data, &rowsizes, &rownum, errorbuf)==EXIT_FAILURE ) {
		//crt_error_html(errorbuf);
		printf("Error: %s", errorbuf);
		return(1);
	}
	
	//TESTING---------------------------
	printf("numrows: %i\n\n", rownum);
	for(i=0; i<rownum; i++) {
		printf("row %i size: %i\n", i, rowsizes[i]);
	}
	printf("\ndata:\n");
	for(i=0; i<rownum; i++) {
		for(j=0; j<rowsizes[i]; j++) {
			printf("%lf, ", data[i][j]);
		}
		printf("\n");
	}
	printf("\ndone\n");
	/*
	
	//----------------------
	for(i=0; i<rownum; i++) {
		row_mean[i] = calc_row_mean(data[i], rowsizes[i]);
		//...
	}
	
	total_mean = calc_total_mean(data, rowsizes, rownum);
	//...
	
	
	//----------------------
	
	//maybe define crthtml here to avoid passing all the parameters!!!
	*/
	
	return(0);
	
}