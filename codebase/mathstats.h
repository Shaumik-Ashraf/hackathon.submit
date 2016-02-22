//mathstats.h
//define functions for calculating stats

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#ifndef _MATHSTATS_H_
#define _MATHSTATS_H_

extern double calc_row_mean(double* dataset, int length);
extern double calc_row_variance(double* dataset, int length, double mean);
//extern double calc_row_standard_deviation(double* dataset, int length);  //I can just take sqrt(variance) instead


extern double calc_total_mean(double** dataset, int* rowsizes, int nrows);
extern double calc_total_variance(double** dataset, int* rowsizes, int nrows, double mean);


extern double calc_sd(double v);

#endif