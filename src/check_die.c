/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   check_die.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: seyildir <seyildir@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/20 14:16:29 by seyildir      #+#    #+#                 */
/*   Updated: 2023/12/20 14:16:29 by seyildir      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

int	one_philo(t_shr_data *shared)
{
	printf("0	1 is taken a fork\n");
	usleep(shared->t_die * 1000);
	printf("%d	1 is died\n", shared->t_die);
	return (0);
}

int	check_end(t_shr_data *s, int end)
{
	pthread_mutex_lock(&s->fork[s->p_count]);
	if (end)
		s->end = end;
	else
		end = s->end;
	pthread_mutex_unlock(&s->fork[s->p_count]);
	return (end);
}

void	check_die(t_shr_data *s)
{
	int			i;
	size_t		cur_time;

	while (!check_end(s, 0))
	{
		i = -1;
		cur_time = gettime();
		while (++i < s->p_count)
		{
			pthread_mutex_lock(&s->thr[i].e_lock);
			if ((cur_time - s->thr[i].l_eat) > (size_t)s->t_die)
			{
				if (print(&s->thr[i], run_time(s->start_time), "died"))
					check_end(s, 2);
				check_end(s, 1);
				pthread_mutex_unlock(&s->thr[i].e_lock);
				return ;
			}
			pthread_mutex_unlock(&s->thr[i].e_lock);
		}
		usleep(1000);
	}
}
