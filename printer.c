#include "philosophers.h"

int	find_node(t_shr_data *shared)
{
	int		i;

	i = 0;
	while (1)
	{
		if (i == shared->p_count)
			i = 0;
		if (shared->thr[i].p_head)
			break;
		i++;
	}
	return (i);
}

int	find_min(t_shr_data *s, int m)
{
	int	i;

	i = -1;
	while (++i < s->p_count)
	{
		if (s->thr[i].p_head && s->thr[m].p_head->ms > s->thr[i].p_head->ms)
			m = i;
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

void	free_node(t_print **node)
{
	t_print *tmp;

	tmp = *node;
	*node = (*node)->next;
	free(tmp);
}

int	print_min(t_shr_data *s)
{
	int		m;
	t_print	*tmp;

	m = find_node(s);
	m = find_min(s, m);
	printf("%ld %d %s\n", s->thr[m].p_head->ms, \
		s->thr[m].p_head->id + 1, s->thr[m].p_head->msg);
	if (*s->thr[m].p_head->msg == 'd' ||
		check_eat(s, s->thr[m].p_head->msg, m))
		s->end = 2;
	pthread_mutex_lock(&s->thr[m].p_lock);
	free_node(&s->thr[m].p_head);
	pthread_mutex_unlock(&s->thr[m].p_lock);
	return (0);
}

void	*printer(void *arg)
{
	t_shr_data	*shared;
	size_t		cur_time;

	shared = (t_shr_data*)arg;
	while (shared->end < 2)
	{
		usleep(1000);
		print_min(shared);
	}
	pthread_exit(NULL);
}