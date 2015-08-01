#ifndef HARALICK02_H
#define HARALICK02_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>

#define EPSILON 0.000000001

double *allocate_vector (int nl, int nh);
double **allocate_matrix (int nrl, int nrh, int ncl, int nch);
double correlac (double **P, int Ng);
double homogen (double **P, int Ng);
double savg (double **P, int Ng);
double f13_icorr (double **P, int Ng);
double **Liberar_matriz_real (int m, int n, double **v);

double** CoOc_simetrH_Mat (int **grays, int rows, int cols, int tone_count);

#endif // HARALICK2_H
