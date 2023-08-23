#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

typedef struct s_philo
{
	int p_count;
	int *sayi;
	pthread_mutex_t *fork;
	pthread_t *philo_id;
}				t_philo;


#endif