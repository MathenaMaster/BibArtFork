#!/bin/bash

entry_regex='^[0-9]+$'
if [ $# -ne 6 ]; then
    echo "Please specify the out dir name as first argument"
    echo "A second for the process binary name"
    echo "The third is the number of processes loop to launch"
    echo "the fourth is the number of fork to launch in these iterations"
    exit
fi

if ! [[ $3 =~ $entry_regex ]]; then
    echo "Please specify a number as loop repetition entry"
    exit
fi

if ! [[ $4 =~ $entry_regex ]]; then
    echo "Please specify a number as fork number entry"
    exit
fi

for i in $(seq 1 $3)
    do #./$2 $4 > "$1/out_file_$i.out" 2> "$1/err_file_$i.err"
    ./$2 $4 > $5 2> $6
    if [ $i -eq 1 ]; then
        echo "$i test realized"
    else
        echo "$i tests realized"
    fi
    done
echo "$3 Multiple test charge launched with each: $4 fork launched"
