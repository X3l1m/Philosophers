#include "philosophers.h"

void	wait(t_shr_data *s)
{
	int	i;

	i = -1;
	while (++i < s->p_count)
		if (s->thr[i].l_eat == 1)
			i--;
}

void	check_die(t_shr_data *s)
{
	int			i;
	size_t		cur_time;

	wait(s);
	while (!s->end)
	{
		usleep(1000);
		i = -1;
		cur_time = gettime();
		while (++i < s->p_count)
		{
			pthread_mutex_lock(&s->thr[i].e_lock);
			if ((cur_time - s->thr[i].l_eat) > (size_t)s->t_die)
			{
				if (print(&s->thr[i], run_time(s->start_time), "died"))
					s->end = 2;
				s->end = 1;
				pthread_mutex_unlock(&s->thr[i].e_lock);
				return ;
			}
			pthread_mutex_unlock(&s->thr[i].e_lock);
		}
	}
}
