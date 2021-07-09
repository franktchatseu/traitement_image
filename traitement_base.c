#include <stdlib.h>
#include "fonction.h"
#include <stdio.h>

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
