# qstats

Quick and first statistics for the UNIX pipeline


### Introduction

I'm a big fan of using UNIX tools in my data analysis workflow. The only
thing I seemed to be missing were a few simple tools for computing 
descriptive statistics from the command-line. A found myself redirecting
output to a file and then starting R just to retrieve simple summary stats.
So, I started this project to make some small command-line executables to
include in a UNIX shell pipeline to help me and any other data analysts out!


### Example usage

All of these tools read one column of data (separated by newline). A simple
usage of to compute mean would be:

    echo -e "5\n1\3" | qstats -m

A more realistic example would be to subset a CSV by a condition, extract
one column (with awk), remove the header and grab summary statistics:

    grep "COND1" mycsv.csv | awk -F, '{print $2}' | tail +2 | qstats -s

If you need to get stats on, say, a comma separated text file of numerical
values that is not in column format, you can use tr (translation) to
substitute commas for newlines and pipe it to a cl* program:

    cat afile | tr , '\n' | qstats

(I like to use unecessary "cat"s)


### Installation

Read "INSTALL" file for more information

$ make
$ make install
