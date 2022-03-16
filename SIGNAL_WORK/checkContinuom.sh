#!/bin/bash

presentleft=3
min=2
while [ $presentleft -gt 1 ];
    do sleep 2
    presentleft=$(ps -aux | grep './bibArt' | wc -l)
    presentleft=$(($presentleft - $min))
    if [ $presentleft -lt 0 ]; then presentleft=0; fi
    echo "$presentleft babies of biba alive!"
done
echo "No more"
