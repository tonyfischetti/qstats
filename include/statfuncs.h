#ifndef STATFUNCS_H
#define STATFUNCS_H

double get_mean(double*, int);

double *get_quartiles(double*, int);

int get_uniques(double*, int, double**);

void get_simple_frequencies(double*, int, int, int**);

#endif
