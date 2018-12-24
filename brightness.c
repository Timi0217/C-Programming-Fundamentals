/*
* Favour Okereke-Mba; FOKERE01
* Timi Dayo-Kayode; ODAYOK01
* COMP 40 - HW 1: PART A: Brightness
* 01/26/2018
*/

#include <stdlib.h>
#include <stdio.h>
#include<math.h>
#include "pnmrdr.h"

enum { STDIN = 1, COM_ARG = 2};

int main(int argc, char const *argv[])
{
        FILE *image;
        Pnmrdr_T analyze;
        /*Reading in from stdin*/
        if (argc == STDIN) {
                analyze = Pnmrdr_new(stdin);
        }
        /*Reading in file as a command line argument*/
        else if (argc == COM_ARG) {
                image = fopen(argv[1], "rb");
                if (image == NULL){
                        fprintf(stderr,"brightness: File failed to open\n");
                        exit(EXIT_FAILURE);
                }
                analyze = Pnmrdr_new(image);
        }
        else {
                fprintf(stderr,
                "brightness: Invalid number of command line arguments\n");
                exit(EXIT_FAILURE);
        }

        Pnmrdr_mapdata image_data = Pnmrdr_data(analyze);
        
        /*Checking that the provided image type is supported*/
        if (image_data.type != Pnmrdr_gray || 
                (image_data.width * image_data.height) <= 0){

                fprintf(stderr,"brightness: Wrong image format\n");
                exit(EXIT_FAILURE);
        }

        unsigned int i; 
        double brightness_avg, pixel, brightness_sum = 0;

        /*compute the sum of the brightness of each pixel of the image*/
        for (i = 0; i < image_data.width * image_data.height; i++) {
                pixel = Pnmrdr_get(analyze);
                brightness_sum = brightness_sum + 
                                (pixel / image_data.denominator);
        }

        /*computes the average brightness of the image file*/
        brightness_avg = brightness_sum / 
                        (image_data.width * image_data.height);
        printf("%0.3f\n", brightness_avg);

        Pnmrdr_free(&analyze);
        return EXIT_SUCCESS;
}