//main.c 
//main cgi script will accept form and return html with processed data

//html output is being hardcoded rather than creating and using crthtml.h

#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<errno.h>
#include<math.h>

#include"getdata.h"
#include"mathstats.h"
//#include"crthtml.h"

int main(void) {
	
	printf("Content-type:text/html\n\n");
	//printf("Content-type:text/plain\n\n");
	
	//vars----------------------
	double** data;
	int* rowsizes;
	int rownum;
	
	double* row_mean;
	//double* row_median;
	//int* row_ismode;
	//double* row_mode;
	double* row_variance;
	double* row_standard_deviation;
	
	double total_mean;
	//double total_median;
	//int total_ismode;
	//double total_mode;
	double total_variance;
	double total_standard_deviation;
	
	char errorbuf[256];
	
	int i, j;
	
	char *s;
	
	//printf("Hello world\n");
	//printf("req_meth:%s\n", s=getenv("REQUEST_METHOD"));
	

	//----------------------
	if( getdata(&data, &rowsizes, &rownum, errorbuf)==EXIT_FAILURE ) {
		//crt_error_html(errorbuf);
		printf("Error: %s", errorbuf);
		return(1);
	}
	
	/*
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
	*/
	
	row_mean = (double*)malloc(rownum*sizeof(double));
	if( row_mean==NULL ) {
		//ERROR HANDLING
		return(1);
	}
	
	row_variance = (double*)malloc(rownum*sizeof(double));
	if( row_variance==NULL ) {
		//ERROR HANDLING
		return(1);
	}
	
	row_standard_deviation = (double*)malloc(rownum*sizeof(double));
	if( row_standard_deviation==NULL ) {
		//ERROR HANDLING
		return(1);
	}
	
	//----------------------
	for(i=0; i<rownum; i++) {
		row_mean[i] = calc_row_mean(data[i], rowsizes[i]);
		row_variance[i] = calc_row_variance(data[i], rowsizes[i], row_mean[i]);
		row_standard_deviation[i] = calc_sd(row_variance[i]);
	}
	
	total_mean = calc_total_mean(data, rowsizes, rownum);
	total_variance = calc_total_variance(data, rowsizes, rownum, total_mean);
	//total_standard_deviation = calc_sd(total_variance);
	total_standard_deviation = sqrt(total_variance);
	
	//----------------------
	printf("<!DOCTYPE html>\n");
	printf("<html lang=\"en\">\n");
	printf("<head>\n");
	printf("<title>Team Mash</title>\n");
	printf("<meta charset=\"utf-8\"></meta>");
	//INSERT OTHER HERE
	printf("<link rel=\"stylesheet\" type=\"text/css\" href=\"mashstyle.css\"> \n");
	//<link rel="stylesheet" type="text/css" href="mashstyle.css">
	printf("</head>\n");

	printf("<body>\n");
	printf("<div>\n");
	printf("<h1>Mash!</h1>\n");
	
	printf("<div class=\"tabcontainer\">");
	printf("<h2>Data</h2>\n");
	
	printf("<table class=\"tab\">\n");
	for(i=0; i<rownum; i++) {
		printf("<tr>\n\t");
		for(j=0; j<rowsizes[i]; j++) {
			printf("<td>%lf</td>\n\t", data[i][j]);
		}
		printf("</tr>\n");
	}
	printf("</table>\n");
	printf("</div>");
	
	printf("<br><br>\n");
	
	printf("<div class=\"tabcontainer\">");
	printf("<h2>Stats</h2>\n");
	printf("<table class=\"tab\">\n");
	printf("<tr>\n\t");
		printf("<th>Row</th>\n\t");
		printf("<th>Mean</th>\n\t");
		printf("<th>Variance</th>\n\t");
		printf("<th>Standard Deviation</th>\n\t");
	printf("</tr>\n");
	for(i=0; i<rownum; i++) {
		printf("<tr>\n\t");
			printf("<td>%i</td>\n\t", i);
			printf("<td>%lf</td>\n\t", row_mean[i]);
			printf("<td>%lf</td>\n\t", row_variance[i]);
			printf("<td>%lf</td>\n\t", row_standard_deviation[i]);
		printf("</tr>\n");
	}
	printf("<tr>\n\t");
		printf("<th>Table</th>\n\t");
		printf("<td>%lf</td>\n\t", total_mean);
		printf("<td>%lf</td>\n\t", total_variance);
		printf("<td>%lf</td>\n\t", total_standard_deviation);
	printf("</tr>\n");
	printf("</table>\n");
	printf("</div>");
	
	printf("</div>\n");
	printf("</body>\n");
	printf("</html>\n");

	return(0);
	
}