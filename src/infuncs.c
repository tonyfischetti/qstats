#include <stdio.h>
#include <stdlib.h>

#define CHUNK 300

int read_column(double** rarray){
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
    double *barray;

    barray = (double *) malloc(size * sizeof(double));
    if(barray == NULL){
        fputs("Error allocating memory", stderr);
        exit(EXIT_FAILURE);
    }

    while(fgets(line, sizeof(line), stdin) != NULL){
        int ret_val;
        ret_val = sscanf(line, "%lf", &current);
        if(ret_val == 0){
            fputs("Error parsing numerics\n", stderr);
            exit(EXIT_FAILURE);
        }
        barray[index] = current;
        index++;
        /* if we ran out of space */
        if(index == size){
            /* try to allocate more memory */
            double *temp;
            size += CHUNK;
            temp = realloc(barray, size * sizeof(double));
            if(temp == NULL){
                free(barray);
                fputs("Error allocating memory", stderr);
                exit(EXIT_FAILURE);
            }
            /* reallocation successful */
            barray = temp;
        }
    }
    size = index;

    /* resize to not waste memory */
    double *temp;
    temp = realloc(barray, size * sizeof(double));
    if(temp == NULL){
        free(barray);
        fputs("Error allocating memory", stderr);
        exit(EXIT_FAILURE);
    }
    barray = temp;
    *rarray = barray;
    return(size);
}

