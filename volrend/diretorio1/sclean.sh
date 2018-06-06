#!/bin/bash

dir=$PWD
files=("p_semaphore" "sequential" "thread_mutex" "thread_spin"
"p_trans" "tbb" "thread_semaphore" "thread_trans")

for element in ${files[@]}
do
    path="progs"/$element
    cd $path
    make clean -f makefile
	cd -
done
