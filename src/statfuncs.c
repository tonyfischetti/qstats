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

// DEBUGGING
// TRACED ONE BUG TO INCORRECT COMPARISON OF FLOATING POINT NUMBERS
// THE OTHER BUG INVOLVES *EXTREMELY* LARGE NUMBERS
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
        printf("We are on %g\n", array[old_index]);
        printf("Last was %g\n", last_value);
        //if(last_value != array[old_index]){
        if(!(fabs(last_value - array[old_index]) <= .0001)){
            printf("It was not experienced before\n");
            uniques[new_index] = array[old_index];
            new_index++;
            new_size++;
            last_value = array[old_index];
        }
        else{
            printf("We've seen this before\n");
        }
        printf("\n");
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
    //uniques = temp;
    *runiques = uniques;

    //////// DEBUG ////////
    int i;
    for(i = 0; i < new_size; i++){
        printf("%g\n", uniques[i]);
    }
    //////// DEBUG ////////


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
    if(size % 2 == 0){
        int the_mid = size/2;
        median = (array[the_mid-1] + array[the_mid]) / 2;
        int p;
        double e_first_half[the_mid];
        for(p = 0; p < the_mid; p++){
            e_first_half[p] = array[p];
        }
        int q;
        double e_second_half[the_mid];
        for(q = the_mid; q < size; q++){
           e_second_half[q-the_mid] = array[q];
        }
        if(the_mid % 2 == 0){
            int new_mid = the_mid/2;
            first_quartile = (e_first_half[new_mid-1] + 
                              e_first_half[new_mid]) / 2;
            third_quartile = (e_second_half[new_mid-1] +
                              e_second_half[new_mid]) / 2;
        }
        else{
            int new_mid = floor(the_mid/2);
            first_quartile = e_first_half[new_mid];
            third_quartile = e_second_half[new_mid];
        }
    }
    else{
        int the_mid = floor(size/2);
        double first_half[the_mid-1];
        int j;
        for(j = 0; j < the_mid; j++){
            first_half[j] = array[j];
        }
        double second_half[the_mid-1];
        int g;
        for(g = the_mid; g < size-1; g++){
            second_half[g-the_mid] = array[g+1];
        }
        median = array[the_mid];
        if(the_mid % 2 == 0){
            int new_mid = the_mid/2;
            first_quartile = (first_half[new_mid-1] + 
                              first_half[new_mid]) / 2;
            third_quartile = (second_half[new_mid-1] +
                              second_half[new_mid]) / 2;
        }
        else{
            int new_mid = floor(the_mid/2);
            first_quartile = first_half[new_mid];
            third_quartile = second_half[new_mid];
        }
    }
    ret_ar[0] = first_quartile;
    ret_ar[1] = median;
    ret_ar[2] = third_quartile;
    return(ret_ar);
}



