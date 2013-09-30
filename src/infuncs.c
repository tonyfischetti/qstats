#include <stdio.h>
#include <stdlib.h>

#define CHUNK 50

int read_column(double** array){
    double current;
    int index = 0;
    int size = 50;
    double *barray;

    barray = (double *) malloc(size * sizeof(double));
    if(array == NULL){
        fputs("Error allocating memory", stderr);
        exit(EXIT_FAILURE);
    }

    while(1){
        int ret_val;
        ret_val = scanf("%lf", &current);
        if(ret_val == EOF){
            break;
        }
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
            /* re-allocation successful */
            barray = temp;
        }
        size = index;
        *array = barray;
    }
    return(size);
}
