#include "haralick01.h"
#include <iostream>

#define NG 256
#define NG2 512

// ALOCA UMA MATRIZ COM O TAMANHO DA MÁSCARA; RETORNA UM PONTEIRO DE POTEIRO REFERENTE À PRIMEIRA POSIÇÃO DA MATRIZ
int** Haralick01::alloca_matriz (int nrh, int nch) // RECEBE O TAMANHO DA MÁSCARA (masc); nrh == nch
{
    int i;
    int **m;
    /* allocate pointers to rows */
    m = (int **) malloc ((unsigned) (nrh+1) * sizeof (int *));
    if (!m) fprintf (stderr, "memory allocation failure (allocate_matrix 1) "), exit (1);

    /* allocate rows and set pointers to them */
    for (i = 0; i <= nrh; i++) {
            m[i] = (int *) malloc ((unsigned) (nch+1) * sizeof (int));
            if (!m[i]) fprintf (stderr, "memory allocation failure (allocate_matrix 2) "), exit (2);
    }
    /* return pointer to array of pointers to rows */
    return m;
}

// LIBERA A MATRIZ ALOCADA ANTERIORMENTE
int** Haralick01::libera_matriz (int m, int n, int **v)
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

// ALOCA UMA MATRIZ DE FLOAT COM O TAMANHO DA MÁSCARA (USADA PARA A NORMALIZAÇÃO DA IMAGEM)
float** Haralick01::alloca_matriz_f (int nrh, int nch) // RECEBE O TAMANHO DA MÁSCARA (masc); nrh == nch
{
    int i;
    float **m;
    /* allocate pointers to rows */
    m = (float **) malloc ((unsigned) (nrh+1) * sizeof (float *));
    if (!m) fprintf (stderr, "memory allocation failure (allocate_matrix 1) "), exit (1);

    /* allocate rows and set pointers to them */
    for (i = 0; i <= nrh; i++) {
            m[i] = (float *) malloc ((unsigned) (nch+1) * sizeof (float));
            if (!m[i]) fprintf (stderr, "memory allocation failure (allocate_matrix 2) "), exit (2);
    }
    /* return pointer to array of pointers to rows */
    return m;
}

// LIBERA A MATRIZ DE FLOAT ALOCADA ANTERIORMENTE
float** Haralick01::libera_matriz_f (int m, int n, float **v)
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


// CONSTRUTOR DO OBJETO Haralick01
Haralick01::Haralick01(int img[], int nCores, int nL, int nC)
{
    this->altura=nCores;
    this->largura=nCores;
    this->imgL = nL;
    this->imgC = nC;

    this->ent = alloca_matriz(imgL, imgC);

    int cont=0;
    for(int i=0; i<imgL; i++)
    {
        for(int j=0; j<imgC; j++)
        {
            ent[i][j] = img[cont];
            cont++;
        }
    }
    this->cN=NULL; this->cS=NULL; this->cL=NULL; this->cO=NULL;
    this->sH=NULL; this->sV=NULL;
    this->normaH=NULL; this->normaV=NULL;
}


// RECEBE O PONTEIRO DO INÍCIO DA IMAGEM E ATRIBUI CADA VALOR DA IMAGEM (0-255) PARA A MATRIZ ent[][].
void Haralick01::novaImagem(IplImage *img, int nCores)
{
    this->altura=nCores;
    this->largura=nCores;
    this->imgL = img->height;
    this->imgC = img->width;

    for(int i=0; i<imgL; i++)
    {
        for(int j=0; j<imgC; j++)
        {
            ent[i][j] = cvGetReal2D(img,i,j);
        }
    }
}

// Matriz de co-ocorrência em relação ao norte
void Haralick01::coOcorN()
{
    if(this->cN == NULL)
        this->cN = this->alloca_matriz(this->altura, this->largura);

    for(int i=0; i<altura; i++)
        for(int j=0; j<largura; j++)
            cN[i][j] = 0;

    for(int i=0; i<this->imgL; i++)
    {
        for(int j=0; j<this->imgC; j++)
        {
            if (i>0){
                cN[ent[i][j]][ent[i-1][j]]++;
            }
        }
    }
}

// Matriz de co-ocorrência em relação ao Sul
void Haralick01::coOcorS()
{
    if(this->cS==NULL)
        this->cS = this->alloca_matriz(this->altura, this->largura);

    for(int i=0; i<altura; i++)
        for(int j=0; j<largura; j++)
            cS[i][j] = 0;

    for(int i=0; i<this->imgL; i++)
    {
        for(int j=0; j<this->imgC; j++)
        {
            if (i+1<this->imgL)
                cS[ent[i][j]][ent[i+1][j]]++;
        }
    }
}

// Matriz de co-ocorrência em relação ao Leste
void Haralick01::coOcorL()
{
    if(this->cL==NULL)
        this->cL = this->alloca_matriz(this->altura, this->largura);

    for(int i=0; i<altura; i++)
        for(int j=0; j<largura; j++)
            cL[i][j] = 0;

    for(int i=0; i<this->imgL; i++)
    {
        for(int j=0; j<this->imgC; j++)
        {
            if (j+1<this->imgC)
               cL[ent[i][j]][ent[i][j+1]]++;
        }
    }
}

// Matriz de co-ocorrência em relação ao Oeste
void Haralick01::coOcorO()
{
    if(this->cO==NULL)
        this->cO = this->alloca_matriz(this->altura, this->largura);

    for(int i=0; i<altura; i++)
        for(int j=0; j<largura; j++)
            cO[i][j] = 0;

    for(int i=0; i<this->imgL; i++)
    {
        for(int j=0; j<this->imgC; j++)
        {
            if (j>0)
                cO[ent[i][j]][ent[i][j-1]]++;
        }
    }
}

// A MATRIZ DE SIMETRIA HORIZONTAL CONSISTE NA SOMA DAS MATRIZES DE CO-OCORRÊNCIA EM RELAÇÃO AO LESTE
// E A MATRIZ DE CO-OCORRÊNCIA EM RELAÇÃO AO OESTE
void Haralick01::simetriaH()
{
    if(this->sH==NULL)
        this->sH = this->alloca_matriz(this->altura, this->largura);

    for(int i=0; i<altura; i++)
        for(int j=0; j<largura; j++)
            sH[i][j] = 0;

    this->coOcorL();
    this->coOcorO();

    for(int i=0; i<altura; i++)
    {
        for(int j=0; j<largura; j++)
        {
            this->sH[i][j] = this->cL[i][j] + this->cO[i][j];
        }
    }
}

// A MATRIZ DE SIMETRIA VERTICAL CONSISTE NA SOMA DAS MATRIZES DE CO-OCORRÊNCIA EM RELAÇÃO AO NORTE
// E A MATRIZ DE CO-OCORRÊNCIA EM RELAÇÃO AO SUL
void Haralick01::simetriaV()
{
    if(this->sV==NULL)
        this->sV = this->alloca_matriz(this->altura, this->largura);

    for(int i=0; i<altura; i++)
        for(int j=0; j<largura; j++)
            sV[i][j] = 0;

    this->coOcorN();
    this->coOcorS();

    for(int i=0; i<altura; i++)
    {
        for(int j=0; j<largura; j++)
        {
            this->sV[i][j] = this->cN[i][j] + this->cS[i][j];
        }
    }
}

// A MATRIZ NORMALIZADA HORIZONTALMENTE CONSISTE NO VALOR DA MATRIZ SIMÉTRICA HORIZONTAL DIVIDIDA PELA QUANTIDADE
// DE CADA COR DE PIXELS (0-255)
void Haralick01::normalizaH()
    {
    if(this->normaH==NULL)
        this->normaH = this->alloca_matriz_f(this->altura, this->largura);

    for(int i=0; i<altura; i++)
        for(int j=0; j<largura; j++)
            normaH[i][j] = 0;

    this->simetriaH();

    float total=0.0;
    for(int i=0; i<altura; i++)
    {
        for(int j=0; j<largura; j++)
        {
            total+= this->sH[i][j];
        }
    }
    for(int i=0; i<altura; i++)
    {
        for(int j=0; j<largura; j++)
        {
            normaH[i][j]=sH[i][j]/total;
        }
    }
}

// A MATRIZ NORMALIZADA VERTICALMENTE CONSISTE NO VALOR DA MATRIZ SIMÉTRICA VERTICAL DIVIDIDA PELA QUANTIDADE
// DE CADA COR DE PIXELS (0-255)
void Haralick01::normalizaV()
{
    if(this->normaV==NULL)
        this->normaV = this->alloca_matriz_f(this->altura, this->largura);

    for(int i=0; i<altura; i++)
        for(int j=0; j<largura; j++)
            normaV[i][j] = 0;

    this->simetriaV();

    float total=0.0;
    for(int i=0; i<altura; i++)
    {
        for(int j=0; j<largura; j++)
        {
            total+= this->sV[i][j];
        }
    }
    for(int i=0; i<altura; i++)
    {
        for(int j=0; j<largura; j++)
        {
            normaV[i][j]=sV[i][j]/total;
        }
    }
}

float Haralick01::miH()
{
    float soma1=0, soma2=0;
    this->normalizaH();
    for(int i=0; i<altura; i++)
    {
        for(int j=0; j<largura; j++)
        {
            soma1+=normaH[i][j];
        }
        soma2+=soma1*i;
        soma1=0;
    }
    return soma2;
}

float Haralick01::miV()
{
    float soma1=0, soma2=0;
    this->normalizaV();
    for(int i=0; i<altura; i++)
    {
        for(int j=0; j<largura; j++)
        {
            soma1+=normaV[i][j];
        }
        soma2+=soma1*i;
        soma1=0;
    }
    return soma2;
}

float Haralick01::sigmaH()
{
    float soma=0, quad=0;
    for(int i=0; i<altura; i++)
    {
        quad = (i-this->miH())*(i-this->miH());
        for(int j=0; j<largura; j++)
        {
            soma+=normaH[i][j]*quad;
        }
    }
    return soma;
}

float Haralick01::sigmaV()
{
    int altura=4, largura=4;
    float soma=0, quad=0;
    for(int i=0; i<altura; i++)
    {
        quad = (i-this->miV())*(i-this->miV());
        for(int j=0; j<largura; j++)
        {
            soma+=normaV[i][j]*quad;
        }
    }
    return soma;
}

// p_{x}

float Haralick01::px(int i){
    float px = 0;

    for (int j = 0; j < altura; j++){
        px += normaH[i][j];
    }

    return px;
}

float Haralick01::py(int j){
    float py = 0;

    for (int i = 0; i < altura; i++){
        py += normaH[i][j];
    }

    return py;
}

// p_{x+y}(k) ## Observar artigo Textural Features for Image Classification - Haralick, pág. 618

float* Haralick01::p_mais(){
    float *p;

    p = (float *) calloc(2 * altura, sizeof(float)); // Alocando e zerando cada posição alocada

    for (int i = 1; i <= altura; i++){
        for (int j = 1; j <= altura; j++){
            *(p + i + j - 2) += normaH[i - 1][j - 1];
        }
    }

    return p;
}

// p_{x-y}(k) ## Observar artigo Textural Features for Image Classification - Haralick, pág. 618

float* Haralick01::p_menos(){
    float *p;
    int k;

    p = (float *) calloc(altura, sizeof(float)); // Alocando e zerando cada posição alocada

    for (int i = 1; i <= altura; i++){
        for (int j = 1; j <= altura; j++){
            k = abs(i - j);
            *(p + k) += normaH[i - 1][j - 1];
        }
    }

    return p;
}

// ################################# DESCRITORES DE HARALICK #########################################


// SEGUNDO MOMENTO ANGULAR - F1

double Haralick01::ASM(){
    double SMA = 0;

    for (int i = 0; i < altura; i++){
        for (int j = 0; j < largura; j++){
            SMA += normaH[i][j] * normaH[i][j];
        }
    }

    return SMA;
}

// ENERGIA

double Haralick01::energy(){
    double energia = this->ASM();

    energia = sqrt(energia);

    return energia;
}

// CONTRASTE - F2

float Haralick01::contrastH()
{
    float soma = 0;

    for (int i = 0; i<altura; i++)
    {
        for (int j = 0; j<largura; j++)
        {
            soma += (float) (normaH[i][j] * ((i - j) * (i - j)));
        }
    }
    return soma;
}

// CORRELAÇÃO HORIZONTAL - F3

float Haralick01::correlacH()
{
    float soma=0, num, den, sigh, mih;

    sigh = this->sigmaH();
    mih = this->miH();
    den = sqrt(sigh*sigh);

    for(int i=0; i<altura; i++)
    {
        num = (i-mih);
        for(int j=0; j<largura; j++)
        {
            num = (i-mih)*(j-mih);
            soma+=normaH[i][j]*(num/den);
        }
    }
    return soma;
}

// VARIÂNCIA - F4

double Haralick01::varience(){
    double F4 = 0;

    float mi = this->miH();

    for (int i = 0; i < altura; i++){
        for (int j = 0; j < largura; j++){
            F4 += ((i - mi) * (i - mi)) * normaH[i][j];
        }
    }

    return F4;
}

// HOMOGENEIDADE OU MOMENTO DA DIFERENÇA INVERSA - F5

float Haralick01::homogeneityH(){
    float soma = 0;

    for (int i = 0; i < altura; i++){
        for (int j = 0; j < largura; j++){
            soma += normaH[i][j] / (1 + ((i - j) * (i - j)));
        }
    }

    return soma;
}

// SOMA DAS MÉDIAS - F6

double Haralick01::soma_media(){
    double media = 0;
    float *p = this->p_mais();

    for (int i = 2; i <= 2 * altura; i++){
        media += i * *(p + i - 2);
    }

    return media;
}

// SOMA DAS VARIÂNCIAS - F7

double Haralick01::sum_varience(){
    double sum = 0;
    float *p = this->p_mais();
    double F8 = this->sum_entropy();

    for (int i = 2; i <= 2 * altura; i++){
        sum += ((i - F8) * (i - F8)) * *(p + i - 2);
    }

    return sum;
}

// SOMA DAS ENTROPIAS - F8

double Haralick01::sum_entropy(){
    double sum = 0;

    float *p = this->p_mais();

    for (int i = 2; i <= 2 * altura; i++){
        if (*(p + i - 2) != 0) sum += *(p + i - 2) * log10(*(p + i - 2));
        else sum += 0;
    }

    sum = -sum;

    return sum;
}

// ENTROPIA - F9

float Haralick01::entropy()
{
    float soma1 = 0;

    for (int i = 0; i < altura; i++)
    {
        for (int j = 2; j < largura; j++)
        {
            if (normaH[i][j] == 0) soma1 += 0;
            else soma1 += (normaH[i][j] * (log10(normaH[i][j])));
        }
    }

    soma1 = - soma1;

    return soma1;
}

// VARIÂNCIA DA DIFERENÇA - F10

double Haralick01::difference_varience(){
    double F10 = 0;
    float *p = this->p_menos();
    float mi_menos = 0;

    for (int i = 0; i < altura; i++){
        mi_menos = i * *(p + i);
    }

    for (int i = 0; i < altura; i++){
        F10 += ((i - mi_menos) * (i - mi_menos)) * *(p + i);
    }

    return F10;
}

// ENTROPIA DA DIFERENÇA - F11

double Haralick01::difference_entropy(){
    double F11 = 0;
    float *p = this->p_menos();

    for (int i = 0; i < altura; i++){
        if (p[i] != 0) F11 += p[i] * log10(p[i]);
        else F11 += 0;
    }

    F11 = -F11;

    return F11;
}

// INFORMAÇÕES DE MEDIDAS DE CORRELAÇÃO 1

double Haralick01::F12(){
    double F12 = 0, HXY1 = 0, HX = 0, HY = 0;
    float px, py;

    double HXY = this->entropy();

    for (int i = 0; i < altura; i++){
        px = this->px(i);
        for (int j = 0; j < largura; j++){
            py = this->py(j);
            if ((px == 0) || (py == 0)) HXY1 += 0;
            else HXY1 -= normaH[i][j] * log(px * py);
        }
    }

    px = 0;
    py = 0;

    for (int i = 0; i < altura; i++){
        for (int j = 0; j < largura; j++){
            px += normaH[i][j];
            py += normaH[j][i];
        }

        if (px == 0) HX += 0;
        else HX -= px * log10(px);

        if (py == 0) HY += 0;
        else HY -= py * log10(py);
    }

    if (HX > HY){
        F12 = (HXY - HXY1) / (HX);
    } else {
        F12 = (HXY - HXY1) / (HY);
    }

    return F12;
}

// INFORMAÇÕES DE MEDIDAS DE CORRELAÇÃO 2

double Haralick01::F13(){
    double F13 = 0, HXY2 = 0;
    float px, py;

    double HXY = this->entropy();

    for (int i = 0; i < altura; i++){
        px = this->px(i);
        for (int j = 0; j < largura; j++){
            py = this->py(j);
            if ((px == 0) || (py == 0)) HXY2 += 0;
            else HXY2 -= px * py * log(px * py);
        }
    }

    F13 = sqrt(1 - exp(-2 * (HXY2 - HXY)));

    return F13;
}

// CORRELAÇÃO VERTICAL

float Haralick01::correlacV()
{
    float soma=0, num, den, miv, sigv;

    miv = this->miV();
    sigv = this->sigmaV();
    den = sqrt(sigv*sigv);

    for(int i=0; i<altura; i++)
    {
        num = (i-miv);
        for(int j=0; j<largura; j++)
        {
            num = (i-miv)*(j-miv);
            soma+=normaV[i][j]*(num/den);
        }
    }
    return soma;
}

// DISSIMILARIDADE

double Haralick01::dissimilarity(){
    double diss = 0;

    for (int i = 0; i < altura; i++){
        for (int j = 0; j < largura; j++){
            diss += (abs(i - j) * normaH[i][j]);
        }
    }

    return diss;
}

/*
// MÉDIA

double Haralick01::media(){
double media = 0, soma = 0;

for(int i = 0; i < altura; i++){
    for (int j = 0; j < largura; j++){
        soma += normaH[i][j];
    }

    media += i * soma;
    soma = 0;
}

return media;

}

// MOMENTO DO PRODUTO

double Haralick01::momento_produto(){
double mom = 0, media, num;

media = this->media();

//cout << media << endl;

for (int i = 0; i < altura; i++){
    num = i - media;
    for (int j = 0; j < largura; j++){
        mom +=  num * (j - media) * (normaH[i][j]);
    }
}


return mom;
}

// VARIÂNCIA

double Haralick01::varience(){
double var = 0, line = 0, media;

media = this->media();

for (int i = 0; i < largura; i++){
    line = 0;

    for (int j = 0; j < altura; j++){
        line += normaH[i][j];
    }

    var += (i - media) * (i - media) * line;
}

return var;
}

// MÉDIA DA SOMA

double Haralick01::media_soma(){
double F6 = 0, soma = 0;

for (int k = 0; k < 2 * largura; k++){
    soma = 0;
    for (int i = 0; i < altura; i++){
        for (int j = 0; j < altura; j++){
            soma += normaH[i][j];
        }
    }

    F6 += soma * k;
}

return F6;
}
*/


Haralick01::~Haralick01()
{
if (this->ent != NULL)
    this->ent = this->libera_matriz(this->imgL, this->imgC, this->ent);

if (this->cN != NULL)
    this->cN = this->libera_matriz(this->altura, this->largura, this->cN);

if (this->cS != NULL)
    this->cS = this->libera_matriz(this->altura, this->largura, this->cS);

if (this->cL != NULL)
    this->cL = this->libera_matriz(this->altura, this->largura, this->cL);

if (this->cO != NULL)
    this->cO = this->libera_matriz(this->altura, this->largura, this->cO);

if (this->sH != NULL)
    this->sH = this->libera_matriz(this->altura, this->largura, this->sH);

if (this->sV != NULL)
    this->sV = this->libera_matriz(this->altura, this->largura, this->sV);

if (this->normaH != NULL)
    this->normaH = this->libera_matriz_f(this->altura, this->largura, this->normaH);

if (this->normaV != NULL)
    this->normaV = this->libera_matriz_f(this->altura, this->largura, this->normaV);
}
