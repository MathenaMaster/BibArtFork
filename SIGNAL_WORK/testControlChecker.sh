#!/bin/bash

theNo='^This is the: [0-9]\+ over: [0-9]\+.$'
childcrtd='^Child created with pid: [0-9]\+ for: [0-9]\+ seconds.$'
thePid='^The pid: [0-9]\+ ended with status: [0-9]\+.$'
childEnd='^Child ended after: [0-9]\+ seconds.$'

if [ ! $# ]; then
    cat out.txt | tail -n+4 | grep -G -v -e "$theNo" -e "$childcrtd" -e "$thePid" -e "$childEnd"
else
    for outfile in $@; do
        cat $outfile | tail -n+4 | grep -G -v -e "$theNo" -e "$childcrtd" -e "$thePid" -e "$childEnd"
    done
fi
