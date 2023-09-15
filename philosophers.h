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
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				must_eat;
	int				die;
	size_t			start_time;
	pthread_mutex_t	*fork;
	pthread_t		*thread_id;
}				t_shared_data;

typedef struct s_thread_data
{
	t_shared_data	*shared;
	int				id;
	int				l_eat;
}				t_thread_data;


int	input_init(t_shared_data *shared, char **argv, int argc);
int	char_to_int(char *str);

#endif

/* 5      1000  300    300      10
   count  t_die t_eat  t_sleep  must_eat */