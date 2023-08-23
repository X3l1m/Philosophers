#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define BIG 100000

int counter = 0;
pthread_mutex_t mutex[2];

void *count(void* arg)
{
	for(int i = 0; i < BIG; i++)
	{
		pthread_mutex_lock(&mutex[1]);
		counter++;
		pthread_mutex_unlock(&mutex[1]);
	}
	pthread_exit(NULL);
}

void *pates(void* a)
{
	for(int i = 0; i < BIG; i++)
	{
		pthread_mutex_lock(&mutex[0]);
		counter++;
		pthread_mutex_unlock(&mutex[0]);
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_t t[2];
	pthread_mutex_init(&mutex[0], NULL);
	pthread_mutex_init(&mutex[1], NULL);
	pthread_create(&t[0], NULL, count, NULL);
	pthread_create(&t[1], NULL, pates, NULL);
	pthread_join(t[0], NULL);
	pthread_join(t[1], NULL);

	pthread_mutex_destroy(&mutex[0]);
	pthread_mutex_destroy(&mutex[1]);
	printf("Counter = %u\n", counter);
}