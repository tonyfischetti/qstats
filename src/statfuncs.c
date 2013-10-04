#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

double get_mean(double *array, int size){
    double sum = 0;
    double mean;
    int i;
    for(i = 0; i < size; i++){
        sum += array[i];
    }
    mean = sum/size;
    return(mean);
}


int get_uniques(double *array, int size, double** runiques){
    /* this function takes a sorted array by reference, it's *
     * size, and a pointer and the address where the new     *
     * array of only unique elements is to be stored         */
    double last_value = array[0];
    /* have to assume that the array of unique elements *
     * is the same size as the original                 */
    double *uniques;
    uniques = (double *) malloc(size * sizeof(double));
    int new_size = 1;
    int old_index;
    int new_index = 1;
    uniques[0] = last_value;
    for(old_index=1; old_index < size; old_index++){
        if(!(fabs(last_value - array[old_index]) <= .0001)){
            uniques[new_index] = array[old_index];
            new_index++;
            new_size++;
            last_value = array[old_index];
        }
    }
    /* resize array */
    double *temp;
    temp = realloc(uniques, new_size * sizeof(double));
    if(temp == NULL){
        free(uniques);
        fputs("Error allocating memory", stderr);
        exit(EXIT_FAILURE);
    }
    /* re-allocation successful */
    uniques = temp;
    *runiques = uniques;
    return(new_size);
}


void get_simple_frequencies(double *bigarray, int bigsize, int unisize, 
                             int** rfreqs){
    int *frequencies;
    frequencies = (int *) malloc(unisize * sizeof(int));
    double last_value = bigarray[0];
    frequencies[0] = 1;
    int old_index;
    int new_index = 0;
    for(old_index = 1; old_index < bigsize; old_index++){
        double current_value = bigarray[old_index];
        if(current_value == last_value){
            frequencies[new_index] = frequencies[new_index] + 1;
        }
        else{
            last_value = current_value;
            new_index++;
            frequencies[new_index] = 1;
        }
    }
    *rfreqs = frequencies;
    return;
}

double *get_quartiles(double *array, int size){
    /********************************************
     * Takes a sorted array of doubles          *
     * and returns the first, second (median),  *
     * and third quartiles                      *
     ********************************************/
    static double ret_ar[2];
    double median;
    double first_quartile;
    double third_quartile;
    if(size % 2 == 1){
        int imid = floor(size/2);
        median = array[imid];
        if(imid % 2 == 1){
            int i1q = floor(imid/2);
            int i3q = imid + 1 + i1q;
            first_quartile = array[i1q];
            third_quartile = array[i3q];
        }
        else{
            int i1qb = imid/2 - 1;
            int i1qa = i1qb + 1;
            int i3qb = i1qb + 1 + imid;
            int i3qa = i3qb + 1;
            first_quartile = ((array[i1qb] + array[i1qa]) / 2);
            third_quartile = ((array[i3qb] + array[i3qa]) / 2);
        }
    }
    else{
        int imida = size/2;
        int imidb = imida-1;
        median = ((array[imidb] + array[imida]) / 2);
        if(imida % 2 == 0){
            int i1qb = imida/2 - 1;
            int i1qa = i1qb + 1;
            int i3qb = i1qb + imida;
            int i3qa = i3qb + 1;
            first_quartile = ((array[i1qb] + array[i1qa]) / 2);
            third_quartile = ((array[i3qb] + array[i3qa]) / 2);
        }
        else{
            int i1q = imidb / 2;
            int i3q = i1q + imida;
            first_quartile = array[i1q];
            third_quartile = array[i3q];
        }
    }
    ret_ar[0] = first_quartile;
    ret_ar[1] = median;
    ret_ar[2] = third_quartile;
    return(ret_ar);
}

