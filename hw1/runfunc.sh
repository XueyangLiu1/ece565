#!/bin/bash
g++ -O3 -o func_lining func_lining.cpp
for (( counter=10; counter>0; counter-- ))
do
./func_lining 100000000
done
printf "\n"