
/* Copyright (c) 2013, Tony Fischetti
 *
 * MIT License, http://www.opensource.org/licenses/mit-license.php
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 * 
 */

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
     * relative (rel_freqent) frequences and  *
     * displays it                            *
     ******************************************/
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int ncols = w.ws_col;
    /* wiggle room is the room to work with *
     * to draw the bars taking into account *
     * the rel_freqent of points in the buckets */
    int wiggle = ncols - 15;
    double rel_freq[size];

    int sum = 0;
    int i;
    for(i = 0; i < size; i++){
        sum += data_array[i];
    }
    
    /* find rel_freqent of data points in each bucket */
    for(i = 0; i < size; i++){
        int thenum = data_array[i];
        rel_freq[i] = ((double)thenum/(double)sum) * 100;
    }
    
    /* find the max value to determine the *
     * factor to to multiply all values    *
     * with to scale all bars in chart     */
    double themax = rel_freq[0];
    for(i = 0; i < size; i++){
        if(rel_freq[i] > themax){
            themax = rel_freq[i];
        }
    }

    double factor;
    factor = wiggle/themax;
    
    for(i = 0; i < size; i++){
        int thenum = rel_freq[i];
        printf("%0.1f%%\t", rel_freq[i]);
        int m;
        for(m = 0; m < floor(factor*thenum); m++){
            printf("#");
        }
        printf("\n");
    }
}
