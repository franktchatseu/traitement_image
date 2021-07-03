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
struct PointHisto
{
      int niveau_gris;
      int valeur;
};
typedef struct PointHisto PointHisto;

struct Histogramme
{
      int valeur_max;
      PointHisto* dataPoint;
};
typedef struct PointHisto PointHisto;
typedef struct Histogramme Histogramme;
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
      PointHisto* points = malloc(sizeof(PointHisto)*(image.maxVal+1));
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
            printf("%d : %d \t",histo.dataPoint[i].niveau_gris,histo.dataPoint[i].valeur);
      }
      
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


// ecrire de la function qui charge et affiche une image
void loadImage(FILE f)
{
}
int main(int argc, char *argv[])
{
      int i, j = 0, row, col;
      Image *image = malloc(sizeof(Image));
      Image *output = malloc(sizeof(Image));
      Image *image2 = malloc(sizeof(Image));
      getPGMfile("lena.pgm", &(*image));
      //increaseLuminance(image, 50);
      save(image, "resultuuu");
      //test addition
      Image imgadd=additionerImage(*image, *image);
      Image sous =soustrationImage(imgadd,*image);
      //multiplicationParRatio(image,1.2);
      save(image, argv[1]);
      //printf("le nombre de %d est %d",0,nbOccurence(*image,0));
      printf("%d",image->maxVal);
      Histogramme histo = constructHistogramme(*image);
      afficheHistogramme(histo);
      return 0;
}