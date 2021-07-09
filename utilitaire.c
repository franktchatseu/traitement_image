#include <stdlib.h>
#include "fonction.h"
#include <stdio.h>

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
