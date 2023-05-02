// ppm_io.c
//Devin Ramsden; dramsde3@gmail.com
#include <string.h>
#include <assert.h>
#include "ppm_io.h"
#include <stdlib.h>
#include <ctype.h>

/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image * read_ppm(FILE *fp) {
    assert(fp);
    int cols;
    int rows;
    char str[20];

    //check if the file start with P6
    int check_scan = fscanf(fp, "%s", str);
    if(check_scan== EOF || check_scan==0 || (strcmp("P6", str)!=0)){
        return NULL;
    }

    //see if there is a comment line
    char comment = ' ';
    while((int)isspace(comment) && comment!=EOF && comment!=0){
        comment = (char)fgetc(fp);
    }

    //if the file ends with P6
    if(comment==EOF || comment==0){
        return NULL;
    }
    //if there is a comment line, ignore the commont
    else if(comment == '#'){
        while(fgetc(fp)!='\n'){}
    }
    //if there is no comment, put the char back to the file
    else{
        ungetc(comment, fp);
    }

    //get the col & row #
    check_scan = fscanf(fp, " %d %d ", &cols, &rows);
    if(check_scan!=2 || cols<=0 || rows<=0){//it won't make sense if the row or col # is not positive
        return NULL;
    }

    //check if the following number is 255
    int i =0;
    check_scan = fscanf(fp, " %d ", &i);
    if(check_scan!=1 || i!=255){
        return NULL;
    }


    //create the image to read to
    Image *im = (Image*)malloc(sizeof(Image));
    if (im==NULL) {
        return NULL;
    }
    im->rows = rows;
    im->cols = cols;
    im->data = (Pixel*)malloc(sizeof(Pixel) * im->rows * im->cols);
    if(im->data==NULL) {
        free(im);
        return NULL;
    }

    //read the file to image
    fread(im->data,sizeof(Pixel),(im->rows*im->cols),fp);
    return im;
}


/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im) {

    // check that fp is not NULL
    assert(fp); 

    // write PPM file header, in the following format
    // P6
    // cols rows
    // 255
    fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);

    // now write the pixel array
    int num_pixels_written = fwrite(im->data, sizeof(Pixel), im->rows * im->cols, fp);

    if (num_pixels_written != im->rows * im->cols) {
        fprintf(stderr, "Uh oh. Pixel data failed to write properly!\n");
    }

    return num_pixels_written;
}

