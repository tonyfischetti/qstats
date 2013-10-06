#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include "statfuncs.h"
#include "infuncs.h"
#include "graphfuncs.h"


const char *usage_text = 
    "\nqstats v0.3 -- quick and dirty statistics tool for the "
    "Unix pipeline\n"
    "To use this, pipe or redirect a newline-delimited column of "
    "numerical values to this program."
    "\nusage: qstats [-amshl | -f breaks | -h breaks] < *stream*\n";


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
    static int SUMMARY_FLAG = false;
    static int MEAN_SPECIFIED = false;
    static int SUMMARY_SPECIFIED = false;
    static int ALL_SPECIFIED = false;
    static int LENGTH_SPECIFIED = false;
    static int LENGTH_FLAG = false;
    static int FREQ_SPECIFIED = false;
    static int FREQ_FLAG = false;
    static int FREQ_BREAKS;
    static int BARS_SPECIFIED = false;
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
            {"frequencies", required_argument, 0, 'f'},
            {"bars", required_argument, 0, 'b'},
            {"help",    no_argument, 0, 'h'},
            {"length",  no_argument, 0, 'l'},
            {0, 0, 0, 0}
        };

        int option_index = 0;

        c = getopt_long(argc, argv, "asmhlf:b:", 
                        long_options, &option_index);

        if(c == -1)
            break;

        switch(c){
            case 0:
                if (long_options[option_index].flag != 0)
                    break;
                printf ("option %s", long_options[option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;
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
            case 'f':
                FREQ_SPECIFIED = true;
                char *res;
                FREQ_BREAKS = strtol(optarg, &res, 10);
                if(*res){
                    fputs("Can't parse frequency breaks, expects integer\n",
                          stderr);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'b':
                FREQ_SPECIFIED = true;
                BARS_SPECIFIED = true;
                char *res2;
                FREQ_BREAKS = strtol(optarg, &res2, 10);
                if(*res2){
                    fputs("Can't parse barchart breaks, expects integer\n",
                          stderr);
                    exit(EXIT_FAILURE);
                }
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
        SUMMARY_SPECIFIED = true;
        LENGTH_SPECIFIED = true;
        FREQ_SPECIFIED = true;
        MEAN_SPECIFIED = true;
    }
    if(MEAN_SPECIFIED == true){
        MEAN_FLAG = true;
    }
    if(SUMMARY_SPECIFIED == true){
        MEAN_FLAG = false;
        SUMMARY_FLAG = true;
    } 
    if(LENGTH_SPECIFIED == true){
        LENGTH_FLAG = true;
    }
    if(FREQ_SPECIFIED == true){
        FREQ_FLAG = true;
    }
    if((FREQ_SPECIFIED + LENGTH_SPECIFIED + 
        SUMMARY_SPECIFIED + MEAN_SPECIFIED) == 0){
        /* summary is default */
        SUMMARY_FLAG = true;
    }


    size = read_column(&array);

    /* only sort if needed */
    if((SUMMARY_FLAG + FREQ_FLAG) > 0){
        qsort(array, size, sizeof(double), comp_func);
    }

    if(MEAN_FLAG == true){
        mean = get_mean(array, size);
        printf("%g\n", mean);
    }

    if(LENGTH_FLAG == true){
        printf("\nLength: \5%d\n\n", size);
    }

    if(FREQ_FLAG == true){
        int breaks = FREQ_BREAKS;
        int *buckets;
        double *intervals;
        ret_buckets(size, array, breaks, &buckets, &intervals);
        if(BARS_SPECIFIED == true){
            draw_bars(buckets, breaks);
        }
        else{
            int n;
            for(n = 0; n < breaks; n++){
                printf("bucket %d:\t%d\n", n+1, buckets[n]);
            }
            //puts("\nthe intervals");
            //int m;
            //for(m = 0; m < breaks+1; m++){
            //    printf("%g\n", intervals[m]);
            //}
        }
    }

    if(SUMMARY_FLAG == true){
        mean = get_mean(array, size);
        /* if the size is less than four, no meaningful
           summary can be made */
        if(size < 4){
            fputs("Input too small for meaningful summary\n", stderr);
            return EXIT_FAILURE;
        }
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
