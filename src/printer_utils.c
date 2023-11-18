#include <philosophers.h>

int	find_node(t_shr_data *shared)
{
	int		i;
	ssize_t	ms;

	i = 0;
	ms = -1;
	while (1)
	{
		if (i == shared->p_count)
			i = 0;
		pthread_mutex_lock(&shared->thr[i].p_lock);
		if (shared->thr[i].p_head)
			ms = shared->thr[i].p_head->ms;
		pthread_mutex_unlock(&shared->thr[i].p_lock);
		if (ms != -1)
			break ;
		usleep(100);
		i++;
	}
	return (ms);
}

int	find_min(t_shr_data *s, size_t ms)
{
	int	i;
	int	m;

	i = -1;
	while (++i < s->p_count)
	{
		pthread_mutex_lock(&s->thr[i].p_lock);
		if (s->thr[i].p_head && s->thr[i].p_head->ms <= ms)
		{
			ms = s->thr[i].p_head->ms;
			m = i;
		}
		pthread_mutex_unlock(&s->thr[i].p_lock);
	}
	return (m);
}

int	ft_strncmp(const char *s1, const char *s2, int n)
{
	int	i;

	i = 0;
	if (n <= 0)
		return (0);
	while (s1[i] == s2[i] && s1[i] && s2[i] && n > i + 1)
		i++;
	return (s1[i] - s2[i]);
}

int	check_eat(t_shr_data *shared, char *msg, int m)
{
	int	i;

	if (!shared->must_eat)
		return (0);
	if (ft_strncmp(msg, "is e", 4))
		return (0);
	i = -1;
	shared->thr[m].eated++;
	while (++i < shared->p_count)
		if (shared->must_eat > shared->thr[i].eated)
			return (0);
	return (1);
}

void	print_min(t_shr_data *s)
{
	int			m;

	m = find_min(s, find_node(s));
	pthread_mutex_lock(&s->thr[m].p_lock);
	printf("%ld	%d	%s\n", s->thr[m].p_head->ms, \
		s->thr[m].p_head->id + 1, s->thr[m].p_head->msg);
	if (*s->thr[m].p_head->msg == 'd'
		|| check_eat(s, s->thr[m].p_head->msg, m))
		check_end(s, 2);
	free_node(&s->thr[m].p_head);
	pthread_mutex_unlock(&s->thr[m].p_lock);
}
