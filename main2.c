#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 800

struct ImageStructure
{
      int maxVal;
      int width;
      int height;
      int **data;
};
typedef struct ImageStructure Image;
//Structure histogramme
struct Point
{
      int niveau_gris;
      float valeur;
};
typedef struct Point Point;

struct Histogramme
{
      int valeur_max;
      Point* dataPoint;
};
typedef struct Point Point;
typedef struct Histogramme Histogramme;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 800

struct ImageStructure
{
      int maxVal;
      int width;
      int height;
      int **data;
};
typedef struct ImageStructure Image;
//Structure histogramme
struct Point
{
      int niveau_gris;
      float valeur;
};
typedef struct Point Point;

struct Histogramme
{
      int valeur_max;
      Point* dataPoint;
};
typedef struct Point Point;
typedef struct Histogramme Histogramme;

//fonction qui initialise une image
void initImage(Image *image,int width,int height,int max_val)
{
      image->height = width;
      image->width = height;
      image->maxVal = max_val;
      int **data = (int **)malloc(width * sizeof(int *));
      for (int i = 0; i < width; i++)
            data[i] = (int *)malloc(height * sizeof(int));
      image->data = data;
}
//verifier les dimensions des images
int verif_dimension(Image image1, Image image2)
{
      if (image1.height != image2.height || image1.width != image2.width)
      {
            printf("les deux n'ont pas la même dimension\n!");
            return 0;
      }
      return 1;
}

void getPGMfile(char filename[], Image *img)
{
      FILE *in_file;
      char ch;
      int row, col, type;
      int ch_int;

      in_file = fopen(filename, "r");
      if (in_file == NULL)
      {
            fprintf(stderr, "Error: Unable to open file %s\n\n", filename);
            exit(8);
      }

      printf("\nReading image file: %s\n", filename);

      // deterninons le type de format pgm de l'image
      ch = getc(in_file);
      if (ch != 'P')
      {
            printf("ERROR(1): Not valid pgm/ppm file type\n");
            exit(1);
      }
      ch = getc(in_file);
      /*convert the one digit integer currently represented as a character to
    an integer(48 == '0')*/

      while (getc(in_file) != '\n')
            ; /* skip to end of line*/

      while (getc(in_file) == '#') /* skip comment lines */
      {
            while (getc(in_file) != '\n')
                  ; /* skip to end of comment line */
      }

      fseek(in_file, -1, SEEK_CUR); /* backup one character*/

      fscanf(in_file, "%d", &((*img).width));
      fscanf(in_file, "%d", &((*img).height));
      fscanf(in_file, "%d", &((*img).maxVal));

      printf("\n width  = %d", (*img).width);
      printf("\n height = %d", (*img).height);
      printf("\n maxVal = %d", (*img).maxVal);
      printf("\n");

      //allocation dynamique
      int **data_pixel = (int **)malloc(img->width * sizeof(int *));
      for (int i = 0; i < img->width; i++)
            data_pixel[i] = (int *)malloc(img->height * sizeof(int));
      img->data = data_pixel;
      //
      if (((*img).width > MAX) || ((*img).height > MAX))
      {
            printf("\n\n***ERROR - image too big for current image structure***\n\n");
            exit(1);
      }

      for (row = 0; row < (*img).height; row++)
      {
            for (col = 0; col < (*img).width; col++)
            {
                  fscanf(in_file, "%d", &ch_int);
                  // printf("%d", ch_int);
                  if (ch_int < 0)
                        printf("%d %d %d", row, col, ch_int);
                  (*img).data[row][col] = ch_int;
            }
      }

      fclose(in_file);
      printf("\nDone reading file.\n");
}

//sauvegarde de la matrice d'image dans un fichier
void save(Image *img, char output_file_name[])
{
      
      FILE *iop;
      int row, col;
      int nr = (*img).height;
      int nc = (*img).width;
      iop = fopen(output_file_name, "w");
      fprintf(iop, "P2\n");
      fprintf(iop, "#created by a.exe\n");
      fprintf(iop, "%d %d\n", nc, nr);
      fprintf(iop, "255\n");

      for (row = 0; row < nr; row++)
      {
            for (col = 0; col < nc; col++)
            {
                  fprintf(iop, "%d ", (*img).data[row][col]);
            }
      }
      fprintf(iop, "\n");
      fclose(iop);
}
//add brillance
void increaseLuminance(Image *img, int quantity)
{

      for (int i = 0; i < img->width; i++)
            for (int j = 0; j < img->height; j++)
            {
                  if (img->data[i][j] + quantity > 255)
                        img->data[i][j] = 255;
                  else if (img->data[i][j] + quantity < 0)
                        img->data[i][j] = 0;
                  else
                        img->data[i][j] += quantity;
            }
}
// multiplication de l'image
void multiplicationParRatio(Image *img, float ratio)
{

      for (int i = 0; i < img->width; i++)
            for (int j = 0; j < img->height; j++)
            {
                  if (img->data[i][j] * ratio > 255)
                        img->data[i][j] = 255;
                  else
                        img->data[i][j] *= ratio;
            }
}
// addition de deux images
Image additionerImage(Image image1, Image image2)
{     Image *result = malloc(sizeof(Image));
      verif_dimension(image1, image2);
      //creation de image en sortie
      initImage(result,image1.width,image1.height,image1.maxVal);
      //addition proprement dite
      for (int i = 0; i < image1.width; i++)
            for (int j = 0; j < image1.height; j++)
                  if (image1.data[i][j] + image2.data[i][j] <= 255)
                        result->data[i][j] =image1.data[i][j] + image2.data[i][j];
                  else
                        result->data[i][j] = 255;
      return *result;
}
// soustration de deux images
Image soustrationImage(Image image1, Image image2)
{
      Image *result = malloc(sizeof(Image));
      verif_dimension(image1, image2);
      //creation de image en sortie
      initImage(result,image1.width,image1.height,image1.maxVal);
      //addition proprement dite
      for (int i = 0; i < image1.width; i++)
            for (int j = 0; j < image1.height; j++)
                  if (image1.data[i][j] - image2.data[i][j] >= 0)
                        result->data[i][j] =image1.data[i][j] - image2.data[i][j];
                  else
                        result->data[i][j] = 0;
      return *result;
}

//construction de histogramme
// nombre d'occurence d'un niveau de gris donnée
int nbOccurence(Image image, int niveau_gris){

      int i,j;
      int nbocc=0;
      for (i = 0; i < image.width; i++)
            for ( j = 0; j < image.height; j++)
                  if(image.data[i][j]==niveau_gris)
                        nbocc++;
      return nbocc;
}
//  histogramme proprement dite
Histogramme constructHistogramme(Image image){
      int i;
      Histogramme* histo = malloc(sizeof(Histogramme));
      Point* points = malloc(sizeof(Point)*(image.maxVal+1));
      histo->valeur_max = image.maxVal +1;

      for(i=0; i<histo->valeur_max; i++){
            points[i].niveau_gris =i;
            points[i].valeur = nbOccurence(image,i);
      }
      histo->dataPoint = points;

      return *histo;
}
// afficher histogramme
void afficheHistogramme(Histogramme histo){
      int i;
      for ( i = 0; i < histo.valeur_max; i++)
      {
            printf("%d : %f \t",histo.dataPoint[i].niveau_gris,histo.dataPoint[i].valeur);
      }
      
}

// transformation lineaire
//1. det min pixel
int getMinPixel(Image image){

    int min = image.data[0][0];
    for(int i=0; i<image.height;i++){
        for(int j=0; j<image.width; j++){
            if (image.data[i][j]<min){
                min = image.data[i][j];
            }
        }
    }
    printf("le eest %d",min);
    return min;
}
//2. det max pixel
int getMaxPixel(Image image){

    int max = image.data[0][0];
    for(int i=0; i<image.height;i++){
        for(int j=0; j<image.width; j++){
            if (image.data[i][j]>max){
                max = image.data[i][j];
            }
        }
    }
    printf("le eest %d",max);
    return max;
}
//transformation lineaire
Image transformationLineaire(Image image)
{    
       int maxPixel = getMaxPixel(image);
       int minPixel = getMinPixel(image);
       int denom = maxPixel - minPixel;
       printf("le pixel max est %d et le pixel min est %d",maxPixel,minPixel);
       Image *result = malloc(sizeof(Image));
      //creation de image en sortie
      initImage(result,image.width,image.height,image.maxVal);
      for (int i = 0; i < image.width; i++)
            for (int j = 0; j < image.height; j++)
                  result->data[i][j] = 255*(image.data[i][j]-minPixel)/denom;

      return *result;
}

// egalisation de Histogramme
Image egalisationHistogramme(Image image){
      // recuperation du nombre de pixel total
      int total = image.width * image.height;
      // calcul de histogramme
      Histogramme histo = constructHistogramme(image);
      // Normalisation de l'histogramme
      for (int i = 0; i < histo.valeur_max; i++)
      {
        histo.dataPoint[i].valeur = (float)((histo.dataPoint[i]).valeur)/total;
      }
      afficheHistogramme(histo);

      // Densite de probabilite normalisee
      float densite[histo.valeur_max];
      printf("calcul");
      densite[0] = histo.dataPoint[0].valeur;
      for (int k = 1; k < histo.valeur_max; k++)
      {
            densite[k] = densite[k-1] + histo.dataPoint[k].valeur;
      }
    // modification niveaux de gris de l'image
    for(int i=0; i< image.height; i++){
        for(int j=0; j< image.width; j++){
            image.data[i][j] = densite[image.data[i][j]] * 255;
        }
    }
    return image;
}

// partie 2: convolution
int convolutionPixel(Image image,int pix_i,int pix_j){
      int result = 0;
      int lign_fil=2; int col_fil=2;
      int mat_pix_i= pix_i-1;
      int mat_pix_j = pix_j-1;
      //int filtre[lign_fil][col_fil];
      int filtre[3][3] = {
        { 1, 1, 1 },
        { 1,  1, 1 },
        { 1, 1, 1 }
    };
    int i=0; int j=0;
        //result = (image.data[i-1][j-1]*filtre[0][0] + image.data[i-1][j]*filtre[0][1]+ image.data[i-1][j+1]*filtre[0][2]+image.data[i+1][j]*filtre[2][1]+image.data[i][j]*filtre[1][1]+image.data[i][j-1]*filtre[1][0]+image.data[i][j+1]*filtre[1][2]+image.data[i+1][j+1]*filtre[2][2]+image.data[i][j-1]*filtre[0][2]);

        //result = (image.data[i+1][j+1]*filtre[0][0] + image.data[i][j+1]*filtre[0][1]+ image.data[i-1][j+1]*filtre[0][2]+image.data[i+1][j]*filtre[1][0]+image.data[i][j]*filtre[1][1]+image.data[i-1][j]*filtre[1][2]+image.data[i+1][j-1]*filtre[2][0]+image.data[i][j+1]*filtre[2][1]+image.data[i-1][j-1]*filtre[2][2]);

      for (int i =0; i<lign_fil; i++){
            for (int j = 0; j < col_fil; j++)
            {
                 result += image.data[mat_pix_i+i][mat_pix_j+j] * filtre[i][j];
            }
            
      }
      return result;
}
Image Imageconvolution(Image imageEntre,int masqX,int masqY,int masque[masqY][masqX],int division)
{
    int ImaX = imageEntre.width;
    int ImaY = imageEntre.height;
    printf("wiX %d ",ImaX);
    // Avoir le centre de notre masque
    int MasqueX = (masqX/2);
    int MasqueY = (masqY/2);
     Image *imageFinale = malloc(sizeof(Image));
      //creation de image en sortie
      initImage(imageFinale,imageEntre.width,imageEntre.height,imageEntre.maxVal);
    // On itère sur tout les pixels de l'image
    // Pour chaque ligne
    // On prend chaque pixels
    for (int x = 0;x<ImaX;x++){
        // Ensuite on change de colonne
        for(int y = 0; y<ImaY;y++){
            // Valeur calculer pour chaque pixel de l'image
            double accumulator = 0;

            // Pour chaque colonne de notre mask
            /*
               -1 0 1
               -1 |0|0|0|
               0  |0|0|0|
               1  |0|0|0|
               Voici comment la boucle du masque fonctionne
*/
            for(int i = -MasqueX;i <= MasqueX; i++){
                // On prend chaque Pixel
                for(int j = -MasqueY;j <= MasqueY; j++){
                      //printf("sfs");
                    int indeX = x+i;
                    int indeY = y+j;
                    // Si l'index n'est pas compris dans l'image
                    if ( indeX < 0 || indeY < 0 || indeX >= ImaX-1 || indeY >= ImaY-1){
                        // On lui donne la valeur Zero
                        accumulator +=0;
                        
                    }
                    
                    else{
                          //printf("%d ",indeX);
                        //printf("%d ",indeY);
                      accumulator += masque[i+MasqueX][j+MasqueY]*(imageEntre.data[indeX][indeY]);
                    }
                }
            }
            imageFinale->data[x][y] = (int)abs(accumulator/division);
        }
    }
    return *imageFinale;
}
//filtre moyenneur
Image filtreMoyenneur(Image image, int taille){
      // creation d'un masque moyenneur avec une division par taille*taille
      int moyenneur [taille][taille];
      for (int i = 0; i < taille; i++)
      {
           for (int j = 0; j < taille; j++)
           {
             moyenneur[i][j] = 1;     
             printf("%d ",moyenneur[i][j]);            /* code */
           } 
           printf("\n");
      }
      Image conv = Imageconvolution(image,taille,taille,moyenneur,taille*taille);
      return conv;
}


// filtre gaussien
Image filtreGaussien(Image image,int flou){
      // creation d'un masque gaussien avec une division par taille*taille
      if(flou){
      int gaussien[5][5] = {{1,2,3,2,1},{2,4,6,4,2},{3,6,9,4,3},{2,4,6,4,2},{1,2,3,2,1}};
      }
      else{
           int gaussien[3][3] = {{1,2,1},{2,4,2},{1,2,1}};
      }
      // gaussien flou
      int gaussien2[5][5] = {{1,2,3,2,1},{2,4,6,4,2},{3,6,9,4,3},{2,4,6,4,2},{1,2,3,2,1}};
      Image conv = Imageconvolution(image,5,5,gaussien2,79);
      return conv;
}
// filtre median: il s'agit ici d'un filtre non lineaire
int  median( int n,int arr[n]){
    // cette partie permet de trier la liste
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}


//CONTOURS
// filtre de prewitt
Image filtrePrewitt(Image image, char axe){
      
      int prewitt[3][3] = {
        { -1, -1, -1 },
        { 0, 0, 0 },
        { 1, 1, 1 }
    };
   return Imageconvolution(image,3,3,prewitt,1);
}
// filtre de robert
Image filtreRobert(Image image, char axe){
      if(axe =='x'){
        int robert_x[3][3] = {
        { 0, 1},
        { -1, 0},
       };
            return Imageconvolution(image,2,2,robert_x,1);
      }
      else{
         int roberty[2][2]={{1,0},{0,-1}};
         return Imageconvolution(image,2,2,roberty,1);
      }
}
//filtre de sobel
Image filtreSobel(Image image,char axe){
    int sobelx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    int sobely[3][3];
    for(int i = 0; i<3;i++){
        for(int j = 0; j<3;j++){
            sobely[i][j]= sobelx[j][i];
        }
    }
    if(axe =='x'){
        return Imageconvolution(image,2,2,sobelx,1);
      }
      else{
         int roberty[2][2]={{1,0},{0,-1}};
         return Imageconvolution(image,2,2,sobely,1);
      }
}
// le laplacien
Image filtreLaplacien(Image image){

    int laplacien[3][3] = {{0,1,0},{1,-4,1},{0,1,0}};
    int laplacien2[3][3]={{1,1,1},{1,-8,1},{1,1,1}};
    return Imageconvolution(image,3,3,laplacien2,1);

}

int main(int argc, char *argv[])
{
      int i, j = 0, row, col;
      Image *image = malloc(sizeof(Image));
      Image *output = malloc(sizeof(Image));
      Image *image2 = malloc(sizeof(Image));
      getPGMfile("lena.pgm", &(*image));
      int boxBlur[3][3] = {
        { 1, 1, 1 },
        { 1,  1, 1 },
        { 1, 1, 1 }
    };
      //Image conv = filtreMoyenneur(*image,11);
        //Image conv = filtreGaussien(*image,1);
      //Image conv = Imageconvolution(*image,3,3,boxBlur,9);

      //CONTOURS
      //Image conv = filtreSobel(*image,'x');
      Image conv = filtrePrewitt(*image,'x');
      //Image conv = filtreLaplacien(*image);
      //Image conv = filtreRobert(*image,'x');


      //increaseLuminance(image, 50);
      //save(image, "resultuuu");
      //test addition
      //Image imgadd=additionerImage(*image, *image);
      //Image sous =soustrationImage(imgadd,*image);
      //multiplicationParRatio(image,1.2);
      //Image trans = transformationLineaire(*image);
      //Image egal = egalisationHistogramme(*image);
      save(&conv, argv[1]);
      //printf("le nombre de %d est %d",0,nbOccurence(*image,0));
      printf("la convolution du pixel 1,0 est %d",convolutionPixel(*image,1,1));
      printf("%d",image->maxVal);
      //Histogramme histo = constructHistogramme(*image);
      //afficheHistogramme(histo);
      return 0;
}
// allocation dynamique d'un tableau de pixel
/*int** allocPixel(int width,int height){
      int **data_pixel  = (int **)malloc(width * sizeof(int *));
      for (int i=0; i<width; i++)
         data_pixel[i] = (int *)malloc(height * sizeof(int));
      return &data_pixel;
}*/

//add brillance
void increaseLuminance(Image *img, int quantity)
{

      for (int i = 0; i < img->width; i++)
            for (int j = 0; j < img->height; j++)
            {
                  if (img->data[i][j] + quantity > 255)
                        img->data[i][j] = 255;
                  else if (img->data[i][j] + quantity < 0)
                        img->data[i][j] = 0;
                  else
                        img->data[i][j] += quantity;
            }
}
// multiplication de l'image
void multiplicationParRatio(Image *img, float ratio)
{

      for (int i = 0; i < img->width; i++)
            for (int j = 0; j < img->height; j++)
            {
                  if (img->data[i][j] * ratio > 255)
                        img->data[i][j] = 255;
                  else
                        img->data[i][j] *= ratio;
            }
}
// addition de deux images
Image additionerImage(Image image1, Image image2)
{     Image *result = malloc(sizeof(Image));
      verif_dimension(image1, image2);
      //creation de image en sortie
      initImage(result,image1.width,image1.height,image1.maxVal);
      //addition proprement dite
      for (int i = 0; i < image1.width; i++)
            for (int j = 0; j < image1.height; j++)
                  if (image1.data[i][j] + image2.data[i][j] <= 255)
                        result->data[i][j] =image1.data[i][j] + image2.data[i][j];
                  else
                        result->data[i][j] = 255;
      return *result;
}
// soustration de deux images
Image soustrationImage(Image image1, Image image2)
{
      Image *result = malloc(sizeof(Image));
      verif_dimension(image1, image2);
      //creation de image en sortie
      initImage(result,image1.width,image1.height,image1.maxVal);
      //addition proprement dite
      for (int i = 0; i < image1.width; i++)
            for (int j = 0; j < image1.height; j++)
                  if (image1.data[i][j] - image2.data[i][j] >= 0)
                        result->data[i][j] =image1.data[i][j] - image2.data[i][j];
                  else
                        result->data[i][j] = 0;
      return *result;
}

//construction de histogramme
// nombre d'occurence d'un niveau de gris donnée
int nbOccurence(Image image, int niveau_gris){

      int i,j;
      int nbocc=0;
      for (i = 0; i < image.width; i++)
            for ( j = 0; j < image.height; j++)
                  if(image.data[i][j]==niveau_gris)
                        nbocc++;
      return nbocc;
}
//  histogramme proprement dite
Histogramme constructHistogramme(Image image){
      int i;
      Histogramme* histo = malloc(sizeof(Histogramme));
      Point* points = malloc(sizeof(Point)*(image.maxVal+1));
      histo->valeur_max = image.maxVal +1;

      for(i=0; i<histo->valeur_max; i++){
            points[i].niveau_gris =i;
            points[i].valeur = nbOccurence(image,i);
      }
      histo->dataPoint = points;

      return *histo;
}
// afficher histogramme
void afficheHistogramme(Histogramme histo){
      int i;
      for ( i = 0; i < histo.valeur_max; i++)
      {
            printf("%d : %f \t",histo.dataPoint[i].niveau_gris,histo.dataPoint[i].valeur);
      }
      
}

// transformation lineaire
//1. det min pixel
int getMinPixel(Image image){

    int min = image.data[0][0];
    for(int i=0; i<image.height;i++){
        for(int j=0; j<image.width; j++){
            if (image.data[i][j]<min){
                min = image.data[i][j];
            }
        }
    }
    printf("le eest %d",min);
    return min;
}
//2. det max pixel
int getMaxPixel(Image image){

    int max = image.data[0][0];
    for(int i=0; i<image.height;i++){
        for(int j=0; j<image.width; j++){
            if (image.data[i][j]>max){
                max = image.data[i][j];
            }
        }
    }
    printf("le eest %d",max);
    return max;
}
//transformation lineaire
Image transformationLineaire(Image image)
{    
       int maxPixel = getMaxPixel(image);
       int minPixel = getMinPixel(image);
       int denom = maxPixel - minPixel;
       printf("le pixel max est %d et le pixel min est %d",maxPixel,minPixel);
       Image *result = malloc(sizeof(Image));
      //creation de image en sortie
      initImage(result,image.width,image.height,image.maxVal);
      for (int i = 0; i < image.width; i++)
            for (int j = 0; j < image.height; j++)
                  result->data[i][j] = 255*(image.data[i][j]-minPixel)/denom;

      return *result;
}

//sauvegarde de la matrice d'image dans un fichier
void save(Image *img, char output_file_name[])
{
      
      FILE *iop;
      int row, col;
      int nr = (*img).height;
      int nc = (*img).width;
      iop = fopen(output_file_name, "w");
      fprintf(iop, "P2\n");
      fprintf(iop, "#created by a.exe\n");
      fprintf(iop, "%d %d\n", nc, nr);
      fprintf(iop, "255\n");

      for (row = 0; row < nr; row++)
      {
            for (col = 0; col < nc; col++)
            {
                  fprintf(iop, "%d ", (*img).data[row][col]);
            }
      }
      fprintf(iop, "\n");
      fclose(iop);
}

// egalisation de Histogramme
Image egalisationHistogramme(Image image){
      // recuperation du nombre de pixel total
      int total = image.width * image.height;
      // calcul de histogramme
      Histogramme histo = constructHistogramme(image);
      // Normalisation de l'histogramme
      for (int i = 0; i < histo.valeur_max; i++)
      {
        histo.dataPoint[i].valeur = (float)((histo.dataPoint[i]).valeur)/total;
      }
      afficheHistogramme(histo);

      // Densite de probabilite normalisee
      float densite[histo.valeur_max];
      printf("calcul");
      densite[0] = histo.dataPoint[0].valeur;
      for (int k = 1; k < histo.valeur_max; k++)
      {
            densite[k] = densite[k-1] + histo.dataPoint[k].valeur;
      }
    // modification niveaux de gris de l'image
    for(int i=0; i< image.height; i++){
        for(int j=0; j< image.width; j++){
            image.data[i][j] = densite[image.data[i][j]] * 255;
        }
    }
    return image;
}

// partie 2: convolution
int convolutionPixel(Image image,int pix_i,int pix_j){
      int result = 0;
      int lign_fil=2; int col_fil=2;
      int mat_pix_i= pix_i-1;
      int mat_pix_j = pix_j-1;
      //int filtre[lign_fil][col_fil];
      int filtre[3][3] = {
        { 1, 1, 1 },
        { 1,  1, 1 },
        { 1, 1, 1 }
    };
    int i=0; int j=0;
        //result = (image.data[i-1][j-1]*filtre[0][0] + image.data[i-1][j]*filtre[0][1]+ image.data[i-1][j+1]*filtre[0][2]+image.data[i+1][j]*filtre[2][1]+image.data[i][j]*filtre[1][1]+image.data[i][j-1]*filtre[1][0]+image.data[i][j+1]*filtre[1][2]+image.data[i+1][j+1]*filtre[2][2]+image.data[i][j-1]*filtre[0][2]);

        //result = (image.data[i+1][j+1]*filtre[0][0] + image.data[i][j+1]*filtre[0][1]+ image.data[i-1][j+1]*filtre[0][2]+image.data[i+1][j]*filtre[1][0]+image.data[i][j]*filtre[1][1]+image.data[i-1][j]*filtre[1][2]+image.data[i+1][j-1]*filtre[2][0]+image.data[i][j+1]*filtre[2][1]+image.data[i-1][j-1]*filtre[2][2]);

      for (int i =0; i<lign_fil; i++){
            for (int j = 0; j < col_fil; j++)
            {
                 result += image.data[mat_pix_i+i][mat_pix_j+j] * filtre[i][j];
            }
            
      }
      return result;
}
Image Imageconvolution(Image imageEntre,int masqX,int masqY,int masque[masqY][masqX],int division)
{
    int ImaX = imageEntre.width;
    int ImaY = imageEntre.height;
    printf("wiX %d ",ImaX);
    // Avoir le centre de notre masque
    int MasqueX = (masqX/2);
    int MasqueY = (masqY/2);
     Image *imageFinale = malloc(sizeof(Image));
      //creation de image en sortie
      initImage(imageFinale,imageEntre.width,imageEntre.height,imageEntre.maxVal);
    // On itère sur tout les pixels de l'image
    // Pour chaque ligne
    // On prend chaque pixels
    for (int x = 0;x<ImaX;x++){
        // Ensuite on change de colonne
        for(int y = 0; y<ImaY;y++){
            // Valeur calculer pour chaque pixel de l'image
            double accumulator = 0;

            // Pour chaque colonne de notre mask
            /*
               -1 0 1
               -1 |0|0|0|
               0  |0|0|0|
               1  |0|0|0|
               Voici comment la boucle du masque fonctionne
*/
            for(int i = -MasqueX;i <= MasqueX; i++){
                // On prend chaque Pixel
                for(int j = -MasqueY;j <= MasqueY; j++){
                      //printf("sfs");
                    int indeX = x+i;
                    int indeY = y+j;
                    // Si l'index n'est pas compris dans l'image
                    if ( indeX < 0 || indeY < 0 || indeX >= ImaX-1 || indeY >= ImaY-1){
                        // On lui donne la valeur Zero
                        accumulator +=0;
                        
                    }
                    
                    else{
                          //printf("%d ",indeX);
                        //printf("%d ",indeY);
                      accumulator += masque[i+MasqueX][j+MasqueY]*(imageEntre.data[indeX][indeY]);
                    }
                }
            }
            imageFinale->data[x][y] = (int)abs(accumulator/division);
        }
    }
    return *imageFinale;
}
//filtre moyenneur
Image filtreMoyenneur(Image image, int taille){
      // creation d'un masque moyenneur avec une division par taille*taille
      int moyenneur [taille][taille];
      for (int i = 0; i < taille; i++)
      {
           for (int j = 0; j < taille; j++)
           {
             moyenneur[i][j] = 1;     
             printf("%d ",moyenneur[i][j]);            /* code */
           } 
           printf("\n");
      }
      Image conv = Imageconvolution(image,taille,taille,moyenneur,taille*taille);
      return conv;
}


// filtre gaussien
Image filtreGaussien(Image image,int flou){
      // creation d'un masque gaussien avec une division par taille*taille
      if(flou){
      int gaussien[5][5] = {{1,2,3,2,1},{2,4,6,4,2},{3,6,9,4,3},{2,4,6,4,2},{1,2,3,2,1}};
      }
      else{
           int gaussien[3][3] = {{1,2,1},{2,4,2},{1,2,1}};
      }
      // gaussien flou
      int gaussien2[5][5] = {{1,2,3,2,1},{2,4,6,4,2},{3,6,9,4,3},{2,4,6,4,2},{1,2,3,2,1}};
      Image conv = Imageconvolution(image,5,5,gaussien2,79);
      return conv;
}
// filtre median: il s'agit ici d'un filtre non lineaire
int  median( int n,int arr[n]){
    // cette partie permet de trier la liste
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}


//CONTOURS
// filtre de prewitt
Image filtrePrewitt(Image image, char axe){
      
      int prewitt[3][3] = {
        { -1, -1, -1 },
        { 0, 0, 0 },
        { 1, 1, 1 }
    };
   return Imageconvolution(image,3,3,prewitt,1);
}
// filtre de robert
Image filtreRobert(Image image, char axe){
      if(axe =='x'){
        int robert_x[3][3] = {
        { 0, 1},
        { -1, 0},
       };
            return Imageconvolution(image,2,2,robert_x,1);
      }
      else{
         int roberty[2][2]={{1,0},{0,-1}};
         return Imageconvolution(image,2,2,roberty,1);
      }
}
//filtre de sobel
Image filtreSobel(Image image,char axe){
    int sobelx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    int sobely[3][3];
    for(int i = 0; i<3;i++){
        for(int j = 0; j<3;j++){
            sobely[i][j]= sobelx[j][i];
        }
    }
    if(axe =='x'){
        return Imageconvolution(image,2,2,sobelx,1);
      }
      else{
         int roberty[2][2]={{1,0},{0,-1}};
         return Imageconvolution(image,2,2,sobely,1);
      }
}
// le laplacien
Image filtreLaplacien(Image image){

    int laplacien[3][3] = {{0,1,0},{1,-4,1},{0,1,0}};
    int laplacien2[3][3]={{1,1,1},{1,-8,1},{1,1,1}};
    return Imageconvolution(image,3,3,laplacien2,1);

}

int main(int argc, char *argv[])
{
      int i, j = 0, row, col;
      Image *image = malloc(sizeof(Image));
      Image *output = malloc(sizeof(Image));
      Image *image2 = malloc(sizeof(Image));
      getPGMfile("lena.pgm", &(*image));
      int boxBlur[3][3] = {
        { 1, 1, 1 },
        { 1,  1, 1 },
        { 1, 1, 1 }
    };
      //Image conv = filtreMoyenneur(*image,11);
        //Image conv = filtreGaussien(*image,1);
      //Image conv = Imageconvolution(*image,3,3,boxBlur,9);

      //CONTOURS
      //Image conv = filtreSobel(*image,'x');
      Image conv = filtrePrewitt(*image,'x');
      //Image conv = filtreLaplacien(*image);
      //Image conv = filtreRobert(*image,'x');


      //increaseLuminance(image, 50);
      //save(image, "resultuuu");
      //test addition
      //Image imgadd=additionerImage(*image, *image);
      //Image sous =soustrationImage(imgadd,*image);
      //multiplicationParRatio(image,1.2);
      //Image trans = transformationLineaire(*image);
      //Image egal = egalisationHistogramme(*image);
      save(&conv, argv[1]);
      //printf("le nombre de %d est %d",0,nbOccurence(*image,0));
      printf("la convolution du pixel 1,0 est %d",convolutionPixel(*image,1,1));
      printf("%d",image->maxVal);
      //Histogramme histo = constructHistogramme(*image);
      //afficheHistogramme(histo);
      return 0;
}