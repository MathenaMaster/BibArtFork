#!/bin/bash

presentleft=2
min=2
while [ $presentleft -gt 0 ];
    do sleep 2
    presentleft=$(ps -aux | grep './bibArt' | wc -l)
    presentleft=$(($presentleft - $min))
    if [ $presentleft -le 0 ]; then presentleft=0; fi
    echo "$presentleft babies of biba alive!"
done
echo "No more"
