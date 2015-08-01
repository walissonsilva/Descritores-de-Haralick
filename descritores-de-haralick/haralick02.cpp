#include "haralick02.h"
#include "stdio.h"
#include <iostream>


double *allocate_vector (int nl, int nh) {
        double *v;

        v = (double *) calloc (1, (unsigned) (nh - nl + 1) * sizeof (double));
        if (!v) fprintf (stderr, "memory allocation failure (allocate_vector) "), exit (1);

        return v - nl;
}

/* Allocates a double matrix with range [nrl..nrh][ncl..nch] */
double **allocate_matrix (int nrl, int nrh, int ncl, int nch)
{
        int i;
        double **m;

        /* allocate pointers to rows */
        m = (double **) malloc ((unsigned) (nrh - nrl + 1) * sizeof (double *));
        if (!m) fprintf (stderr, "memory allocation failure (allocate_matrix 1) "), exit (1);
        m -= ncl;

        /* allocate rows and set pointers to them */
        for (i = nrl; i <= nrh; i++) {
                m[i] = (double *) malloc ((unsigned) (nch - ncl + 1) * sizeof (double));
                if (!m[i]) fprintf (stderr, "memory allocation failure (allocate_matrix 2) "), exit (2);
                m[i] -= ncl;

        }

        /* return pointer to array of pointers to rows */
        return m;
}
double **Liberar_matriz_real (int m, int n, double **v)
{
  int  i;  /* variavel auxiliar */
  if (v == NULL){
      printf("Nao foi possível desalocar - Erro 01\n");
      return (NULL);
  }
  if (m < 1 || n < 1) {  /* verifica parametros recebidos */
     printf ("** Erro: Parametro invalido **\n");
     return (v);
     }
  for (i=0; i<m; i++) free (v[i]); /* libera as linhas da matriz */
  free (v);      /* libera a matriz */
  return (NULL); /* retorna um ponteiro nulo */
}

double correlac (double **P, int Ng) {
        int i, j;
        double sum_sqrx = 0, sum_sqry = 0, tmp, *px;
        double meanx =0 , meany = 0 , stddevx, stddevy;

        px = allocate_vector (0, Ng);
        for (i = 0; i < Ng; ++i)
                px[i] = 0;

        /*
        * px[i] is the (i-1)th entry in the marginal probability matrix obtained
        * by summing the rows of p[i][j]
        */
        for (i = 0; i < Ng; ++i)
                for (j = 0; j < Ng; ++j)
                        px[i] += P[i][j];


        /* Now calculate the means and standard deviations of px and py */
        /*- fix supplied by J. Michael Christensen, 21 Jun 1991 */
        /*- further modified by James Darrell McCauley, 16 Aug 1991
        *     after realizing that meanx=meany and stddevx=stddevy
        */
        for (i = 0; i < Ng; ++i) {
                meanx += px[i]*i;
                sum_sqrx += px[i]*i*i;
        }

        /* M. Boland meanx = meanx/(sqrt(Ng)); */
        meany = meanx;
        sum_sqry = sum_sqrx;
        stddevx = sqrt (sum_sqrx - (meanx * meanx));
        stddevy = stddevx;

        /* Finally, the correlation ... */
        for (tmp = 0, i = 0; i < Ng; ++i)
                for (j = 0; j < Ng; ++j)
                          tmp += i*j*P[i][j];

        free(px);
        if (stddevx * stddevy==0) return(1);  /* protect from error */
        else return (tmp - meanx * meany) / (stddevx * stddevy);
}
double homogen (double **P, int Ng) {
        int i, j;
        double idm = 0;

        for (i = 0; i < Ng; ++i)
                for (j = 0; j < Ng; ++j)
                        idm += P[i][j] / (1 + (i - j) * (i - j));

        return idm;
}
double savg (double **P, int Ng) {
        int i, j;
        double savg = 0;
        double *Pxpy = allocate_vector (0, 2*Ng);

        for (i = 0; i <= 2 * Ng; ++i)
                Pxpy[i] = 0;

        for (i = 0; i < Ng; ++i)
                for (j = 0; j < Ng; ++j)
                  /* M. Boland Pxpy[i + j + 2] += P[i][j]; */
                  /* Indexing from 2 instead of 0 is inconsistent with rest of code*/
                  Pxpy[i + j] += P[i][j];

        /* M. Boland for (i = 2; i <= 2 * Ng; ++i) */
        /* Indexing from 2 instead of 0 is inconsistent with rest of code*/
        for (i = 0; i <= (2 * Ng - 2); ++i)
                savg += i * Pxpy[i];

        free (Pxpy);
        return savg;
}

double f13_icorr (double **P, int Ng) {
        int i, j;
        double *px, *py;
        double hx = 0, hy = 0, hxy = 0, hxy1 = 0, hxy2 = 0;

        px = allocate_vector (0, Ng);
        py = allocate_vector (0, Ng);

        /* All /log10(2.0) added by M. Boland */

        /*
        * px[i] is the (i-1)th entry in the marginal probability matrix obtained
        * by summing the rows of p[i][j]
        */
        for (i = 0; i < Ng; ++i) {
                for (j = 0; j < Ng; ++j) {
                  px[i] += P[i][j];
                  py[j] += P[i][j];
                }
        }

        for (i = 0; i < Ng; ++i)
                for (j = 0; j < Ng; ++j) {
                        hxy1 -= P[i][j] * log10 (px[i] * py[j] + EPSILON)/log10(2.0);
                        hxy2 -= px[i] * py[j] * log10 (px[i] * py[j] + EPSILON)/log10(2.0);
                        hxy -= P[i][j] * log10 (P[i][j] + EPSILON)/log10(2.0);
                }

        /* Calculate entropies of px and py */
        for (i = 0; i < Ng; ++i) {
                hx -= px[i] * log10 (px[i] + EPSILON)/log10(2.0);
                hy -= py[i] * log10 (py[i] + EPSILON)/log10(2.0);
        }

        free(px);
        free(py);
        return (sqrt (fabs (1 - exp (-2.0 * (hxy2 - hxy)))));
}

/* Compute gray-tone spatial dependence matrix */
double** CoOc_simetrH_Mat (int **grays, int rows, int cols, int tone_count)
{

        int  itone, jtone;
        int count=0; /* normalizing factor */
        //printf("entrou ");
        double** matrix = allocate_matrix (0, tone_count, 0, tone_count);

        /* zero out matrix */
        for (itone = 0; itone < tone_count; ++itone){
                for (jtone = 0; jtone < tone_count; ++jtone){
                        matrix[itone][jtone] = 0;
                }
        }

        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                if (j>0){

                    matrix[grays[i][j]][grays[i][j-1]]++;
                }
            }
        }
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                if (j+1<cols)
                    matrix[grays[i][j]][grays[i][j+1]]++;

            }
        }
        for(int i=0; i<tone_count; i++)
        {
            for(int j=0; j<tone_count; j++)
            {
                count+=matrix[i][j];
            }
        }
        /* normalize matrix */
        for (itone = 0; itone < tone_count; ++itone){
            for (jtone = 0; jtone < tone_count; ++jtone){
                if (count==0)   /* protect from error */
                    matrix[itone][jtone]=0;
                else{

                    //cout<<matrix[itone][jtone]<<" ";
                    matrix[itone][jtone] /= count;
                   // cout<<matrix[itone][jtone]<<" ";
                }
            }
            //cout<<"\n";
        }

        return matrix;
}

