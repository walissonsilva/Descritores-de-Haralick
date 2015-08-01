#ifndef HARALICK01_H
#define HARALICK01_H

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <vector>

using namespace std;
using namespace cv;

class Haralick01 { // RNA
    public:
        Haralick01(int img[], int nCores, int nL, int nC);
        void novaImagem(IplImage *img, int nCores);
        void coOcorN();
        void coOcorS();
        void coOcorL();
        void coOcorO();
        void simetriaH();
        void simetriaV();
        void normalizaH();
        void normalizaV();
        float miH();
        float miV();
        float sigmaH();
        float sigmaV();
        float px(int i);
        float py(int j);
        float *p_mais();
        float *p_menos();
        double ASM();
        double energy();
        float contrastH();
        float correlacH();
        float correlacV();
        double varience();
        float homogeneityH();
        double soma_media();
        double sum_varience();
        double sum_entropy();
        float entropy();
        double difference_varience();
        double difference_entropy();
        double F12();
        double F13();
        double dissimilarity();
        //double media();
        //double momento_produto();
        //double media_soma();
        ~Haralick01();
        int imgL, imgC;
        int **ent;
        int altura, largura;
        int **cN, **cS, **cL, **cO;
        int **sH, **sV;
        float **normaH, **normaV;

    //private:
        int** alloca_matriz (int nrh, int nch);
        int** libera_matriz (int m, int n, int **v);
        float** alloca_matriz_f (int nrh, int nch);
        float** libera_matriz_f (int m, int n, float **v);

};
#endif // HARALICK01_H
