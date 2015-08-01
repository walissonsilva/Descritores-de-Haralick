#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "haralick01.h"
#include "haralick02.h"

using namespace cv;
using namespace std;

int main(){

/// BLOCO 1 >> ABRE UMA NOVA IMAGEM COLORIADA (img) E A CONVERTE PARA TONS DE CINZA (imgCinza)
    IplImage* img;
    /// INSIRA O DIRETÓRIO E O NOME DA IMAGEM NA VARIÁVEL dir[]
    const char dir[] = "";
    img =  cvLoadImage(dir, CV_LOAD_IMAGE_COLOR);
    if(!img)
    {
        cout <<  "Imagem não encontrada!" << endl ;
        return -1;
    }
    IplImage* imgCinza = cvLoadImage(dir, 0);
    Mat img2 = imread(dir, 0);
    Mat image_out(Size(512, 512), CV_8U);
    Mat image_final(Size(512, 512), CV_8U);
/// FIM DE BLOCO 1

/// BLOCO 2 >> CARREGA A POSIÇÃO DE CADA PIXEL (DE INTERESSE) DA IMAGEM
    int nAmostras = 0;
    vector<int> gx, gy;


    ///Captura todos os pixels da imagem para calcular os descritores em cada um deles
    for (int i = 0; i < img2.rows; i++){
        for (int j = 0; j < img2.cols; j++){
            if (img2.at<uchar>(i, j) != 255){
                nAmostras++;
                gx.push_back(j);
                gy.push_back(i);
            }
            image_out.at<uchar>(i, j) = 255;
            image_final.at<uchar>(i, j) = 255;
        }
    }

    nAmostras--;

    int **pontos;
    pontos = (int**) malloc( nAmostras * sizeof (int*));
        for (int i = 0; i < nAmostras; ++i)
           pontos[i] = (int*) malloc( 2 * sizeof (int));

    for(int i = 0; i < nAmostras; i++)
    {
        //getAmostra(imgCinza, img2);
        pontos[i][0] = gy[i];//gY;
        pontos[i][1] = gx[i];//gX;
        cout << i+1 << "Posicao (" << pontos[i][0] << "," << pontos[i][1] << ") salva!\n\n";
    }
/// FIM DE BLOCO 2

/// BLOCO 3 >> EXTRAI A VINZINHAÇA DE CADA PIXEL (DE INTERESSE) NA IMAGEM
    int masc = 5; // Tamanho da vizinhança: masc x masc
    int** amostras = (int**) malloc( nAmostras * sizeof (int*));
        for (int i = 0; i < nAmostras; ++i)
           amostras[i] = (int*) malloc( masc*masc * sizeof (int));

    float f = (masc/2.0)-0.5;
    int fator = (int)f;
    int x,y, cont;
    for(int i = 0; i < nAmostras; i++)
    {
        y = pontos[i][0]; // linha do pixel
        x = pontos[i][1]; // coluna do pixel
        cont=0;
        for(int a=0; a<masc; a++)
        {
            for(int b=0; b<masc; b++){
                amostras[i][cont] = (int)cvGetReal2D(imgCinza, y+a-fator, x+b-fator);
                cont++;
            }
        }
    }
/// FIM DE BLOCO 3

///Cálculo dos descritores de Haralick

    for (int k = 0; k < nAmostras; k++){
        Haralick01 obj(amostras[k], 256, masc, masc);

        IplImage* testeMLP = cvCreateImage(cvSize(masc,masc) , IPL_DEPTH_8U , 1);
        CvScalar pix;

        for(int i=0; i<masc; i++){
            for(int j=0; j<masc; j++){
                pix.val[0] = (int)(obj.ent[i][j]);
                cvSet2D(testeMLP, i, j, pix);
            }
        }

        // Descritores de Haralick (haralick01):
        obj.simetriaH();
        obj.normalizaH();
        //float F3 = obj.correlacH();
        //float F2 = obj.contrastH();
        //float F9 = obj.entropy();
        //float F5 = obj.homogeneityH();
        //double ener = obj.energy();
        //double diss = obj.dissimilarity();
        double F6 = obj.soma_media();
        //double F4 = obj.varience();
        //double F1 = obj.ASM();
        double F7 = obj.sum_varience();
        //double F8 = obj.sum_entropy();
        //double F10 = obj.difference_varience();
        //double F11 = obj.difference_entropy();
        //double F12 = obj.F12();
        //double F13 = obj.F13();

        cout << "F6 = " << F6 << "  F7 = " << F7 << endl;

        if ((F6 >= 10) && (F6 <= 140)){
            if ((F7 >= 10) && (F7 <= 18000)){
                image_out.at<uchar>(gy[k], gx[k]) = 0;
            }
        } else {
            image_out.at<uchar>(gy[k], gx[k]) = 255;
        }

        cvNamedWindow("Output", WINDOW_AUTOSIZE);
        cvShowImage("Output", testeMLP);
        cvWaitKey(0);
        cvDestroyWindow("Output");
    }

    cvNamedWindow("IMAGEM", CV_WINDOW_AUTOSIZE);
    imshow("IMAGEM", img2);
    cvNamedWindow("SAIDA", CV_WINDOW_AUTOSIZE);
    imshow("SAIDA", image_final);
    cvWaitKey(0);

    getchar();
    return 0;
}
