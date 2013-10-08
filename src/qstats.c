#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "statfuncs.h"
#include "infuncs.h"
#include "graphfuncs.h"


const char *header_text = 
    "\nqstats v0.4.0 -- quick and dirty statistics tool for the "
    "Unix pipeline\n";

const char *usage_text =
    "\nusage: qstats [-mshl | -f<breaks> | -b<breaks>] file\n";


static int FROM_FILE = false;
static int MEAN_FLAG = false;
static int SUMMARY_FLAG = false;
static int MEAN_SPECIFIED = false;
static int SUMMARY_SPECIFIED = false;
static int LENGTH_SPECIFIED = false;
static int LENGTH_FLAG = false;
static int FREQ_SPECIFIED = false;
static int FREQ_FLAG = false;
static int FREQ_BREAKS;
static int BARS_SPECIFIED = false;


int comp_func(const void * a, const void * b) {
    /*******************************************************
     * sorting function to be used for qsorting data array *
     *******************************************************/
    double *x = (double *) a;
    double *y = (double *) b;
    if (*x < *y){
        return -1;
    }
    else if (*x > *y) return 1; return 0;
}


int process_call(FILE* input){
    double *data_array;
    int size;

    size = read_column(&data_array, input);

    if(MEAN_SPECIFIED){
        MEAN_FLAG = true;
    }
    if(SUMMARY_SPECIFIED){
        SUMMARY_FLAG = true;
    } 
    if(LENGTH_SPECIFIED){
        LENGTH_FLAG = true;
    }
    if(FREQ_SPECIFIED){
        FREQ_FLAG = true;
    }
    if((FREQ_SPECIFIED + LENGTH_SPECIFIED + 
        SUMMARY_SPECIFIED + MEAN_SPECIFIED) == 0){
        /* summary is default */
        SUMMARY_FLAG = true;
    }

    /* only sort if needed */
    if((SUMMARY_FLAG + FREQ_FLAG) > 0){
        qsort(data_array, size, sizeof(double), comp_func);
    }

    if(MEAN_FLAG){
        double mean = get_mean(data_array, size);
        printf("%g\n", mean);
    }

    if(LENGTH_FLAG){
        printf("%d\n", size);
    }

    if(FREQ_FLAG){
        int breaks;
        /* if break number not specified, use sturge's rule */
        if(FREQ_BREAKS){
            breaks = FREQ_BREAKS;
        }
        else{
            breaks = ceil(log(size)) + 1;
        }
        int *buckets;
        double *intervals;
        deliver_frequencies(size, data_array, breaks, &buckets, &intervals);
        if(BARS_SPECIFIED == true){
            draw_bars(buckets, breaks);
        }
        else{
            /* first we have to find the max length string
             * in order to format properly */
            int m;
            int max_len = 0;
            for(m = 0; m < breaks; m++){
                int len;
                char line[30];
                len = sprintf(line, "[%.01f - %.01f):", intervals[m], 
                                                        intervals[m+1]);
                if(len > max_len){
                    max_len = len;
                }
            } 
            int n;
            for(n = 0; n < breaks; n++){
                int n2;
                char line[30];
                n2 = sprintf(line, "[%.01f - %.01f):", intervals[n], 
                                                       intervals[n+1]);
                printf("%*s %d\n", max_len, line, buckets[n]);
            }
        }
    }

    if(SUMMARY_FLAG){
        double *quartile_call_result;
        double mean = get_mean(data_array, size);
        /* if the size is less than five, no meaningful
           summary can be made */
        if(size < 5){
            fputs("Input too small for meaningful summary\n", stderr);
            exit(EXIT_FAILURE);
        }
        double the_min = data_array[0];
        double the_max = data_array[size-1];
        quartile_call_result = get_quartiles(data_array, size);
        double first_quartile = quartile_call_result[0];
        double median = quartile_call_result[1];
        double third_quartile = quartile_call_result[2];
        double stddev = get_standard_deviation(data_array, mean, size);
        printf("Min.     %g\n", the_min);
        printf("1st Qu.  %g\n", first_quartile);
        printf("Median   %g\n", median);
        printf("Mean     %g\n", mean);
        printf("3rd Qu.  %g\n", third_quartile);
        printf("Max.     %g\n", the_max);
        printf("Range    %g\n", (the_max - the_min));
        printf("Std Dev. %g\n", stddev);
        printf("Length   %d\n", size);
    }

    return EXIT_SUCCESS;
}


int main(int argc, char **argv){
    char *filename;
    FILE *input;
    int c;
    static int MULTIPLE_FILES = false;
   
    /* process command-line arguments */ 
    while(true){
        static struct option long_options[] = 
        {
            {"mean",    no_argument, 0, 'm'},
            {"summary", no_argument, 0, 's'},
            {"frequencies", optional_argument, 0, 'f'},
            {"bars", optional_argument, 0, 'b'},
            {"help",    no_argument, 0, 'h'},
            {"length",  no_argument, 0, 'l'},
            {0, 0, 0, 0}
        };

        int option_index = 0;

        c = getopt_long(argc, argv, "smhlf::b::", 
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
                if(optarg == NULL){
                    FREQ_BREAKS = 0;
                }
                else{
                    char *res;
                    FREQ_BREAKS = strtol(optarg, &res, 10);
                    if(*res){
                        fputs("Can't parse frequency breaks, expects integer\n",
                              stderr);
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            case 'b':
                FREQ_SPECIFIED = true;
                BARS_SPECIFIED = true;
                if(optarg == NULL){
                    FREQ_BREAKS = 0;
                }
                else{
                    char *res2;
                    FREQ_BREAKS = strtol(optarg, &res2, 10);
                    if(*res2){
                        fputs("Can't parse barchart breaks, expects integer\n",
                              stderr);
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            case 'h':
                printf("%s", header_text);
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

    /* check if filenames are specified */
    if(optind < argc){
        FROM_FILE = true;
        if(optind+1 < argc){
            MULTIPLE_FILES = true;
        }
        do{ 
            filename = argv[optind];
            input = fopen(filename, "r");
            if (NULL == input) {
                fprintf(stderr, "Unable to open '%s': %s\n",
                                 filename, strerror(errno));
                exit(EXIT_FAILURE);
            }
            if(MULTIPLE_FILES){
                printf("%s\n", filename);
            }
            process_call(input);
            if(MULTIPLE_FILES && optind+1 != argc){
                printf("\n");
            }
        } while (++optind < argc);
        return EXIT_SUCCESS;
    }

    /* if not, read once from stdin */
    process_call(NULL);
    return EXIT_SUCCESS;

}
