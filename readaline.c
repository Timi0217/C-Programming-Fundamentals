/*
* Favour Okereke-Mba; FOKERE01
* Timi Dayo-Kayode; ODAYOK01
* COMP 40 - HW 1: PART B: Read a line
* 01/27/2018
*/

#include <stdlib.h>
#include <stdio.h>
#include "readaline.h"

/*resize uses realloc to grow or shrink the line array*/
char *resize(char *line, size_t new_size);

size_t readaline(FILE *inputfd, char **datapp){
        size_t byte_size = 0;

        /*Validating arguments passed to the readaline program*/
        if (inputfd == NULL || datapp == NULL){
                fprintf(stderr, "readaline: invalid arguments\n");
                exit(EXIT_FAILURE);
        }

        int i = 0, size = 200;
        char c;
        char *line = calloc(size ,sizeof(char));

        /*checking that memory allocation for the line was successful*/
        if (line == NULL){
                fprintf(stderr, "readaline: memory allocation failed\n");
                exit(EXIT_FAILURE);
        }

        while (!feof(inputfd)) {
                c = fgetc(inputfd);

                /*catching any errors while reading from the provided file*/
                if (ferror(inputfd)) {
                        fprintf(stderr, "readaline: error reading file\n");
                        exit(EXIT_FAILURE);
                }

                if (i == 0 && feof(inputfd)) {
                        free(line);
                        *datapp = NULL;
                        return 0;
                }

                /*checking that the program has reached the end of the line*/
                if (c == '\n') {
                                byte_size += sizeof(c);
                                line[i] = c;
                        break;
                }
        
                /*checking if the input is beyond the limits of the program*/
                if (i == size && c != '\n') {
                        size = (size * 2) + 1;
                        line = resize(line, size);
                }
                
                byte_size += sizeof(c);
                line[i] = c;
                i++;
        }
        line = resize(line, byte_size);
        *datapp = line;
        return byte_size;
}

/*
*Function: resize
*Parameters: a pointer a char array, the size of the array
*/
char *resize(char *line, size_t new_size){
        char *hold;
        hold = realloc(line, new_size);
        if (hold == NULL) {
                free(line);
                fprintf(stderr, "readaline: memory allocation failed\n");
                exit(EXIT_FAILURE);
        }
        return hold;
}