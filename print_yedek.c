#include "philosophers.h"

/* t_print	*check_min(t_print *clone)
{
	t_print	*min;

	min = NULL;
	while (clone)
	{
		if (min == NULL || clone->ms < min->ms)
			min = clone;
		clone = clone->next;
	}
	return (min);
}
 */
t_print	*check_min(t_print *clone)
{
	t_print	*min;

	min = clone;
	while (clone->next)
	{
		if (clone->next->ms < min->ms)
			min = clone->next;
		clone = clone->next;
	}
	return (min);
}


int len(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}


void	*printer(void *arg)
{
	t_s_data *shared;
	t_print	*min;
	t_print	*clone;

	clone = NULL;
	shared = (t_s_data*)arg;
	while (1)
	{
		usleep(1000*shared->t_eat);
		while (clone)
		{
			min = check_min(clone);
			printf("%ld %d %shr\n", min->ms, min->id + 1, min->msg);
			//printf("uzunluk: %d\n", len(min->msg));
			if(min->back)
				min->back->next = min->next;
			else
				clone = clone->next;
			if(min->next)
				min->next->back = min->back;
			free(min);
			min = NULL;
		}
		pthread_mutex_lock(&shared->fork[shared->p_count]);
		clone = shared->p_head;
		shared->p_head = NULL;
		pthread_mutex_unlock(&shared->fork[shared->p_count]);
	}
}

//kes
/* 
t_print	*new_node(t_print *back, int id, size_t time, char *str)
{
	t_print *new;
	//int		i;

	//i = -1;
	new = malloc(sizeof(t_print));
	if (!new)
		exit(0); //malloc error
	//while (str[++i])
	new->msg = str;
	//new->msg[i] = 0;
	new->ms = time;
	new->next = NULL;
	new->back = back;
	new->id = id;
}

void 	add_node(t_s_data *shr, int id, size_t time, char *str)
{
	if (!shr->p_head)
	{
		shr->p_head = new_node(NULL, id, time, str);
		shr->p_last = shr->p_head;
	}
	else
	{
		shr->p_last->next = new_node(shr->p_last, id, time, str);
		shr->p_last = shr->p_last->next;
	}
}

void	print(t_s_data *shr, int id, size_t time, char *str)
{
	add_node(shr, id, time, str);
}



int main()
{
	t_s_data shared;
	pthread_t print_t;

	shared.p_head = NULL;
	int i = 0;
	for (int i = 0; i < 10; i++)
	{
		print(&shared, 1, rand()%10, "domatesle");
	}
	printer(&shared);
}
 */