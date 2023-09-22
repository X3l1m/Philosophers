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

/* int	check_die(t_thr_data *thread)
{
	int	diff;
	
	diff = gettime() - thread->l_eat;
	if (diff > thread->shr->t_die)
		return (1);
	return (0);
} */

t_print	*new_node(int id, size_t time, char *str)
{
	t_print *new;
	new = malloc(sizeof(t_print));
	if (!new)
		exit(0); //malloc error
	new->ms = time;
	new->msg = str;
	new->next = NULL;
	new->id = id;
	return (new);
}

void 	add_node(t_thr_data *thread, size_t time, char *str)
{
	if (!thread->p_head)
	{
		thread->p_head = new_node(thread->id, time, str);
		thread->p_last = thread->p_head;
	}
	else
	{
		thread->p_last->next = new_node(thread->id, time, str);
		thread->p_last = thread->p_last->next;
	}
}

void	print(t_thr_data *thread, size_t time, char *str)
{
	if (thread->shr->end)
		return ;
	pthread_mutex_lock(&thread->p_lock);
	add_node(thread, time, str);
	pthread_mutex_unlock(&thread->p_lock);
}

void take_forks(t_thr_data *thread)
{
	pthread_mutex_lock(&thread->shr->fork[thread->id]);
	print(thread, run_time(thread->shr->start_time), "is taken R fork");
	pthread_mutex_lock(&thread->shr->fork[thread->right]);
	print(thread, run_time(thread->shr->start_time), "is taken L fork");
}

void leave_fork(t_thr_data *thread)
{
	pthread_mutex_unlock(&thread->shr->fork[thread->right]);
	pthread_mutex_unlock(&thread->shr->fork[thread->id]);
}


void *philo_thread(void* arg)
{
	t_thr_data *thread;
	thread = (t_thr_data*)arg;
	thread->right = (thread->id + 1) % thread->shr->p_count;
	thread->l_eat = 1;
	thread->eated = 0;
	pthread_mutex_lock(&thread->shr->fork[thread->shr->p_count]);
	pthread_mutex_unlock(&thread->shr->fork[thread->shr->p_count]);
	thread->l_eat = thread->shr->start_time;
	if (thread->id % 2)
		s_sleep(thread->shr->t_eat / 2);
	while (!thread->shr->end)
	{
		take_forks(thread);
		pthread_mutex_lock(&thread->e_lock);
		thread->l_eat = gettime();
		pthread_mutex_unlock(&thread->e_lock);
		print(thread, run_time(thread->shr->start_time), "is eating");
		s_sleep(thread->shr->t_eat);
		leave_fork(thread);
		print(thread, run_time(thread->shr->start_time), "is sleeping");
		s_sleep(thread->shr->t_sleep);
		print(thread, run_time(thread->shr->start_time), "is thinking");
	}
	pthread_exit(NULL);
}

void	check_die(t_shr_data *shared)
{
	int			i;
	size_t		cur_time;

	i = -1;
	while (++i < shared->p_count)
		if (shared->thr[i].l_eat == 1)
			i = -1;
	while (1)
	{
		usleep(100);
		i = -1;
		cur_time = gettime();
		while (++i < shared->p_count)
		{
			pthread_mutex_lock(& shared->thr[i].e_lock);
			if ((cur_time - shared->thr[i].l_eat) > shared->t_die)
			{
/* 				printf("l_eat:	%ld\ncur:	%ld\nfark:	%ld\ns_time:	%ld",\
					(shared->thr[i].l_eat), cur_time,\
					(cur_time - shared->thr[i].l_eat),\
					shared->start_time); */
				print(&shared->thr[i], run_time(shared->start_time), "died");
				pthread_mutex_unlock(&shared->thr[i].e_lock);
				return ;
			}
			pthread_mutex_unlock(&shared->thr[i].e_lock);
		}
	}
}


int main(int argc, char **argv)
{
	int				i;
	pthread_t		print_t;
	t_shr_data	shared;
	t_thr_data	*thread;

/* 	shared.p_count = 100;
	shared.t_die = 1000;
	shared.t_eat = 300;
	shared.t_sleep = 300; */

	if (argc != 5 && argc != 6)
		return(printf("Wrong input\n"));

	if(!input_init(&shared, argv, argc))
		return (printf("Invalid Number\n"));

	shared.fork = malloc(sizeof(pthread_mutex_t) * (shared.p_count + 1));
	shared.thread_id = malloc(sizeof(pthread_t) * shared.p_count);
	thread = malloc(sizeof(t_thr_data) * shared.p_count);
	shared.thr = thread;
	i = -1;
	while (++i <= shared.p_count)
		pthread_mutex_init(&shared.fork[i], NULL);
	i = -1;
	while (++i < shared.p_count)
		thread[i].l_eat = 0;
	i = -1;
	pthread_mutex_lock(&shared.fork[shared.p_count]);
	while (++i < shared.p_count)
	{
		thread[i].shr = &shared;
		thread[i].id = i;
		thread[i].p_head = NULL;
		pthread_mutex_init(&thread[i].p_lock, NULL);
		pthread_mutex_init(&thread[i].e_lock, NULL);
		pthread_create(&shared.thread_id[i], NULL, philo_thread, &thread[i]);
	}
	i = -1;
	while (++i < shared.p_count)
		if (thread[i].l_eat != 1)
			i = -1;
	shared.start_time = gettime();
	pthread_mutex_unlock(&shared.fork[shared.p_count]);
	pthread_create(&print_t, NULL, printer, &shared);
	check_die(&shared);
	i = -1;
	while (++i < shared.p_count)
		pthread_join(shared.thread_id[i], NULL);
	pthread_join(print_t, NULL);
	i = -1;
	while (++i < shared.p_count)
		pthread_mutex_destroy(&shared.fork[i]);
	free(shared.fork);
	free(shared.thread_id);
	free(thread);
	return (0);
}
