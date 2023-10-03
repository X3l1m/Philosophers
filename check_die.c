#include "philosophers.h"

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
				if (print(&shared->thr[i], run_time(shared->start_time), "died"))
					shared->end = 1;
				pthread_mutex_unlock(&shared->thr[i].e_lock);
				return ;
			}
			pthread_mutex_unlock(&shared->thr[i].e_lock);
		}
	}
}