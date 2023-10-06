#!/bin/bash

loop=1

p_count=$1
t_die=$2
t_eat=$3
t_sleep=$4
must_eat=$5

gcc -Wall -Werror -Wextra main.c input_time.c printer.c printer_utils.c clean.c philo.c check_die.c -o philo

if [ $? -ne 0 ]; then
	exit 1
fi

for ((i=0;i<$loop;i++))
do
	if [ $loop -gt 1 ]; then
		echo "Loop $(($i+1)):"
	fi
	./philo $p_count $t_die $t_eat $t_sleep $must_eat
	if [ $loop -gt $(($i+1)) ]; then
		echo ""
	fi
done
