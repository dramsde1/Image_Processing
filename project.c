//project.c
// Devin Ramsden; dramsde1@jhu.edu>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ppm_io.h"
#include "imageManip.h"
int main(int argc, char* argv[]){

    //check if both input filename and output filename are supplied
    if(argc<3){ 
        return 1; 
    }

    //open the file
    FILE *fp  = NULL;           //the file pointer from which we read
    fp = fopen(argv[1], "rb");
    if(fp==NULL){               //if specified input file could not be opened
        return 2;           
    }

    //read file to image
    Image * orig = NULL;      //the image we want to modify
    orig = read_ppm(fp);      //read file to image
    if(orig==NULL){           //if read fails
        return 3;
    }
    fclose(fp);


    //check if the operation name is specified, and if it is valid
    if(argc==3 || !(strcmp("swap", argv[3])==0 || strcmp("grayscale", argv[3])==0 || strcmp("contrast", argv[3])==0
            || strcmp("zoom_in", argv[3])==0 || strcmp("zoom_out", argv[3])==0 
            || strcmp("occlude", argv[3])==0 || strcmp("blur", argv[3])==0)){
        free_all(orig);
        return 4; 
    }

    //correct opention is given
    //see which modification function to call & modify
    else if(strcmp("swap", argv[3])==0) {               //swap is called
        if(argc!=4){   //check the number of arguments
            free_all(orig);
            return 5;
        }
        swap(orig);
    } 

    else if(strcmp("grayscale", argv[3])==0) {         //grayscale is called
        if(argc!=4){    //check the number of arguments 
            free_all(orig);
            return 5;
         }
        grayscale(orig);
    }

    else if(strcmp("contrast", argv[3])==0){           //contrast is called
        if(argc!=5){    //check the number of arguments 
            free_all(orig);
            return 5;
        }

        //check if the contrast number is vaild
        double rate;
        int check = sscanf(argv[4], "%lf", &rate);
        if(check!=1 || rate<=0){ 
            free_all(orig); 
            return 5;
        } 
            contrast(orig, rate);
    }

    else if(strcmp("zoom_in", argv[3])==0){             //zoom_in is called
        if(argc!=4){    //check the number of arguments 
            free_all(orig);
            return 5;
        }
        orig=zoom_in(orig);
    }

    else if(strcmp("zoom_out", argv[3])==0){            //zoom_out is called
        if(argc!=4){    //check the number of arguments 
            free_all(orig);
            return 5;
        }
        orig=zoom_out(orig);
    }

    else if(strcmp("occlude", argv[3])==0){             //occlude is called
        if(argc!=8){    //check the number of arguments 
            free_all(orig);
            return 5;
        }

        //check if the rest inputs are valid
        int ul_r=0;
        int ul_c =0;
        int br_r=0;
        int br_c =0;
        int check = sscanf(argv[4], " %d ", &ul_c);
        check += sscanf(argv[5], " %d ", &ul_r);
        check += sscanf(argv[6], " %d ", &br_c);
        check += sscanf(argv[7], " %d ", &br_r);
        if(check!=4){
            free_all(orig);
            return 5;
        }

        orig=occlude(orig, ul_r, ul_c, br_r, br_c);
        
        //check if arguments for occlude operation were out of range for the given input image
        if(orig == NULL){
            return 6; 
        }
    }


    else if(strcmp("blur", argv[3])==0){                 //blur is called
        if(argc!=5){    //check the number of arguments 
            free_all(orig);
            return 5;
        }

        //check if the sigma is valid
        double sigma;
        int check = sscanf(argv[4], " %lf", &sigma);
        if(check!=1 || sigma<=0){ //Incorrect number or kind of arguments specified for blur
            free_all(orig);
            return 5;
        }
        orig = blur(orig, sigma);
    } 


    //image is modified, write it to the new file
    FILE *new_fp = fopen(argv[2], "wb"); //the file pointer which we write to
    write_ppm(new_fp, orig);

    fclose(new_fp);

    //free the image
    free_all(orig);

    //No errors detected
    return 0;
}
