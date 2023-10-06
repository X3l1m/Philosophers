#include "philosophers.h"

void	leave_fork(t_thr_data *thread, char right)
{
	pthread_mutex_unlock(&thread->shr->fork[thread->id]);
	if (right)
		pthread_mutex_unlock(&thread->shr->fork[thread->right]);
}

int	take_forks(t_thr_data *thread)
{
	pthread_mutex_lock(&thread->shr->fork[thread->id]);
	if (print(thread, run_time(thread->shr->start_time), "is taken a fork"))
	{
		leave_fork(thread, 0);
		return (1);
	}
	pthread_mutex_lock(&thread->shr->fork[thread->right]);
	if (print(thread, run_time(thread->shr->start_time), "is taken a fork"))
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
		if (take_forks(thread))
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

void	*philo_thread(void *arg)
{
	t_thr_data	*thread;

	thread = (t_thr_data *)arg;
	thread->right = (thread->id + 1) % thread->shr->p_count;
	thread->l_eat = 1;
	thread->eated = 0;
	pthread_mutex_lock(&thread->shr->fork[thread->shr->p_count]);
	pthread_mutex_unlock(&thread->shr->fork[thread->shr->p_count]);
	thread->l_eat = thread->shr->start_time;
	if (thread->id % 2)
		s_sleep(thread->shr->t_eat / 2);
	philo_loop(thread);
	return (NULL);
}
