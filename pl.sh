#!/bin/bash

loop=1

for ((i=0;i<$loop;i++))
do
	if [ $loop -gt 1 ]; then
		echo "Loop $(($i+1)):"
	fi
	gcc philosophers.c -o philo && ./philo
	if [ $loop -gt $(($i+1)) ]; then
		echo ""
	fi
done