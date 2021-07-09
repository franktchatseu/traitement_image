#define FONCTION_H

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
//initialiser une image
void initImage(Image *image,int width,int height,int max_val);
// verifier si deux images ont la meme dimension
int verif_dimension(Image image1, Image image2);
// lire image dans un fichier
void getPGMfile(char filename[], Image *img);
//sauvegarde de la matrice d'image dans un fichier
void save(Image *img, char output_file_name[]);
//modifier la luminance
void increaseLuminance(Image *img, int quantity);
// multiplication par un ratio
void multiplicationParRatio(Image *img, float ratio);
// addition de deux images
Image additionerImage(Image image1, Image image2);
//soustraction de deux images
Image soustrationImage(Image image1, Image image2);
// fonction de construction de histogramme
int nbOccurence(Image image, int niveau_gris);
Histogramme constructHistogramme(Image image);
void afficheHistogramme(Histogramme histo);
// egalisation de Histogramme
Image egalisationHistogramme(Image image);
// plus petit pixel de image
int getMinPixel(Image image);
// le grand pixel de image
int getMaxPixel(Image image);
// transformation lineaire
Image transformationLineaire(Image image);

// PARTIE 2: CONVOLUTION
int convolutionPixel(Image image,int pix_i,int pix_j);
Image Imageconvolution(Image imageEntre,int masqX,int masqY,int masque[masqY][masqX],int division);
//filtre moyenneur
Image filtreMoyenneur(Image image, int taille);
// filtre gaussien
Image filtreGaussien(Image image,int flou);
void filtreMedian(Image image);

// CONTOURS
// ici si axe=='x' on applique le filtre sur la vertical sinon sur horizontal
// filtre de prewitt
Image filtrePrewitt(Image image, char axe);
// filtre de robert
Image filtreRobert(Image image, char axe);
//filtre de sobel
Image filtreSobel(Image image,char axe);
// le laplacien
Image filtreLaplacien(Image image);