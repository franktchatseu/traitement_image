

// Read a PGM-format file into an array
// PGM file format is documented here:
// http://en.wikipedia.org/wiki/Netpbm_format
//
// Here's a short example:
//
// P2
// # a comment line
// 24 7
// 3
// 0 0 0 0 1 1 1 0 0 0 2 2 2 2 0 0 0 0 3 3 3 3 3 0
// ... 6 more rows of pixel data
//
// The P2 means a gray-scale image
// Comment lines start with # and can be ignored
// 24 means 24 wide
// 7 means 7 high
// 3 means the maximum pixel value is 3 in this example

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>


//typedef unsigned short int word ;
//typedef unsigned char byte ;

// Maximum size image we will process (larger ones
// get truncated to the upper left corner), or may "wrap"
// in interesting ways...
#define MAX_HEIGHT 1025
#define MAX_WIDTH 1025

// allows writing multiple pixels per line to the
// output PGM file.  Looks like many programs that
// use the PGM format just keep this at one.  It
// should not be more than 70 in any event.
#define MAX_PIXELS_PER_LINE 25

// Useful macro def
#define MIN( x, y ) ( ( x ) < ( y ) ? ( x ) : ( y ) )

// This is the array that will contain the image
// within your program.
//*** ????????????????????????????????


// You will need an identical  array to use to hold
// the output of your image transformation
//*** ????????????????????????????????

// ME DEFINING THEIMAGEARRAY

int theImageArray[1025][MAX_WIDTH];
int theImageArrayDup[1025][MAX_WIDTH];

// Internal function to read the header information
// stored at the beginning of a PGM format file.
//
// Used by read_pgm_file_into_array to see set the
// height and width of the image
FILE* read_pgm_file_info
(
    int* height,    // output
    int* width,    // output
    int* maxPixel,  // output
    char* pgmFname   // input
)
{
    static FILE* fp = 0L ;
    char pgmFormatFlag[3] =  { '\0' } ;

    char trash[80] = { '\0' } ;
    memset ( ( void * ) trash, '\0', 80 ) ;

    fp = fopen ( pgmFname, "r" ) ;

    if ( fp )
    {
        // Check the signature
        fscanf ( fp, "%2c\n", pgmFormatFlag ) ;
        //printf ( "%s\n", pgmFormatFlag ) ;
        if ( ! strcmp ( pgmFormatFlag, "P2" ) )
        {
            // Skip the comment line
            //fscanf ( fp, "\n" ) ;
            fgets (  trash, 70, fp ) ;
            //printf ( "%s", trash ) ;

            // Read the width
            fscanf ( fp, "%i", width ) ;
            //printf ( "%i,", *width ) ;

            // Read the height
            fscanf ( fp, "%i", height ) ;
            //printf ( "%i\n", *height ) ;

            // Read the maximum pixel value
            fscanf ( fp, "%i", maxPixel ) ;
        }
        //fclose ( fp ) ;
    }


    return fp ;
}

// Write out an array as a PGM file (ascii).
// writes imageArray to the file named by pgmOutFileName.
// commentLine, height, width, and maxPixel must be
// set and passed to this function so they can
// be written correctly in the PGM file header and
// so this function will know how many rows (image height)
// and how many columns (image width) to write.
void write_pgm_file_from_array
(
    char* pgmOutFileName,     // input
    int imageArray[][MAX_WIDTH], // input
    char* commentLine,      // input
    int height,          // input
    int width,          // input
    int maxPixel         // input
)
{
    int row = 0 ;
    int col = 0 ;
    FILE* fp = fopen( pgmOutFileName, "w" ) ;
    if ( fp )
    {
        // Generate the header info
        fprintf ( fp, "P2\n" ) ;
        fprintf ( fp, "%s\n", commentLine ) ;
        fprintf ( fp, "%u %u\n", width, height ) ;
        fprintf ( fp, "%u\n", maxPixel ) ;

        // Now write out the data, ensuring that at most 70
        // values appear on each line, even if width is > 70
        for ( row = 0 ; row < height ; row ++ )
        {
            for ( col = 0 ; col < width ; col ++ )
            {
                fprintf ( fp, "%u", imageArray[row][col] ) ;
                // break up long rows into multiple lines as needed
                if ( MAX_PIXELS_PER_LINE > 1 )
                {
                    fprintf ( fp, " " ) ;
                }
                if ( ( col % MAX_PIXELS_PER_LINE ) == 0 )
                {
                    fprintf ( fp, "\n" ) ;
                }
            }
            // Watch out of special case of width == 70
            if ( col % MAX_PIXELS_PER_LINE )
            {
                fprintf ( fp, "\n" ) ;
            }
        }
        fclose ( fp ) ;
    }
    return ;
}

// Read file named by pgmInFileName argument into
// the array imageArray. This function respects the
// MAX_HEIGHT and MAX_WIDTH values, so if you declare
// your target array using these bounds, even reading
// a large image file should not blow up your program.
// (but you will only get the upper left-hand corner
// of the image).  Checks that the file read is a PGM
// (gray scale, ascii) file.
//
// Sets height, width, and maxPixel according to the
// header read.
void read_pgm_file_into_array
(
    int imageArray[][MAX_WIDTH],          // output
    int* height,        // output
    int* width,        // output
    int* maxPixel,              // output
    char* pgmInFileName      // input
)
{
    int row = 0 ;
    int col = 0 ;
    FILE* fp = read_pgm_file_info ( height, width, maxPixel, pgmInFileName ) ;
    //char trash = ' ';
    //char yesThreshold = ' ';

    if ( fp )
    {
        printf ( "reading height=%d, width=%d\n", *height, *width ) ;
        for ( row = 0 ; row < MIN( MAX_HEIGHT - 1, *height ) ; row ++ )
        {
            for ( col = 0 ; col < MIN( MAX_WIDTH -1, *width ) ; col ++ )
            {
                fscanf ( fp, "%i", &imageArray[row][col]) ;
            }
        }
        fclose ( fp ) ;
    }

    return ;
}

int main( void )
{
    int height  = 0 ;
    int width = 0 ;
    int maxPixel = 0 ;

    //char* pgmInFileName = "lynx_central.pgm" ;
    //char* pgmOutFileName = "lynx_central_out.pgm" ;
    char* pgmInFileName = "lena.pgm" ;
    char* pgmOutFileName = "lena_out.pgm" ;

    // Read input file into array, then write it back out,
    // just to prove this all works.  Input file and output
    // file should look just the same.  (sizes on disk might
    // vary slightly)
    read_pgm_file_into_array (theImageArray, &height, &width, &maxPixel, pgmInFileName ) ;

    //****
    //**** Your code to transform the image goes here ...
    for(int i = 0; i < MAX_HEIGHT; i++){
        for(int j = 0; j < MAX_WIDTH; j++){
            theImageArrayDup[i][j] = theImageArray[i][j];
        }
    }

    imageProcessing(theImageArray, theImageArrayDup, height, width);

    //Transfer the current values in theImageArray back to disk
    write_pgm_file_from_array ( pgmOutFileName, theImageArray, "# JR test file", height, width,
                                maxPixel ) ;

    printf ( "Copying %s to %s, height=%u, width=%u, maxPixel=%d\n", pgmInFileName,
             pgmOutFileName,
             height, width, maxPixel ) ;

    //system("pause");
    return 0 ;
}

void convolve(int theImageArray[1025][1025], int theImageArrayDup[1025][1025], int height, int width){
    int boxBlur[3][3] = {
        { 1, 1, 1 },
        { 1,  1, 1 },
        { 1, 1, 1 }
    };

    int gaussian[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };
     int edgeDetect[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };

    int sharpen[3][3] = {
        {0, -1,  0},
        {-1, 4, -1},
        {0, -1,  0}
    };

    char convolution[30];
    printf("Would you like to run box blur, Gaussian blur, edge detection, or sharpen?\n(Box, Gaussian, Edge, Sharpen) \n");
    scanf("%s", convolution);
    lowerletter(convolution);

    if(strcmp(convolution, "box") == 0){
        for (int i = 1; i < MAX_HEIGHT; i++) {
            for (int j = 1; j < MAX_WIDTH; j++) {
                conMatrixMult(theImageArray, theImageArrayDup, boxBlur, i, j, 9, 0);
            }
        }
    }
    if(strcmp(convolution, "gaussian") == 0){
        for (int i = 1; i < MAX_HEIGHT; i++) {
            for (int j = 1; j < MAX_WIDTH; j++) {
                shapeMatrixMult(theImageArray, theImageArrayDup, gaussian, i, j, 16, 0);

            }
        }
    }
    if(strcmp(convolution, "edge") == 0){
        for (int i = 1; i < MAX_HEIGHT; i++) {
            for (int j = 1; j < MAX_WIDTH; j++) {
                conMatrixMult(theImageArray, theImageArrayDup, edgeDetect, i, j, 128, 0);

            }
        }
    }
    if(strcmp(convolution, "sharpen") == 0){
        for (int i = 1; i < MAX_HEIGHT; i++) {
            for (int j = 1; j < MAX_WIDTH; j++) {
                conMatrixMult(theImageArray, theImageArrayDup, sharpen, i, j, 8, -50);
                /*if((2*theImageArray[i][j])<= 255){
                    theImageArray[i][j] = (2*theImageArray[i][j]);
                }
                else{
                    theImageArray[i][j] = 255;
                }*/
                theImageArray[i][j] = 2*(theImageArray[i][j] -50) + 100;
                if (theImageArray[i][j] > 255) {
                    theImageArray[i][j] = 255;
                }
            }

        }
        printf("Image is washed out, sorry. Couldn't figure out a way around that. It is sharpened though.");
    }
}

void shapeDetection(int theImageArray[1025][1025], int theImageArrayDup[1025][1025], int height, int width)
{
   int prewitt_x[3][3] = {
        { -1, 0, 1 },
        { -1, 0, 1 },
        { -1, 0, 1 }
    };
    int prewitt_y[3][3] = {
        { -1, -1, -1 },
        { 0, 0, 0 },
        { 1, 1, 1 }
    };

    int sobel_x[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int sobel_y[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    int robert_x[3][3] = {
        { 1, 0},
        { 0, -1},
    };

    int robert_y[3][3] = {
        { 0, 1},
        { -1, 0},
    };

    int laplacien[3][3] = {
        {0, 1,  0},
        {1, -4, 1},
        {0, 1,  0}
    };
    
    char contour[30];
    printf("Which filter do you want to use do detect shapes?, sobel filter, prewitt filter, robert filter or laplacien?\n(Sobel, Prewitt, Robert, Laplacien) \n");
    scanf("%s", contour);
    lowerletter(contour); 

    if(strcmp(contour, "sobel") == 0){
        for (int i = 1; i < MAX_HEIGHT; i++) {
            for (int j = 1; j < MAX_WIDTH; j++) {
                conMatrixMult(theImageArray, theImageArrayDup, sobel_x, i, j, 5, -20) + conMatrixMult(theImageArray, theImageArrayDup, sobel_y, i, j, 5, -20);
            }
        }
    }
    if(strcmp(contour, "prewitt") == 0){
        for (int i = 1; i < MAX_HEIGHT; i++) {
            for (int j = 1; j < MAX_WIDTH; j++) {
                shapeMatrixMult(theImageArray, theImageArrayDup, prewitt_x, i, j, 5, -20) + shapeMatrixMult(theImageArray, theImageArrayDup, prewitt_y, i, j, 5, -20);
            }
        }
    }
    if(strcmp(contour, "robert") == 0){
        for (int i = 1; i < MAX_HEIGHT; i++) {
            for (int j = 1; j < MAX_WIDTH; j++) {
                shapeMatrixMult(theImageArray, theImageArrayDup, robert_x, i, j, 1, 0);

                if (theImageArray[i][j] < 0) {
                    theImageArray[i][j] = 0;
                }
            }
        }
    }
    if(strcmp(contour, "laplacien") == 0){
        for (int i = 1; i < MAX_HEIGHT; i++) {
            for (int j = 1; j < MAX_WIDTH; j++) {
                shapeMatrixMult(theImageArray, theImageArrayDup, laplacien, i, j, 1, 0);
                /*if((2*theImageArray[i][j])<= 255){
                    theImageArray[i][j] = (2*theImageArray[i][j]);
                }
                else{
                    theImageArray[i][j] = 255;
                }*/
                if (theImageArray[i][j] < 0) {
                    theImageArray[i][j] = 0;
                }
            }
        }
    }
}

void conMatrixMult(int theImageArray[1025][1025], int theImageArrayDup[1025][1025], int myMatrix[3][3], int i, int j, int divisor, int subtractor){
    theImageArray[i][j] = (theImageArray[i+1][j+1]*myMatrix[0][0] + theImageArray[i][j+1]*myMatrix[0][1]+ theImageArray[i-1][j+1]*myMatrix[0][2]+theImageArray[i+1][j]*myMatrix[1][0]+theImageArray[i][j]*myMatrix[1][1]+theImageArray[i-1][j]*myMatrix[1][2]+theImageArray[i+1][j-1]*myMatrix[2][0]+theImageArray[i][j+1]*myMatrix[2][1]+theImageArray[i-1][j-1]*myMatrix[2][2])/divisor - subtractor;
}

void shapeMatrixMult(int theImageArray[1025][1025], int theImageArrayDup[1025][1025], int myMatrix[3][3], int i, int j, int divisor, int subtractor){
    theImageArray[i][j] = (theImageArray[i-1][j-1]*myMatrix[0][0] + theImageArray[i-1][j]*myMatrix[0][1]+ theImageArray[i-1][j+1]*myMatrix[0][2]+theImageArray[i+1][j]*myMatrix[2][1]+theImageArray[i][j]*myMatrix[1][1]+theImageArray[i][j-1]*myMatrix[1][0]+theImageArray[i][j+1]*myMatrix[1][2]+theImageArray[i+1][j+1]*myMatrix[2][2]+theImageArray[i][j-1]*myMatrix[0][2])/divisor - subtractor;
}

void invert(int theImageArray[1025][1025]){
    for (int i = 1; i < MAX_HEIGHT; i++) {
        for (int j = 1; j < MAX_WIDTH; j++) {
            theImageArray[i][j] = 255 - theImageArray[i][j];
        }
    }

}

// to sort neighbor vector to det median element
int sort_vector(int tab[9]){
    int imin, min, tmp;

    for(int i=0; i<9; i++){
        imin = i; min= tab[i];
		for(int j=i+1; j<9; j++){
			if(tab[j] < tab[imin]){
				imin = j;
			}
			if (imin != i)
			{
				tmp = tab[i];
				tab[i]= tab[imin];
				tab[imin] = tmp;
			}
			
		}
    }

	return tab[4];
}

// Filtre de convolution median 3x3
void medianFilter(int theImageArray[MAX_HEIGHT][MAX_WIDTH]){
    int neighbor[9];
    int i, j, median;

    printf("FIlter Median 3x3\n");

    for (i = 1; i < MAX_HEIGHT-1; i++)
    {
        for (j = 1; j < MAX_WIDTH-1; j++)
        {
            neighbor[0] = theImageArray[i][j];
            neighbor[1] = theImageArray[i-1][j];
            neighbor[2] = theImageArray[i+1][j];
            neighbor[3] = theImageArray[i][j+1];
            neighbor[4] = theImageArray[i][j-1];
            neighbor[5] = theImageArray[i-1][j-1];
            neighbor[6] = theImageArray[i+1][j+1];
            neighbor[7] = theImageArray[i-1][j+1];
            neighbor[8] = theImageArray[i+1][j-1];
            
            median = sort_vector(neighbor);
            theImageArray[i][j] = median;
        }
        
    }
    
}

void threshold(int theImageArray[1025][1025]) {


    int n;
    printf("Enter the number of levels you wish to threshold by. \n(Integer between 2-6 inclusive). \n");
    scanf("%d", &n);

    for (int i = 0; i < MAX_HEIGHT; i++) {
        for (int j = 0; j < MAX_WIDTH; j++) {
            switch (n){

            case 1:
                printf("Invalid response. Please start over. /n");
                break;

            case 2:
                if (theImageArray[i][j] > (n-1)*255/n) {
                    theImageArray[i][j] = 255;
                }
                else {
                    theImageArray[i][j] = 0;
                }
                break;
            case 3:
                if (theImageArray[i][j] > (n-1)*255/n) {
                    theImageArray[i][j] = 255;
                }
                else if (theImageArray[i][j] > (n-2)*(255/n)){
                    theImageArray[i][j] = (n-2)*(255/n);
                }
                else {
                    theImageArray[i][j] = 0;
                }
                break;
            case 4:
                if (theImageArray[i][j] > (n-1)*255/n) {
                    theImageArray[i][j] = 255;
                }
                else if (theImageArray[i][j] > (n-2)*(255/n)){
                    theImageArray[i][j] = (n-2)*(255/n);
                }
                else if (theImageArray[i][j] > (n-3)*(255/n)){
                    theImageArray[i][j] = (n-3)*(255/n);
                }
                else {
                    theImageArray[i][j] = 0;
                }
                break;
            case 5:
                if (theImageArray[i][j] > (n-1)*255/n) {
                    theImageArray[i][j] = 255;
                }
                else if (theImageArray[i][j] > (n-2)*(255/n)){
                    theImageArray[i][j] = (n-2)*(255/n);
                }
                else if (theImageArray[i][j] > (n-3)*(255/n)){
                    theImageArray[i][j] = (n-3)*(255/n);
                }
                else if (theImageArray[i][j] > (n-4)*(255/n)){
                    theImageArray[i][j] = (n-4)*(255/n);
                }
                else {
                    theImageArray[i][j] = 0;
                }
                break;
            case 6:
                if (theImageArray[i][j] > (n-1)*255/n) {
                    theImageArray[i][j] = 255;
                }
                else if (theImageArray[i][j] > (n-2)*(255/n)){
                    theImageArray[i][j] = (n-2)*(255/n);
                }
                else if (theImageArray[i][j] > (n-3)*(255/n)){
                    theImageArray[i][j] = (n-3)*(255/n);
                }
                else if (theImageArray[i][j] > (n-4)*(255/n)){
                    theImageArray[i][j] = (n-4)*(255/n);
                }
                else if (theImageArray[i][j] > (n-5)*(255/n)){
                    theImageArray[i][j] = (n-5)*(255/n);
                }
                else {
                    theImageArray[i][j] = 0;
                }
                break;
            default:
                printf("Invalid response. Please start over. /n");
                break;
            }
        }
    }
}

void luminosity(int theImageArray[1025][1025]) {
    char lum[10];
    double perc;
    printf("Do you want to lighten or darken the image? (Lighten, Darken) \n");
    scanf("%s", lum);
    lowerletter(lum);

    printf("Enter a percentage between 0 and 100 (without the %%) of how much you want\nthe luminosity to change by. \n");
    scanf("%lf", &perc);
    perc = 0.01* perc;


    if(strcmp(lum, "lighten") == 0){
        perc = 1.0 + perc;

        for (int i = 0; i < MAX_HEIGHT; i++) {
            for (int j = 0; j < MAX_WIDTH; j++) {
                if(perc*theImageArray[i][j]<= 255){
                    theImageArray[i][j] = perc*theImageArray[i][j];
                }
                else{
                    theImageArray[i][j] = 255;
                }
            }
        }
    }

    if(strcmp(lum, "darken") == 0){
        perc = 1.0 - perc;

        for (int i = 0; i < MAX_HEIGHT; i++) {
            for (int j = 0; j < MAX_WIDTH; j++) {
                theImageArray[i][j] = perc*theImageArray[i][j];
            }
        }
    }
}

// calcul d'histogramme
int histogramme(int theImageArray[1025][1025]){
    int h[256] = {0};
    int i, j, p;
    for(i=0; i<= MAX_HEIGHT; i++){
        for(j=0; j<= MAX_WIDTH; j++){
            p = theImageArray[i][j];
            h[p] += 1;
        }
    }
    for ( i = 0; i < 256; i++)
    {
        printf("pixel %d => valeur = %d\n", i, h[i]);
    }
}


//1. det min pixel
int minimum(int theImageArray[1025][1025]){

    int min = theImageArray[0][0];

    for(int i=0; i<MAX_HEIGHT;i++){
        for(int j=0; j<MAX_WIDTH; j++){
            if (theImageArray[i][j]<min){
                min = theImageArray[i][j];
            }
        }
    }
    return min;
}

//2. det max pixel
int maximum(int theImageArray[1025][1025]){
    int max = theImageArray[0][0];
    for(int i=0; i< MAX_HEIGHT;i++){
        for(int j=0; j<MAX_WIDTH; j++){
            if (theImageArray[i][j]>max){
                max = theImageArray[i][j];
            }
        }
    }
    return max;
}

// Transformation lineaire
void linearTransformation(int theImageArray[MAX_HEIGHT][MAX_WIDTH]){
    int i, j;
    int min = minimum(theImageArray);
    int max = maximum(theImageArray);
    int s = max - min;

    for(i=0; i<= MAX_HEIGHT; i++){
        for(j=0; j<= MAX_WIDTH; j++){
            theImageArray[i][j] = 255 * (theImageArray[i][j] - min) / s;
        }
    }
}

// Histogramme Equalisation
void histogrammeEqualisation(int theImageArray[MAX_HEIGHT][MAX_WIDTH]){
    int h[256] = {0};
    int i, j, p;
    int total = MAX_WIDTH * MAX_HEIGHT;

    //1. calcule de l'histogramme
    for(i=0; i<= MAX_HEIGHT; i++){
        for(j=0; j<= MAX_WIDTH; j++){
            p = theImageArray[i][j];
            h[p] += 1;
        }
    }

    //2. Normalisation de l'histogramme
    float hn[256]; 
    for (i = 0; i < 256; i++)
    {
        hn[i] = (float)h[i]/total;
    }

    
    //3. Densite de probabilite normalisee
    float c[256];
    c[0] = hn[0];
    for ( i = 1; i < 256; i++)
    {
        c[i] = c[i-1] + hn[i];
    }

    //4. Transformation des niveaux de gris de l'image
    for(i=0; i<= MAX_HEIGHT; i++){
        for(j=0; j<= MAX_WIDTH; j++){
            theImageArray[i][j] = c[theImageArray[i][j]] * 255;
        }
    }

}

//Image Addition
void add_image(int theImageArray[MAX_HEIGHT][MAX_WIDTH]){

    for (int i = 0; i < MAX_HEIGHT; i++)
    {
        for (int j = 0; j < MAX_WIDTH; j++)
        {
            theImageArray[i][j] += theImageArray[i][j];
            if(theImageArray[i][j]>255)
            {
                theImageArray[i][j]=255;
            }
        }
        
    }
    
}


void imageProcessing(int theImageArray[1025][1025], int theImageArrayDup[1025][1025], int height, int width) {
    char inputFunc[30];

    printf("Enter the function you wish to apply to your image. (Threshold, Luminosity, Convolve, Invert, histogram, lineartrans, histoequal) \n");
    scanf("%s", inputFunc);
    lowerletter(inputFunc);

    if(strcmp(inputFunc,"invert") == 0) {
        invert(theImageArray);
    }

    if(strcmp(inputFunc,"convolve") == 0) {
        convolve(theImageArray, theImageArrayDup, height, width);
    }

    if(strcmp(inputFunc, "threshold") == 0){
        threshold(theImageArray);
    }

    if(strcmp(inputFunc, "luminosity") == 0){
        luminosity(theImageArray);
    }

    if (strcmp(inputFunc, "histogram") == 0) {
        histogramme(theImageArray);
    }

    if(strcmp(inputFunc, "lt") == 0){
        linearTransformation(theImageArray);
    }

    if(strcmp(inputFunc, "histoequal") == 0){
        histogrammeEqualisation(theImageArray);
    }

    if(strcmp(inputFunc, "median") == 0){
        medianFilter(theImageArray);
    }

    if(strcmp(inputFunc, "addition") == 0){
        add_image(theImageArray);
    }

    if(strcmp(inputFunc, "shape") == 0){
        shapeDetection(theImageArray, theImageArrayDup, height, width);
    }

    printf("\nDone\n");
}


void lowerletter(char myString[30]){
    for(int i = 0; i < (int)strlen(myString); i++){
        myString[i] = tolower(myString[i]);
    }

}
