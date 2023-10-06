#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

typedef struct s_print
{
	size_t			ms;
	int				id;
	char			*msg;
	struct s_print	*next;
}	t_print;

typedef struct s_shr_data
{
	int					p_count;
	int					t_die;
	int					t_eat;
	int					t_sleep;
	int					must_eat;
	int					end;
	size_t				start_time;
	pthread_t			print_t;
	pthread_t			*thread_id;
	pthread_mutex_t		*fork;
	struct s_thr_data	*thr;
}				t_shr_data;

typedef struct s_thr_data
{
	int					id;
	int					eated;
	int					right;
	size_t				l_eat;
	t_print				*p_head;
	t_print				*p_last;
	pthread_mutex_t		p_lock;
	pthread_mutex_t		e_lock;
	t_shr_data			*shr;
}				t_thr_data;

void	*philo_thread(void *arg);
int		char_to_int(char *str);
void	print_min(t_shr_data *s);
size_t	gettime(void);
void	s_sleep(size_t time);
size_t	run_time(size_t start_time);
int		input_init(t_shr_data *shared, char **argv, int argc);
void	*printer(void *arg);
int		print(t_thr_data *thread, size_t time, char *str);
void	check_die(t_shr_data *shared);
void	free_node(t_print **node);
int		free_all(t_shr_data *shared, int step, int mutex);
void	mutex_clean(t_shr_data *shared);

#endif