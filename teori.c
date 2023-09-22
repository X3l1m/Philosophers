#include "philosophers.h"

#define P_NUM 5

size_t gettime(void)
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + (time.tv_usec / 1000));
}

void *philo_thread(void *arg)
{
	t_thr_data *data = (t_thr_data *)arg;
	int left = (data->id - 1) % P_NUM;
	int right = data->id % P_NUM;

		pthread_mutex_lock(&data->shr->fork[0]);
		//pthread_mutex_lock(&data->shared->fork[right]);
		printf("%d:%ld\n", data->id, gettime());
		pthread_mutex_unlock(&data->shr->fork[0]);
		//pthread_mutex_unlock(&data->shared->fork[right]);

}

int main()
{
	srand(time(NULL));

	t_shr_data shared;
	t_thr_data thread[P_NUM];
	int i;
	
	shared.fork = malloc(sizeof(pthread_mutex_t) * P_NUM);
	shared.thread_id = malloc(sizeof(pthread_t) * P_NUM);
	for (i = 0; i < P_NUM; i++)
		pthread_mutex_init(&shared.fork[i], NULL);

	for (i = 0; i < P_NUM; i++)
	{
		thread[i].shr = &shared;
		thread[i].id = i + 1;
		pthread_create(&shared.thread_id[i], NULL, philo_thread, &thread[i]);
	}

	for (i = 0; i < P_NUM; i++)
		pthread_join(shared.thread_id[i], NULL);

	for (i = 0; i < P_NUM; i++)
		pthread_mutex_destroy(&shared.fork[i]);

	return 0;
}
