#ifndef STATFUNCS_H
#define STATFUNCS_H

double get_mean(double*, int);

void make_intervals(double*, double, int, double**);

void ret_buckets(int, double*, int, int**, double**);

double *get_quartiles(double*, int);

#endif
