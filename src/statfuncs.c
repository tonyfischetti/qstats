#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>


double get_mean(double *data_array, int size){
    /* self-explanatory */
    double sum = 0;
    double mean;
    int i;
    for(i = 0; i < size; i++){
        sum += data_array[i];
    }
    mean = sum/size;
    return(mean);
}

void make_intervals(double themin, double themax, 
                    int breaks, double** delivery_array){
    /****************************************************
     * return an array with 'breaks'+1 equally spaced   *
     * numbers from min data point to max data point.   *
     * This will be used for the bucketed frequency     *
     * counts function. See the "deliver_frequencies"   *
     * function documentation for more information      *
     ****************************************************/
    double range = themax - themin;
    double *to_return_array;
    to_return_array = (double *) malloc((breaks+1) * sizeof(double));
    if(to_return_array == NULL){
        fputs("Error allocating memory", stderr);
        exit(EXIT_FAILURE);
    }
    int to_return_arrayindex = 0;
    double upto = themin;
    double thedivide = range/breaks-1;
    int i;
    for(i=0; i < breaks; i++){
        double newupto = i*thedivide+((i-1)*1)+themin+1;
        to_return_array[to_return_arrayindex] = newupto;
        to_return_arrayindex++;
        upto = newupto;
    }
    to_return_array[to_return_arrayindex] = themax+1;
    *delivery_array = to_return_array;
}


void deliver_frequencies(int size, double *data_array, int breaks, 
                          int** delivery_frequencies, 
                          double** delivery_intervals){
    /***************************************************************
     * This function returns an array of frequency counts for each *
     * data point in 'breaks' number of intervals. It takes: the   *
     * size of the data array, the data array by reference, the    *
     * number of equally sized 'buckets' to create, a pointer to   *
     * a pointer to store the frequency counts, and a pointer to   *
     * a pointer to store the intervals that are used to come      *
     * up with the frequency counts. The interval array will have  *
     * one more element than the frequency array because the       *
     * space between every value in the interval array and the     *
     * subsequent one constitute a "bucket". As such, the first    *
     * and last values of the interval array is the first and      *
     * the last value of the data array (after it's been sorted,   *
     * respectively.                                               *
     ***************************************************************/
    double *intervals;
    double themin = data_array[0];
    double themax = data_array[size-1];
    make_intervals(themin, themax, breaks, &intervals);
    *delivery_intervals = intervals;
    int *buckets;
    buckets = (int *) malloc(breaks * sizeof(int));
    if(buckets == NULL){
        fputs("Error allocating memory", stderr);
        exit(EXIT_FAILURE);
    }
    int j;
    for(j = 0; j < breaks; j++){
        buckets[j] = 0;
    }
    int place_in_bucket = 0;
    int the_bound = 1;
    int k;
    for(k = 0; k < size; k++){
        double num = data_array[k];
        if(num < intervals[the_bound]){
            buckets[place_in_bucket]++;
        }
        else{
            the_bound++;
            place_in_bucket++;
            buckets[place_in_bucket]++;
        }
    }
    intervals[breaks]--;
    *delivery_frequencies = buckets;
}


int get_uniques(double *data_array, int size, double** delivery_uniques){
    /*********************************************************
     * this function takes a sorted array by reference, it's *
     * size, and a pointer and the address where the new     *
     * array of only unique elements is to be stored         *
     *********************************************************/
    double last_value = data_array[0];
    /* have to assume that the array of unique elements *
     * is the same size as the original                 */
    double *uniques;
    uniques = (double *) malloc(size * sizeof(double));
    int new_size = 1;
    int old_index;
    int new_index = 1;
    uniques[0] = last_value;
    for(old_index=1; old_index < size; old_index++){
        if(!(fabs(last_value - data_array[old_index]) <= .0001)){
            uniques[new_index] = data_array[old_index];
            new_index++;
            new_size++;
            last_value = data_array[old_index];
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
    *delivery_uniques = uniques;
    return(new_size);
}


double *get_quartiles(double *data_array, int size){
    /********************************************
     * Takes a sorted data_array of doubles          *
     * and returns the first, second (median),  *
     * and third quartiles                      *
     ********************************************/
    static double ret_ar[2];
    double median;
    double first_quartile;
    double third_quartile;
    if(size % 2 == 1){
        int imid = floor(size/2);
        median = data_array[imid];
        if(imid % 2 == 1){
            int i1q = floor(imid/2);
            int i3q = imid + 1 + i1q;
            first_quartile = data_array[i1q];
            third_quartile = data_array[i3q];
        }
        else{
            int i1qb = imid/2 - 1;
            int i1qa = i1qb + 1;
            int i3qb = i1qb + 1 + imid;
            int i3qa = i3qb + 1;
            first_quartile = ((data_array[i1qb] + data_array[i1qa]) / 2);
            third_quartile = ((data_array[i3qb] + data_array[i3qa]) / 2);
        }
    }
    else{
        int imida = size/2;
        int imidb = imida-1;
        median = ((data_array[imidb] + data_array[imida]) / 2);
        if(imida % 2 == 0){
            int i1qb = imida/2 - 1;
            int i1qa = i1qb + 1;
            int i3qb = i1qb + imida;
            int i3qa = i3qb + 1;
            first_quartile = ((data_array[i1qb] + data_array[i1qa]) / 2);
            third_quartile = ((data_array[i3qb] + data_array[i3qa]) / 2);
        }
        else{
            int i1q = imidb / 2;
            int i3q = i1q + imida;
            first_quartile = data_array[i1q];
            third_quartile = data_array[i3q];
        }
    }
    ret_ar[0] = first_quartile;
    ret_ar[1] = median;
    ret_ar[2] = third_quartile;
    return(ret_ar);
}

