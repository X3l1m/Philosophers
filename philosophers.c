#include "philosophers.h"

#define P_NUM 5

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

void *philo_thread(void* arg)
{
	
	t_thread_data *data;
	data = (t_thread_data*)arg;
	int left;
	left = (data->id == 1) ? P_NUM : data->id - 1;
	pthread_mutex_lock(&data->shared->fork[P_NUM]);
	pthread_mutex_unlock(&data->shared->fork[P_NUM]);
	pthread_mutex_lock(&data->shared->fork[data->id - 1]);
	pthread_mutex_lock(&data->shared->fork[left - 1]);
	printf("%d:%ld\n", data->id, run_time(data->shared->start_time));
	usleep(100000);
	// printf("left: %d right: %d\n", left - 1, data->id - 1);
	pthread_mutex_unlock(&data->shared->fork[data->id - 1]);
	pthread_mutex_unlock(&data->shared->fork[left - 1]);
}


int main()
{
	t_shared_data	shared;
	t_thread_data	thread[P_NUM];
	int		i;

	i = 0;
	shared.fork = malloc(sizeof(pthread_mutex_t) * (P_NUM + 1));
	shared.thread_id = malloc(sizeof(pthread_t) * P_NUM);
	while (i <= P_NUM)
		pthread_mutex_init(&shared.fork[i++], NULL);
	i = 0;
	pthread_mutex_lock(&shared.fork[P_NUM]);
	while (i < P_NUM)
	{
		thread[i].shared = &shared;
		thread[i].id = i + 1;
		pthread_create(&shared.thread_id[i], NULL, philo_thread, &thread[i]);
		i++;
		//printf("id:%d  i:%d\n", shared.philo_id, i);
	}
	shared.start_time = gettime();
	pthread_mutex_unlock(&shared.fork[P_NUM]);
	i = 0;
	while (i < P_NUM)
		pthread_join(shared.thread_id[i++], NULL);
	i = 0;
	while (i < P_NUM)
		pthread_mutex_destroy(&shared.fork[i++]);
	free(shared.fork);
	free(shared.thread_id);
	return (0);
}
