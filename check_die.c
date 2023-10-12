#include "philosophers.h"

int	one_philo(t_shr_data *shared)
{
	printf("0	1 is taken a fork\n");
	usleep(shared->t_die * 1000);
	printf("%d	1 is died\n", shared->t_die);
	return (0);
}

void	wait(t_shr_data *s, int a)
{
	int	i;

	i = -1;
	while (++i < s->p_count)
	{
		if ((s->thr[i].l_eat == 1 && a)
			|| (s->thr[i].l_eat != 1 && !a))
		{
			i--;
			usleep(1000);
		}
	}
}

void	check_die(t_shr_data *s)
{
	int			i;
	size_t		cur_time;

	wait(s, 1);
	while (!s->end)
	{
		i = -1;
		usleep(1000);
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
