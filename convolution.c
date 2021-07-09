#include <stdlib.h>
#include "fonction.h"
#include <stdio.h>


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

