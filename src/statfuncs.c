#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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




