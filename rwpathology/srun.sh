#!/bin/bash

TIMEFORMAT=%R
dir=$PWD
EXECUTIONS=11
INP=2048
files=("p_semaphore" "p_trans" "thread_mutex" "thread_spin" "thread_semaphore" 
        "thread_trans" "tbb")

mkdir output

path="progs/sequential"
cd $path
for ((i=1;i<=${EXECUTIONS};i++)); do
	echo -e "\n--> sequential $i/${EXECUTIONS}\n\n"
	(time ./build/app/main -m ${INP}) 2>> ../../output/tempo_sequential.txt
done
cd -

for element in ${files[@]}; do
	path="progs"/$element
	cd $path
	for t in 2 4 8 16; do
		for ((i=1;i<=${EXECUTIONS};i++)); do
			if [ "$element" = "tbb" ]
        	then
            	if [ "$t" -le "4" ]
            	then
                	echo -e "\n--> $element ($t flows) $i/${EXECUTIONS}\n"
                	(time ./build/app/main -m ${INP} -p $t) 2>> ../../output/tempo_${element}_${t}_threads.txt
            	fi
        	else
            	echo -e "\n--> $element ($t flows) $i/${EXECUTIONS}\n"
            	(time ./build/app/main -m ${INP} -p $t) 2>> ../../output/tempo_${element}_${t}_threads.txt
        	fi
		done
    done
	cd -
done

