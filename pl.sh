#!/bin/bash

loop=1

p_count=$1
t_die=$2
t_eat=$3
t_sleep=$4
must_eat=$5

gcc philosophers.c input.c printer.c check_die.c -o philo

if [ $? -ne 0 ]; then
	exit 1
fi

for ((i=0;i<$loop;i++))
do
	if [ $loop -gt 1 ]; then
		echo "Loop $(($i+1)):"
	fi
	valgrind --track-origins=yes ./philo $p_count $t_die $t_eat $t_sleep $must_eat
	if [ $loop -gt $(($i+1)) ]; then
		echo ""
	fi
done