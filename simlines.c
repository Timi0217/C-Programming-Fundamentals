/*
* Favour Okereke-Mba; FOKERE01
* Timi Dayo-Kayode; ODAYOK01
* COMP 40 - HW 1: PART C: Read a line
* 02/01/2018
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "atom.h"
#include "list.h"
#include "readaline.h"

/*Checks that a character is an alphabet, digit, or underscore('_')*/
int iswordchar(char my_char);

/*gets the next line of the file, parses it and returns its atom equivalent*/
const char *reader(FILE *inputfd);

/*passed into list map to free the values in a list*/
void free_list(void **line, void *cl);

/*passed into listmap to print match groups*/
void print_list(void **value,void *cl);

/*Removes non-alphanumeric characters except '_'*/
char *parse(char *line, size_t *line_size);

/*used to free data in the case of an error while reading from file*/
void error_free(const void *key, void **value,void *cl);

/*passed into the tablemap to free the values of a table */
void free_values(const void *key, void **value,void *cl);

/*passed into tablemap to print match groups*/
void print_groups(const void *key, void **value,void *cl);

/*struct that contains the location info of a line*/
typedef struct{
        char *filename;
        int line_num;
} value;

int main(int argc, char *argv[]){

        int i, line_num;
        Table_T sim_groups = Table_new(0, NULL, NULL);

        for (i = 1; i < argc; i++) {
                line_num = 0;
                FILE *input_data = fopen(argv[i], "r");

                /*error checking of file and handles free if an error occurs*/
                if (input_data == NULL) {
                        if(Table_length(sim_groups) == 0)
                                Table_free(&sim_groups);
                        else{
                                Table_map(sim_groups, error_free, NULL);
                                Table_map(sim_groups, free_values, NULL);
                                Table_free(&sim_groups);
                        }
                        fprintf(stderr, "simline: File not valid\n");
                        exit(EXIT_FAILURE);
                }

                /*loop to read a whole file*/
                while(!feof(input_data)) {

                        /*creates and initialises an atom with line from file*/
                        const char *string_atom;
                        string_atom = reader(input_data);
                        line_num++;

                        /*Checks if its an empty line (contains no word char)*/
                        if (string_atom == NULL)
                                continue;
                        else{
                                /*creates a value for an atom*/
                                List_T sim_list;
                                sim_list = Table_get(sim_groups, string_atom);
                                value *my_struct = malloc(sizeof(value));

                                /*Checks if a line already exists*/
                                if (sim_list == NULL) {
                                        my_struct->filename = argv[i];
                                        my_struct->line_num = line_num;
                                        sim_list = List_list(my_struct, NULL);
                                        Table_put(sim_groups, 
                                                string_atom, sim_list);
                                }
                                else{
                                        my_struct->filename = argv[i];
                                        my_struct->line_num = line_num;
                                        sim_list = List_push(sim_list, 
                                                my_struct);
                                        Table_put(sim_groups, 
                                                string_atom, sim_list);
                                }
                        }
                }
                fclose(input_data);
        }

        int num_matchGroups = 1;
        /*prints match groups*/
        Table_map(sim_groups, print_groups, &num_matchGroups);

        /*Memory management*/
        Table_map(sim_groups, free_values, NULL);
        Table_free(&sim_groups);
        return EXIT_SUCCESS;
}

/*
*Funtion: parse
*Parameters: a pointer to a line and a pointer to the line size
*/
char *parse(char *line, size_t *line_size) {
        char *temp_line = calloc(*line_size + 1, sizeof(char));
        if (temp_line == NULL) {
                fprintf(stderr, "simline: memory allocation failed\n");
                exit(EXIT_FAILURE);
        }
        int new_size = 0, space = 0, num_char = 0;
        size_t i;

        /*Gets rid of non-word characters*/
        for (i = 0; i < *line_size; i++) {
                if (!(iswordchar(line[i]) == 0)) {
                        space = 1;
                        temp_line[new_size] = line[i];
                        new_size++; 
                        num_char++;
                }
                else if(space == 1 && line[i] == ' ') {
                        temp_line[new_size] = ' ';
                        space = 0;
                        new_size++;
                }
                else
                        continue;
        }
        temp_line[new_size] = '\0';
        if (num_char == 0){
                free(temp_line);
                temp_line = NULL;
        }
        free(line);
        line = temp_line;
        *line_size = new_size + 1;
        return line;
}


/*
*Funtion: iswordchar
*Parameters: a char
*/
int iswordchar(char my_char) {
        if (my_char >= 65 && my_char <= 90)
                return 1;
        else if (my_char >= 97 && my_char <= 122) 
                return 1;
        else if (my_char >= 48 && my_char <= 57)
                return 1;
        else if (my_char == 95)
                return 1;
        else
                return 0;
}

/*
*Funtion: reader
*Parameters: a file pointer
*/
const char *reader(FILE *inputfd) {
        char *my_line = NULL;
        size_t line_size;
        line_size = readaline(inputfd, &my_line);
        my_line = parse(my_line, &line_size);

        if (my_line == NULL){
                return NULL;
        }
        else{
                /*creates an atom to be returned*/
                const char *test = Atom_string(my_line);
                free(my_line);
                return test;
        }
}

/*
*Funtion: print_groups
*Parameters: a pointer to a table key, a pointer to a pointer that points to 
*            the key value, a pointer to an additional parameter
*/ 
void print_groups(const void *key, void **value,void *cl) {
        if (List_length(*value) > 1) {
                if (*(int *)cl != 1)
                        printf("\n");
                else if (*(int *)cl == 1)
                        *(int *)cl = 0;

                printf("%s\n", (char *)key);
                *value = List_reverse(*value);

                /*Maps over list and prints values*/
                List_map(*value, print_list, NULL);
                /*Maps over list freeing values*/
                List_map(*value, free_list, NULL);
        }else {
                /*Maps over list freeing values*/
                List_map(*value, free_list, NULL);
        }
        (void)key;
        (void)cl;
}

/*
*Funtion: print_list
*Parameters: a pointer to a pointer to a char array, a pointer to an additional 
*            parameter
*/
void print_list(void **datapp,void *cl) {
        value *datap = *(value **)datapp;
        size_t string_size = strlen(datap->filename);

        /*Print according to format*/
        printf("%-20s %7d\n", datap->filename, datap->line_num);
        (void)string_size;
        (void)cl;
}

/*
*Funtion:
*Parameters:
*/
void free_values(const void *key, void **value,void *cl) {
        /*Free list*/
        List_free(*value);
        free(*value);
        (void)key;
        (void)cl;
}

/*
*Funtion: free_list
*Parameters: a pointer to a pointer to a char array, a pointer to an additional 
*            parameter
*/
void free_list(void **line, void *cl) {
      free(*line);
      (void)cl;
}

/*
*Funtion: error_free
*Parameters: a pointer to a table key, a pointer to a pointer that points to 
*            the key value, a pointer to an additional parameter
*/
void error_free(const void *key, void **value,void *cl) {
        List_map(*value, free_list, NULL);
        (void)key;
        (void)cl;
}