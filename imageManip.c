//imageManip.c:
//Devin Ramsden; dramsde3@gmail.com
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ppm_io.h"
#include "imageManip.h"

/*
 * This function gets a Pixel pointer and turns its rgb values into the contrast effect
 */
void adjust_contrast(double factor, Pixel* p){

    //convert values 0~225 to -0.5~0.5
    double red = ((p->r *1.0) /255 -0.5) * factor; 
    double green = ((p->g *1.0) /255 -0.5) * factor; 
    double blue = ((p->b *1.0) /255 -0.5) *factor;

    //restrict all the values within the boundary
    if(red>0.5) {red=0.5;}
    else if(red<-0.5) {red = -0.5;}

    if(green>0.5){ green = 0.5;}
    else if(green<-0.5){ green = -0.5;}

    if(blue>0.5){ blue=0.5;}
    else if(blue<-0.5){ blue=-0.5;}
    
    //convert values from -0.5~0.5 to 0~225
    p->r = (red+0.5)*255;
    p->g = (green+0.5)*255;
    p->b = (blue+0.5)*255;

}

/*
 * This function takes in an Image pointer, 
 * and frees its data, i.e. its Pixel pointer, and itself
 */
void free_all(Image *im){
    free(im->data);
    free(im);
}

/*
 * The swap function swaps the color of the input image 
 * it takes in an Image pointer,
 * and changes its red values to blue, green to red, and blue to green.
 */
void swap(Image * im){

    int row = im->rows; //store the rows of the image for easier representation later
    int col = im->cols; //store the cols of the image for easier representation later

    int pix_num = 0; //this int will represent the index of the pixel that will be modified
    unsigned char temp = 0;

    for(int i=0; i < row; i++){
        for(int j=0; j < col; j++){
            pix_num = i*col + j; //calculate the index of the pixel we want to modify
            //swap colors
            temp = im->data[pix_num].r;
            im->data[pix_num].r = im->data[pix_num].g;
            im->data[pix_num].g = im->data[pix_num].b;
            im->data[pix_num].b = temp;
        }
    }
}


/*
 * The grayscale function applies a grayscale to the picture.
 * It takes in an Image pointer, and turns the image into gray.
 */
void grayscale(Image * im){
    int row = im->rows;
    int col = im->cols;

    int pix_num = 0;
    unsigned char intensity = 0;

    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            pix_num = i*col+j;
            //calculate the intensity of this pixel
            intensity = (unsigned char)
                (((double)im->data[pix_num].r*0.30) 
                 + ((double)im->data[pix_num].g*0.59) 
                 + ((double)im->data[pix_num].b*0.11));
            //turn the pixel into gray
            im->data[pix_num].r = intensity;
            im->data[pix_num].g = intensity;
            im->data[pix_num].b = intensity;
        }
    }

}


/*
 * The contrast function emphsize the colors according to the given contrast value.
 * It takes in an Image pointer and the contrast value.
 */
void contrast(Image * im, double factor){
    int pix_num = 0;

    int row = im->rows;
    int col = im->cols;

    for(int i =0; i<row; i++){
        for(int j =0; j<col; j++){
            pix_num=i*col+j;
            //call the helper function to turn each pixel into its contrast effect
            adjust_contrast(factor, (im->data+pix_num));
        }
    }
}


/*
 * The zoom_in function returns a zoomed in version of the original image.
 * It takes in an Image pointer, 
 * and zoom it in by 4 times of the size.
 */
Image * zoom_in(Image * im){

    //create the Image we want to return, i.e. the zoomed in image
    Image *result = (Image *)malloc(sizeof(Image));
    result->rows = 2 * im->rows;//the new image should have doubled number of rows
    result->cols = 2 * im->cols;

    int row = im->rows; //indicate the original # of rows
    int col = im->cols; //indicate the original # of cols
    //the new image should have 4 times the original pixels
    Pixel *res_p = (Pixel *)malloc(sizeof(Pixel)*row*col*4);
    result->data = res_p;

    int pix_num=0;

    //the rows and coloums of the 4 new corresponding pixels
    int col1 = 0;
    int col2 = 0;
    int row1 = 0;
    int row2 = 0;

    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){

            pix_num = i*col +j;

            //calculate the corresponding rows and columns
            col1 = j*2;
            col2 = col1+1;
            row1 = i*2;
            row2 = row1+1;

            //give the original pixel value to the corresponding new pixels
            res_p[(row1*col*2)+col1] = im->data[pix_num];
            res_p[(row1*col*2)+col2] = im->data[pix_num];
            res_p[(row2*col*2)+col1] = im->data[pix_num];
            res_p[(row2*col*2)+col2] = im->data[pix_num];
        }
    }

    free_all(im); //free the original image
    return result;
}
/*
 * The zoom_out function returns a zoomed out version of the function.
 * It takes in an Image pointer,
 * and zoom it out by 4 times of the size.
 */
Image * zoom_out(Image * im){

    //create a new image, i.e. the zoomed out image
    Image *result = (Image *)malloc(sizeof(Image));
    result->rows = im->rows/2; //the new image's rows should be half of the original's
    result->cols = im->cols/2; //the new image's cols should be half of the original's

    Pixel *res_p = (Pixel *)malloc(sizeof(Pixel) * result->rows * result->cols);
    result->data=res_p;

    //these represent the index of the 4 corresponding pixels in the original image 
    int upper_left = 0;
    int upper_right = 0;
    int lower_left = 0;
    int lower_right = 0;

    int pix_num = 0; //the index of the corresponding pixel in the new image

    for(int i=0; i<result->rows; i++){
        for(int j=0; j<result->cols; j++){

            //calculate the cooresponding indicies in the original image
            upper_left = (i*2)*(im->cols) + j*2;
            upper_right = (i*2)*(im->cols) + j*2+1;
            lower_left = (i*2+1)*(im->cols) +j*2;
            lower_right = (i*2+1)*(im->cols) +j*2+1;

            pix_num=i*result->cols+j;
            
            //calculate the average RGB values in the original image
            //and assign the average to the new image
            res_p[pix_num].r = (im->data[upper_left].r + im->data[upper_right].r 
                        + im->data[lower_left].r + im->data[lower_right].r)/4.0;

            res_p[pix_num].g = (im->data[upper_left].g + im->data[upper_right].g
                        + im->data[lower_left].g + im->data[lower_right].g)/4.0;

            res_p[pix_num].b = (im->data[upper_left].b + im->data[upper_right].b 
                        + im->data[lower_left].b + im->data[lower_right].b)/4.00;
        }
    }
    free_all(im); //free the original image
    return result;
}


/*
 * The occlude function returns a version of the image,
 *  where there is a strip blacked out indicated by the top left and bottom right indices.
 *  It takes in an Image pointer, 
 *  and 4 numbers representing the coordinates of rectangle wanted to be blacked out.
 */
Image * occlude(Image * im, int top_left_r, int top_left_c, int bottom_right_r, int bottom_right_c){

    //check if the coordinate values are valid
    if(top_left_r < 0 || top_left_c < 0 || bottom_right_r >= im->rows || bottom_right_c >= im->cols
            || top_left_r > bottom_right_r || top_left_c > bottom_right_c){
        free_all(im);
        return NULL;
        //we return NULL here so that in the main function, we can detect that the coordinates are not vaild
    }

    int pix_num = 0;

    for(int i=top_left_r; i<=bottom_right_r; i++){
        for(int j=top_left_c; j<=bottom_right_c; j++){

            pix_num = i*im->cols + j;

            //assign black color to the certain  pixels 
            im->data[pix_num].r = 0;
            im->data[pix_num].g = 0;
            im->data[pix_num].b = 0;
        }
    }
    return im;
}

/*
 * This is a helper function for the g_num function to aid in creating the gaussian distribution.
 */
double sq(double val){
    return (val*val);
}

/*
 * g_num gets the individual values that will fill up the gaussian distribution that will act as a filter for our blur function.
 */
double g_num(double sigma, int dx, int dy){
    double PI = 3.14159265358979323846264;
    double g = (1.0 / (double)(2.0 * PI * sq(sigma))) * ((double)exp((-((double)sq(dx) + (double)sq(dy))) / (double)(2.0 * sq(sigma))));
    return g;
}

/*
 * This function creates the filter that will help in creating the weighted pixel values.
 */
void gaussian_dis(double sigma, int n, double arr[n][n]){
    int dx = 0;
    int dy = 0;
    int mid = (n - 1) / 2;
    for (int i = 0; i < n; i++){
        dx = mid - i;
        for (int j = 0; j < n; j++) {
            dy = mid - j;
            double g = g_num(sigma,dx,dy);
            arr[i][j] = g;
        }
    }
}

/*
 * This is a function that will help create our final blured pixel array by multiplying the pixels value by the filter values and then finding the normalized sum.
 */
void  blurdat(int size, Image *im, int n, double filter[n][n], int i, int j, Image * holder) {
    int start_x = i - (size/2);
    int start_y = j - (size/2);
    double rsum = 0;
    double gsum = 0;
    double bsum = 0;
    double sum = 0;
    for (int r  = 0; r <= size; r++){
        for (int c = 0; c <= size; c++) {
            if(r+start_x >= 0 && r+start_x < im->rows && c+start_y >= 0 && c+start_y < im->cols){
                rsum += im->data[(r+start_x)*im->cols+(c+start_y)].r * filter[r][c];
                gsum += im->data[(r+start_x)*im->cols+(c+start_y)].g * filter[r][c];
                bsum += im->data[(r+start_x)*im->cols+(c+start_y)].b * filter[r][c];
                sum += filter[r][c];
            } 
        }
    }

    //normalization
    holder->data[i*im->cols+j].r = rsum / sum;
    holder ->data[i*im->cols+j].g = gsum / sum;
    holder ->data[i*im->cols+j].b = bsum / sum;
}

/*
 * This blur function will blur according to the sigma value the original value. 
 */
Image * blur(Image * im, double sigma){
    int n = 0;
    if ((int)(10*sigma) % 2 == 0) {
        n = (int)(10*sigma) + 1;
    }
    else {
        n = (int)(10*sigma);
    }
    double filter[n][n];
    int size = (int)(sigma * 10);
    gaussian_dis(sigma,n,filter);
    Image * holder = malloc(sizeof(Image));
    holder->rows=im->rows;
    holder->cols=im->cols;
    holder->data = malloc(sizeof(Pixel) * im->rows * im->cols);
    for (int i = 0; i < im->rows; i++){
        for (int j = 0; j < im->cols; j++){
            blurdat(size, im,n,filter,i,j,holder);
        }
    }
    free(im->data);
    free(im);
    return holder;
}






