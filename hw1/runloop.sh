#!/bin/bash
for (( counter=10; counter>0; counter-- ))
do
./loop_optimizeO3 100000000
done
printf "\n"