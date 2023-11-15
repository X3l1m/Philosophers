#include <philosophers.h>

void	philo_join(t_shr_data *shared, int count)
{
	while (--count > -1)
		pthread_join(shared->thread_id[count], NULL);
}

int	philo_create(t_shr_data *s)
{
	int	i;

	i = -1;
	pthread_mutex_lock(&s->fork[s->p_count]);
	while (++i < s->p_count)
	{
		s->thr[i].shr = s;
		s->thr[i].id = i;
		s->thr[i].p_head = NULL;
		if (pthread_create(&s->thread_id[i], NULL, philo_thread, &s->thr[i]))
			return (i + 1);
	}
	wait(s, 0);
	if (pthread_create(&s->print_t, NULL, printer, s))
		return (s->p_count + 1);
	s->start_time = gettime();
	pthread_mutex_unlock(&s->fork[s->p_count]);
	return (0);
}

int	mutex_create(t_shr_data *shared)
{
	int	i;

	i = -1;
	while (++i < shared->p_count)
	{
		if (pthread_mutex_init(&shared->fork[i], NULL))
			return (1);
		if (pthread_mutex_init(&shared->thr[i].p_lock, NULL))
			return (1);
		if (pthread_mutex_init(&shared->thr[i].e_lock, NULL))
			return (1);
	}
	if (pthread_mutex_init(&shared->fork[i], NULL))
		return (1);
	return (0);
}

int	make_malloc(t_shr_data *shared)
{
	int	i;

	i = -1;
	shared->thr = malloc(sizeof(t_thr_data) * shared->p_count);
	if (!shared->thr)
		return (0);
	shared->fork = malloc(sizeof(pthread_mutex_t) * (shared->p_count + 1));
	if (!shared->fork)
		return (1);
	shared->thread_id = malloc(sizeof(pthread_t) * shared->p_count);
	if (!shared->thread_id)
		return (2);
	while (shared->p_count > ++i)
		shared->thr[i].l_eat = 0;
	return (-1);
}

int	main(int argc, char **argv)
{
	int			err;
	t_shr_data	shared;

	if ((argc != 5 && argc != 6) || !input_init(&shared, argv, argc))
		return (printf("Wrong input\n"));
	if (shared.p_count == 1)
		return (one_philo(&shared));
	if (free_all(&shared, make_malloc(&shared), 0) >= 0)
		return (printf("!!!make malloc failed!!!\n"));
	if (mutex_create(&shared))
		return (free_all(&shared, 3, 1));
	err = philo_create(&shared);
	if (err)
	{
		shared.end = 2;
		pthread_mutex_unlock(&shared.fork[shared.p_count]);
		philo_join(&shared, err - 1);
		printf("Phillo error at %d\n", err - 1);
		return (free_all(&shared, 3, 1));
	}
	check_die(&shared);
	pthread_join(shared.print_t, NULL);
	philo_join(&shared, shared.p_count);
	free_all(&shared, 4, 1);
	return (0);
}
