//mathstats.c
//define functions for calculating stats

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"mathstats.h"

double calc_row_mean(double* dataset, int length) {
	
	int i;
	double sum=0;
	
	for(i=0; i<length; i++) sum += dataset[i];
	
	return( sum / (double)length );
	
}

double calc_row_variance(double* dataset, int length, double mean) {
	
	int i;
	int sum;
	
	for(i=0; i<length; i++) {
		sum += ( (dataset[i]-mean)*(dataset[i]-mean) );
	}
	
	return( sum/(double)length );
	
}


double calc_total_mean(double** dataset, int* rowsizes, int nrows) {
	
	int i, j;
	double sum=0;
	int number_of_elements=1;
	
	for(i=0; i<nrows; i++) {
		for(j=0; j<rowsizes[i]; j++) {
			sum += dataset[i][j];
			//number_of_elements+=1;
		}
	}
	
	return( sum / (double)number_of_elements );
	
}

double calc_total_variance(double** dataset, int* rowsizes, int nrows, double mean) {
	
	int i, j;
	double sum=0;
	int number_of_elements=1;
	
	for(i=0; i<nrows; i++) {
		for(j=0; j<rowsizes[i]; j++) {
			sum += ( (dataset[i][j]-mean)*(dataset[i][j]-mean) );
			//number_of_elements+=1;
		}
	}
	
	return( sum / (double)number_of_elements );
	
}


//calculate standard deviation
double calc_sd(double v) {
	
	return( sqrt(v) );
	
}
