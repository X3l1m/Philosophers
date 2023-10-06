#include "philosophers.h"

void	free_node(t_print **node)
{
	t_print	*tmp;

	tmp = *node;
	*node = (*node)->next;
	free(tmp);
}

void	mutex_clean(t_shr_data *shared)
{
	int	i;

	i = -1;
	while (shared->p_count > ++i)
	{
		pthread_mutex_destroy(&shared->fork[i]);
		pthread_mutex_destroy(&shared->thr[i].e_lock);
		pthread_mutex_destroy(&shared->thr[i].p_lock);
	}
	pthread_mutex_destroy(&shared->fork[shared->p_count]);
}

int	free_all(t_shr_data *shared, int step, int mutex)
{
	int		i;
	t_print	*tmp;

	i = -1;
	if (mutex)
		mutex_clean(shared);
	if (step >= 4)
		while (shared->p_count > ++i)
			while (shared->thr[i].p_head)
				free_node(&shared->thr[i].p_head);
	if (step >= 1)
		free(shared->thr);
	if (step >= 2)
		free(shared->fork);
	if (step >= 3)
		free(shared->thread_id);
	return (step);
}
