#!/bin/bash

OkNb=0

while true
    do echo "If the test blocks, please kill the last './bibArt' proces instance that you can get by typing in a shell 'make ps'"
    make execute
    let "++OkNb"
    echo "new succeded test: $OkNb realized actually"
    done

echo "$OkNb test finallly succeded at normal or forced ending."
