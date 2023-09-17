#include "philosophers.h"

size_t	gettime(void)
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return(time.tv_sec * 1000 + (time.tv_usec / 1000));
}

size_t	run_time(size_t start_time)
{
	return(gettime() - start_time);
}

void	s_sleep(long time)
{
	long	start;

	start = gettime();
	while (gettime() - start < time)
		usleep(250);
}

int	check_die(t_thread_data *data)
{
	int	diff;
	
	diff = gettime() - data->l_eat;
	if (diff > data->shared->t_die)
		return (1);
	return (0);
}


void	print(t_thread_data *data, size_t time, char *str)
{
	pthread_mutex_lock(&data->shared->fork[data->shared->p_count]); 
	printf("%ld %d %s\n", run_time(data->shared->start_time), data->id, str);
	pthread_mutex_unlock(&data->shared->fork[data->shared->p_count]);
}


void *philo_thread(void* arg)
{
	
	t_thread_data *data;
	data = (t_thread_data*)arg;
	int left;
	left = (data->id == 1) ? data->shared->p_count : data->id - 1;
	pthread_mutex_lock(&data->shared->fork[data->shared->p_count]);
	pthread_mutex_unlock(&data->shared->fork[data->shared->p_count]);
	if (!data->id % 2)
		s_sleep(data->shared->t_eat / 2);
	data->l_eat = data->shared->start_time;
	while (1)
	{
		if(data->shared->die)
			break;
		pthread_mutex_lock(&data->shared->fork[data->id - 1]);
		print(data, run_time(data->shared->start_time), "is taken a fork");
		pthread_mutex_lock(&data->shared->fork[left - 1]);
		print(data, run_time(data->shared->start_time), "is taken a fork");
		print(data, run_time(data->shared->start_time), "is eating");
		s_sleep(data->shared->t_eat);
		data->l_eat = gettime();
		// printf("left: %d right: %d\n", left - 1, data->id - 1);
		pthread_mutex_unlock(&data->shared->fork[data->id - 1]);
		pthread_mutex_unlock(&data->shared->fork[left - 1]);
		print(data, run_time(data->shared->start_time), "is sleeping");
		s_sleep(data->shared->t_sleep);
		print(data, run_time(data->shared->start_time), "is thinking");
	}
}


int main(int argc, char **argv)
{
	int				i;
	pthread_t		print_t;
	t_shared_data	shared;
	t_thread_data	*thread;

/* 	shared.p_count = 5;
	shared.t_die = 1000;
	shared.t_eat = 300;
	shared.t_sleep = 300; */

	if (argc != 5 && argc != 6)
		return(printf("Wrong input\n"));

	if(!input_init(&shared, argv, argc))
		return (printf("Invalid Number\n"));

	shared.fork = malloc(sizeof(pthread_mutex_t) * (shared.p_count + 1));
	shared.thread_id = malloc(sizeof(pthread_t) * shared.p_count);
	thread = malloc(sizeof(t_thread_data) * shared.p_count);


	i = 0;
	while (i <= shared.p_count)
		pthread_mutex_init(&shared.fork[i++], NULL);
	i = 0;
	pthread_mutex_lock(&shared.fork[shared.p_count]);
	while (i < shared.p_count)
	{
		thread[i].shared = &shared;
		thread[i].id = i + 1;
		pthread_create(&shared.thread_id[i], NULL, philo_thread, &thread[i]);
		i++;
	}
	pthread_create(&print_t, NULL, printer, &shared);
	shared.start_time = gettime();
	pthread_mutex_unlock(&shared.fork[shared.p_count]);
	i = 0;
	while (i < shared.p_count)
		pthread_join(shared.thread_id[i++], NULL);
	i = 0;
	while (i < shared.p_count)
		pthread_mutex_destroy(&shared.fork[i++]);
	free(shared.fork);
	free(shared.thread_id);
	free(thread);
	return (0);
}
