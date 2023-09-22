t_print	*new_node(t_print *back, int id, size_t time, char *str)
{
	t_print *new;
	new = malloc(sizeof(t_print));
	if (!new)
		exit(0); //malloc error
	new->ms = time;
	new->msg = str;
	new->next = NULL;
	new->back = back;
	new->id = id;
	return (new);
}

void 	add_node(t_thr_data *thread, size_t time, char *str)
{
	if (!thread->shr->p_head)
	{
		thread->shr->p_head = new_node(NULL, thread->id, time, str);
		thread->shr->p_last = thread->shr->p_head;
	}
	else
	{
		thread->shr->p_last->next = new_node(thread->shr->p_last, thread->id, time, str);
		thread->shr->p_last = thread->shr->p_last->next;
	}
}

void	print(t_thr_data *thread, size_t time, char *str)
{
	pthread_mutex_lock(&thread->shr->fork[thread->shr->p_count]);
	add_node(thread, time, str);
	pthread_mutex_unlock(&thread->shr->fork[thread->shr->p_count]);
}