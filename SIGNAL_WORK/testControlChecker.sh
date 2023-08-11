#!/bin/bash

theNo='^This is the: [0-9]\+ over: [0-9]\+.$'
childcrtd='^Child created with pid: [0-9]\+ for: [0-9]\+ seconds.$'
thePid='^The pid: [0-9]\+ ended with status: [0-9]\+$'
childEnd='^Child ended after: [0-9]\+ seconds.$'
theres='^There is actually: [0-9]\+ left in processing$'

if [ ! $# ]; then
    cat out.txt | tail -n+5 | grep -a -x -G -v -e "$theNo" -e "$childcrtd" -e "$thePid" -e "$childEnd" -e "$theres"
else
    for outfile in $@; do
        cat $outfile | tail -n+5 | grep -a -G -v -e "$theNo" -e "$childcrtd" -e "$thePid" -e "$childEnd" -e "$theres"
    done
fi
