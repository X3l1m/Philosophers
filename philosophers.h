#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

typedef struct s_shared_data
{
	int				p_count;
	int				eat_t;
	int				sleep_t;
	int				die_t;
	size_t			start_time;
	pthread_mutex_t	*fork;
	pthread_t		*thread_id;
}				t_shared_data;

typedef struct s_thread_data
{
	t_shared_data	*shared;
	int				id;
}				t_thread_data;

#endif