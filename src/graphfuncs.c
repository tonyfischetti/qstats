#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


void draw_bars(int *data_array, int size){
    /******************************************
     * draws a horizontal bar chart of the    *
     * frequencies of the data points in each *
     * bucket. It takes the array of data     *
     * point counts in each bucket, and the   *
     * size of the array; calculates the      *
     * percents and displayes it              *
     ******************************************/
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int ncols = w.ws_col;
    /* wiggle room is the room to work with *
     * to draw the bars taking into account *
     * the percent of points in the buckets */
    int wiggle = ncols - 15;
    double perc[size];

    int sum = 0;
    int i;
    for(i = 0; i < size; i++){
        sum += data_array[i];
    }
    
    /* find percent of data points in each bucket */
    for(i = 0; i < size; i++){
        int thenum = data_array[i];
        perc[i] = ((double)thenum/(double)sum) * 100;
    }
    
    /* find the max value to determine the *
     * factor to to multiply all values    *
     * with to scale all bars in chart     */
    double themax = perc[0];
    for(i = 0; i < size; i++){
        if(perc[i] > themax){
            themax = perc[i];
        }
    }

    double factor;
    factor = wiggle/themax;
    
    for(i = 0; i < size; i++){
        int thenum = perc[i];
        printf("%0.1f%%\t", perc[i]);
        int m;
        for(m = 0; m < floor(factor*thenum); m++){
            printf("#");
        }
        printf("\n");
    }
}
