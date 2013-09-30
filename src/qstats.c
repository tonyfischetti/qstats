#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include "statfuncs.h"
#include "infuncs.h"


const char *usage_text = 
    "\nqstats v0.5 -- quick and dirty statistics tool for the "
    "UNIX pipeline\n"
    "To use this, pipe or redirect a newline-delimited column of "
    "numerical values to this program."
    "\nusage: qstats [-amshl] < *stream*\n";


int comp_func(const void * a, const void * b) {
    double *x = (double *) a;
    double *y = (double *) b;
    if (*x < *y){
        return -1;
    }
    else if (*x > *y) return 1; return 0;
}


int main(int argc, char **argv){

    static int MEAN_FLAG = false;
    static int SUMMARY_FLAG = true;
    static int MEAN_SPECIFIED = false;
    static int SUMMARY_SPECIFIED = false;
    static int ALL_SPECIFIED = false;
    static int LENGTH_SPECIFIED = false;
    static int LENGTH_FLAG = false;
    double *array;
    double *result;
    double the_min;
    double first_quartile;
    double median;
    double mean;
    double third_quartile;
    double the_max;
    int size;
    int c;
   
    /* process command-line arguments */ 
    while(1){
        static struct option long_options[] = 
        {
            {"all", no_argument, 0, 'a'},
            {"mean",    no_argument, 0, 'm'},
            {"summary", no_argument, 0, 's'},
            {"help",    no_argument, 0, 'h'},
            {"length",  no_argument, 0, 'l'},
            {0, 0, 0, 0}
        };

        int option_index = 0;

        c = getopt_long(argc, argv, "asmhl", long_options, &option_index);

        if(c == -1)
            break;

        switch(c){
            case 'a':
                ALL_SPECIFIED = true;
                break;
            case 'm':
                MEAN_SPECIFIED = true;
                break;
            case 's':
                SUMMARY_SPECIFIED = true;
                break;
            case 'l':
                LENGTH_SPECIFIED = true;
                break;
            case 'h':
                printf("%s\n", usage_text);
                freopen("/dev/null", "r", stdin);
                exit(EXIT_FAILURE);
            case '?':
                printf("%s\n", usage_text);
                freopen("/dev/null", "r", stdin);
                exit(EXIT_FAILURE);
            default:
                abort();
        }
    }

    if(ALL_SPECIFIED == true){
        SUMMARY_FLAG = true;
        LENGTH_FLAG = true;
        //MEAN_FLAG = true;
    }
    else if(MEAN_SPECIFIED == true){
        SUMMARY_FLAG = false;
        MEAN_FLAG = true;
        LENGTH_FLAG = false;
    }
    else if(SUMMARY_SPECIFIED == true){
        MEAN_FLAG = false;
        LENGTH_FLAG = false;
    }
    if(LENGTH_SPECIFIED == true){
        LENGTH_FLAG = true;
        if(SUMMARY_SPECIFIED == false){
            SUMMARY_FLAG = false;
        }
    }


    size = read_column(&array);

    if(MEAN_FLAG == true){
        mean = get_mean(array, size);
        printf("%g\n", mean);
    }

    if(LENGTH_FLAG == true){
        printf("\nLength: \5%d\n", size);
    }

    if(SUMMARY_FLAG == true){

        mean = get_mean(array, size);

        /* if the size is less than four, no meaningful
           summary can be made */
        if(size < 4){
            fputs("Input too small for meaningful summary\n", stderr);
            return EXIT_FAILURE;
        }

        qsort(array, size, sizeof(double), comp_func);
        the_min = array[0];
        the_max = array[size-1];
        result = get_quartiles(array, size);
        first_quartile = result[0];
        median = result[1];
        third_quartile = result[2];

        printf("\n");
        printf("Min.     %g\n", the_min);
        printf("1st Qu.  %g\n", first_quartile);
        printf("Median   %g\n", median);
        printf("Mean     %g\n", mean);
        printf("3rd Qu.  %g\n", third_quartile);
        printf("Max.     %g\n\n", the_max);
    }

    return EXIT_SUCCESS;


}
