#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK 300


int read_column(double** delivery_array, FILE *input){
    /***************************************
     * this is called by the main program  *
     * with a pointer to the array where   *
     * the data will be stored. It is safe *
     * from overflows and it dynamically   *
     * resizes.                            *
     ***************************************/
    double current;
    char line[50];
    int index = 0;
    int size = 50;
    double *build_array;

    if(!input){
        input = stdin;
    }

    build_array = (double *) malloc(size * sizeof(double));
    if(build_array == NULL){
        fputs("Error allocating memory", stderr);
        exit(EXIT_FAILURE);
    }

    while(fgets(line, sizeof(line), input) != NULL){
        if(!strcmp(line, "\n")){
            continue;
        }
        int ret_val;
        ret_val = sscanf(line, "%lf", &current);
        if(ret_val == 0){
            fputs("Error parsing numerics\n", stderr);
            exit(EXIT_FAILURE);
        }
        build_array[index] = current;
        index++;
        /* if we ran out of space */
        if(index == size){
            /* try to allocate more memory */
            double *temp;
            size += CHUNK;
            temp = realloc(build_array, size * sizeof(double));
            if(temp == NULL){
                free(build_array);
                fputs("Error allocating memory", stderr);
                exit(EXIT_FAILURE);
            }
            /* reallocation successful */
            build_array = temp;
        }
    }
    size = index;

    /* resize to not waste memory */
    double *temp;
    temp = realloc(build_array, size * sizeof(double));
    if(temp == NULL){
        free(build_array);
        fputs("Error allocating memory", stderr);
        exit(EXIT_FAILURE);
    }
    build_array = temp;
    *delivery_array = build_array;
    return(size);
}

