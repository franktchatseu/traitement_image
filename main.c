#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonction.h"
#include <curses.h>
#define MAX 800

Image recuperer_image(char image_name[100]){
      Image *image = malloc(sizeof(Image));
      getPGMfile(image_name, &(*image));
      return *image;
}


void menu2(char image_entree[100],char image_output[100]){
        char choice = '0';
        char choice2 = '0';
        Image entree=recuperer_image(image_entree);
        Image sortie;

            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n \n\n\n\n\n\n\n\n\n\tMENU TP VISION PAR ORDINATEUR");
            printf("\n\t---------------------------------------------");
            printf("\n\n\t 1. TRAITEMENT DE BASE");
            printf("\n\t 2. CONVOLUTION NUMERIQUE ");
            printf("\n\t 3. CONTOURS ");
            printf("\n\t 4. EXIT");
            printf("\n\n Enter Your CHoice: ");
            scanf("%1[1234]c%*c", &choice);
            switch (choice)
            {
            case '1':
                  getchar();
                  printf("\n\n\t 1. ADDITION DE DEUX IMAGES");
                  printf("\n\t 2. SOUSTRACTION IMAGE ");
                  printf("\n\t 3. MULTIPLICATION IMAGE ");
                  printf("\n\t 4. TRANSFORMATION LINEAIRE ");
                  printf("\n\t 5. EGALISATION HISTOGRAMME");
                  printf("\n\t 6. EXIT");
                  printf("\n\n QUELLE FONCTION: ");
                  scanf("%1[12345]c%*c", &choice2);
                  switch (choice2)
                  {
                  case '1':
                        printf("addition image\n");
                        sortie=additionerImage(entree,entree);
                        save(&sortie, image_output);
                        break;
                  case '2':
                        printf("soustraction image\n");
                        //Image entree2=recuperer_image("");
                        sortie=soustrationImage(entree,entree);
                        save(&sortie, image_output);
                        break;
                  case '3':
                        printf("multiplication par un ratio image\n");
                        multiplicationParRatio(&entree,1.5);
                        save(&entree, image_output);
                        break;
                  case '4':
                        printf("transformation lineaire\n");
                        sortie=transformationLineaire(entree);
                        save(&sortie, image_output);
                        break;
                  case '5':
                        printf("egalisation histogramme\n");
                        sortie=egalisationHistogramme(entree);
                        save(&sortie, image_output);
                        break;
                  
                  default:
                        break;
                  }
                  break;
            case '2':
                  getchar();
                  printf("\n\n\t 1. FILTRE MOYENNEUR");
                  printf("\n\t 2. FILTRE GAUSSIEN ");
                  printf("\n\t 3. FILTRE MEDIAN ");
                  printf("\n\t 4. EXIT");
                  printf("\n\n QUELLE FONCTION: ");
                  scanf("%1[1234]c%*c", &choice2);
                  switch (choice2)
                  {
                  case '1':
                        printf("Filtre moyenneur\n");
                        sortie = filtreMoyenneur(entree,21);
                        save(&sortie, image_output);
                        break;
                  case '2':
                        printf("Filtre Gaussien\n");
                        // 1 represente le gaussien flou
                        sortie = filtreGaussien(entree,1);
                        save(&sortie, image_output);
                        break;
                  case '3':
                        printf("filtre median\n");
                        sortie = filtreGaussien(entree,1);
                        save(&sortie, image_output);
                        break;
                  
                  default:
                        break;
                  }
                  break;
            case '3':
                  getchar();
                  printf("\n\n\t 1. FILTRE ROBERT");
                  printf("\n\t 2. FILTRE DE PREWITT ");
                  printf("\n\t 3. FILTRE DE SOBEL ");
                  printf("\n\t 4. FILTRE LAPLACIEN ");
                  printf("\n\t 5. EXIT");
                  printf("\n\n QUELLE FONCTION: ");
                  scanf("%1[1234]c%*c", &choice2);
                  switch (choice2)
                  {
                  case '1':
                        printf("Filtre de Robert\n");
                        sortie = filtreRobert(entree,'x');
                        save(&sortie, image_output);
                        break;
                  case '2':
                        printf("Filtre de Prewitt\n");
                        // 1 represente le gaussien flou
                        sortie = filtrePrewitt(entree,'x');
                        save(&sortie, image_output);
                        break;
                  case '3':
                        printf("filtre de Sobel\n");
                        sortie = filtreSobel(entree,'x');
                        save(&sortie, image_output);
                        break;
                  case '4':
                        printf("filtre du Laplacien\n");
                        sortie = filtreLaplacien(entree);
                        save(&sortie, image_output);
                        break;
                  
                  default:
                        break;
                  }
                  break;
            case '4':
                  printf("\nYOU SELECTED OPTION 1 %c", 4);
                  exit(0);
            default:
                  printf("\nINVALID SELECTION...Please try again");
            }
           // getchar();

}
int main(int argc, char *argv[])
{
      menu2(argv[1],argv[2]);
      return 0;
}
