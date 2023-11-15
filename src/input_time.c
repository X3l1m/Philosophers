#include <philosophers.h>

size_t	gettime(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	s_sleep(size_t time)
{
	size_t	start;

	start = gettime();
	while ((gettime()) - start < time)
		usleep(250);
}

size_t	run_time(size_t start_time)
{
	return (gettime() - start_time);
}

int	char_to_int(char *str)
{
	size_t	num;

	num = 0;
	if (*str == '+')
		str++;
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
			num = num * 10 + (*str - 48);
		else
			return (0);
		if (num > INT_MAX)
			return (0);
		str++;
	}
	return (num);
}

int	input_init(t_shr_data *shared, char **argv, int argc)
{
	shared->p_count = char_to_int(argv[1]);
	if (!shared->p_count)
		return (0);
	shared->t_die = char_to_int(argv[2]);
	if (!shared->t_die)
		return (0);
	shared->t_eat = char_to_int(argv[3]);
	if (!shared->t_eat)
		return (0);
	shared->t_sleep = char_to_int(argv[4]);
	if (!shared->t_sleep)
		return (0);
	shared->must_eat = 0;
	if (argc == 6)
	{
		shared->must_eat = char_to_int(argv[5]);
		if (!shared->must_eat)
			return (0);
	}
	shared->end = 0;
	return (1);
}
