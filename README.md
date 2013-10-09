# qstats

Quick and dirty statistics for the Unix pipeline


### Introduction

I'm a big fan of using Unix tools in my data analysis workflow. The only
thing I seemed to be missing were a few simple tools for computing 
descriptive statistics from the command-line. A found myself redirecting
output to a file and then starting R just to retrieve simple summary stats.
So, I started this project to make a small command-line executable to
include in a Unix shell pipeline to help me and any other data analysts out!


### Example usage

This tool reads one column of data (separated by newline). A simple
usage of to compute mean would be:

    echo -e "5\n1\n3" | qstats -m

A more realistic example would be to subset a CSV by a condition, extract
one column (with awk), remove the header and grab summary statistics:

    grep "COND1" mycsv.csv | awk -F, '{print $2}' | tail +2 | qstats -s

If you need to get stats on, say, a comma separated text file of numerical
values that is not in column format, you can use tr (translation) to
substitute commas for newlines and pipe it to the program:

    cat afile | tr , '\n' | qstats

(I like to use unecessary "cat"s)


You can also call this tool with filenames as arguments, but it's utility
that way is limited because, for the time being, it can only read input
that is one column long of just numerics--and very few files are one
column long in nature.


### Installation

Read "INSTALL" file for more information

    $ make
    $ make install
