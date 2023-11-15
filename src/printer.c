#include <philosophers.h>

t_print	*new_node(int id, size_t time, char *str)
{
	t_print	*new;

	new = malloc(sizeof(t_print));
	if (!new)
	{
		printf("Malloc fail: %ld %d %s\n", time, id, str);
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
	int	err;

	if (thread->shr->end)
		return (0);
	pthread_mutex_lock(&thread->p_lock);
	err = add_node(thread, time, str);
	pthread_mutex_unlock(&thread->p_lock);
	return (err);
}

void	*printer(void *arg)
{
	t_shr_data	*shared;

	shared = (t_shr_data *)arg;
	while (shared->end < 2)
	{
		usleep(1000);
		print_min(shared);
	}
	return (NULL);
}
