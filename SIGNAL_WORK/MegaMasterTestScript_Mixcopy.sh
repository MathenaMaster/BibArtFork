#!/bin/bash

entry_regex='^[0-9]+$'
if [[ $# -ne 4 ]]; then
    echo "Please specify the out dir name as first argument"
    echo "A second for the process binary name"
    echo "The third is the number of $2 processes loop to launch"
    echo "the fourth is the number of fork to launch in these"
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

echo "Launching $3 tests with each: $4 forks"

for i in $(seq 0 $(($3 - 1)))
    do
    echo "Launching test number $i"
    ($(./$2 $4 > "$1/mix_out_file_$i.out" 2> "$1/mix_err_file_$i.err") && echo "Test number $i OK") &
    #if [ $i -eq 1 ]
    #then
        #echo "$i test launched"
    #else
        #echo "$i tests launched"
    #fi
    #&& echo "Test number $i OK") &
    #./$2 $4 > $5 2> $6 &
    done

echo "Awaiting all bibArts ending"


#if [ $i -eq 1 ]
    #then
        #echo "$i test launched"
    #else
        #echo "$i tests launched"
    #fi

#while [ $(ps | grep bibArt | wc -l) -gt 3 ]
    #do sleep 1
    #done
#ps


save=0
left=$(ps | grep bibArt | wc -l)
while [[ $left -gt 1 ]]
    do
        save=$left
        left=$(ps | grep bibArt | wc -l)
        if [[ $save -eq $left && $save -le 3 ]]
        then
            echo "Attention script seems to not end because of $left bibArts"
        fi
        sleep 1
    done

echo "Process endend"

ps
