#!/bin/bash

theNo='^This is the: [0-9]\+ over: [0-9]\+.$'

cat out.txt | grep -G -e "$theNo" | tail -n 20
