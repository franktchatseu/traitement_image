#include <stdlib.h>
#include "convolution.c"
#include <stdio.h>

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
