#include "philosophers.h"

size_t	gettime(void)
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

size_t	run_time(size_t start_time)
{
	return(gettime() - start_time);
}

void	s_sleep(size_t time)
{
	size_t	start;

	start = gettime();
	while ((gettime()) - start < time)
		usleep(250);
}

t_print	*new_node(int id, size_t time, char *str)
{
	t_print *new;
	new = malloc(sizeof(t_print));
	if (!new)
	{
		printf("Malloc fail!\n %s\n", str);
		return (NULL);
	}
	new->ms = time;
	new->msg = str;
	new->next = NULL;
	new->id = id;
	return (new);
}

int	add_node(t_thr_data *thread, size_t time, char *str)
{
	if (!thread->p_head)
	{
		thread->p_head = new_node(thread->id, time, str);
		if (!thread->p_head)
			return (1);
		thread->p_last = thread->p_head;
	}
	else
	{
		thread->p_last->next = new_node(thread->id, time, str);
		if (!thread->p_last->next)
			return (1);
		thread->p_last = thread->p_last->next;
	}
	return (0);
}

int	print(t_thr_data *thread, size_t time, char *str)
{
	if (thread->shr->end)
		return (0);
	pthread_mutex_lock(&thread->p_lock);
	if(add_node(thread, time, str))
	{
		pthread_mutex_unlock(&thread->p_lock);
		return (1);
	}
	pthread_mutex_unlock(&thread->p_lock);
	return (0);
}

int leave_fork(t_thr_data *thread, char right)
{
	pthread_mutex_unlock(&thread->shr->fork[thread->id]);
	if (right)
		pthread_mutex_unlock(&thread->shr->fork[thread->right]);
}

int take_forks(t_thr_data *thread)
{
	pthread_mutex_lock(&thread->shr->fork[thread->id]);
	if(print(thread, run_time(thread->shr->start_time), "is taken a fork"))
	{
		leave_fork(thread, 0);
		return (1);
	}
	pthread_mutex_lock(&thread->shr->fork[thread->right]);
	if(print(thread, run_time(thread->shr->start_time), "is taken a fork"))
	{
		leave_fork(thread, 1);
		return (1);
	}
	return (0);
}

void	philo_loop(t_thr_data *thread)
{
	while (!thread->shr->end)
	{
		if(take_forks(thread))
			return ;
		pthread_mutex_lock(&thread->e_lock);
		thread->l_eat = gettime();
		pthread_mutex_unlock(&thread->e_lock);
		if (print(thread, run_time(thread->shr->start_time), "is eating"))
			{
				leave_fork(thread, 1);
				return ;
			}
		s_sleep(thread->shr->t_eat);
		leave_fork(thread, 1);
		if (print(thread, run_time(thread->shr->start_time), "is sleeping"))
			return ;
		s_sleep(thread->shr->t_sleep);
		if (print(thread, run_time(thread->shr->start_time), "is thinking"))
			return ;
	}
}

void *philo_thread(void* arg)
{
	t_thr_data	*thread;

	thread = (t_thr_data*)arg;
	thread->right = (thread->id + 1) % thread->shr->p_count;
	thread->l_eat = 1;
	thread->eated = 0;
	pthread_mutex_lock(&thread->shr->fork[thread->shr->p_count]);
	pthread_mutex_unlock(&thread->shr->fork[thread->shr->p_count]);
	thread->l_eat = thread->shr->start_time;
	if (thread->id % 2)
		s_sleep(thread->shr->t_eat / 2);
	philo_loop(thread);
	thread->shr->end = 1;
	pthread_exit(NULL);
}


int	free_all(t_shr_data *shared, int step)
{
	int		i;
	t_print	*tmp;

	if (step >= 4)
	{
		i = -1;
		while (shared->p_count > ++i)
		{
			while (shared->thr[i].p_head)
				free_node(&shared->thr[i].p_head);
		}
	}
	if (step >= 1)
		free(shared->thr);
	if (step >= 2)
		free(shared->fork);
	if (step >= 3)
		free(shared->thread_id);
	return (step);
}

int	make_malloc(t_shr_data *shared)
{
	shared->thr = malloc(sizeof(t_thr_data) * shared->p_count);
	if (!shared->thr)
		return (0);
	shared->fork = malloc(sizeof(pthread_mutex_t) * (shared->p_count + 1));
	if (!shared->fork)
		return (1);
	shared->thread_id = malloc(sizeof(pthread_t) * shared->p_count);
	if (!shared->thread_id)
		return (2);
	return (-1);
}

int	philo_create(t_shr_data *shared)
{
	int	i;

	i = -1;
	pthread_mutex_lock(&shared->fork[shared->p_count]);
	while (++i < shared->p_count)
	{
		shared->thr[i].shr = shared;
		shared->thr[i].id = i;
		shared->thr[i].p_head = NULL;
		if (pthread_mutex_init(&shared->thr[i].p_lock, NULL)
			|| pthread_mutex_init(&shared->thr[i].e_lock, NULL)
			|| pthread_create(&shared->thread_id[i], NULL, philo_thread, &shared->thr[i]))
		{
			printf("Philo create failed!\n");
			return(free_all(shared, 3));
		}
	}
	i = -1;
	while (++i < shared->p_count)
		if (shared->thr[i].l_eat != 1)
			--i;
	shared->start_time = gettime();
	pthread_mutex_unlock(&shared->fork[shared->p_count]);
	return (0);
}

int main(int argc, char **argv)
{
	int				i;
	pthread_t		print_t;
	t_shr_data	shared;

	if (argc != 5 && argc != 6)
		return(printf("Wrong input!\n"));
	if(!input_init(&shared, argv, argc))
		return (printf("Invalid Number!\n"));
	if (free_all(&shared, make_malloc(&shared)) >= 0)
		return(printf("!!!Malloc fail!!!\n"));
	i = -1;
	while (++i <= shared.p_count)
	{
		if(pthread_mutex_init(&shared.fork[i], NULL))
			return(free_all(&shared, 0));
	} 
	i = -1;
	if (philo_create(&shared))
		return (1);
	if(pthread_create(&print_t, NULL, printer, &shared))
		return(free_all(&shared, 3));
	check_die(&shared);
	i = -1;
	while (++i < shared.p_count)
		pthread_join(shared.thread_id[i], NULL);
	pthread_join(print_t, NULL);
	i = -1;
	while (++i < shared.p_count)
		pthread_mutex_destroy(&shared.fork[i]);
	return (0);
}
