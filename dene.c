#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define THINKING 0
#define HUNGRY 1
#define EATING 2

int num_philosophers;
int *state;
int *philosophers;
pthread_mutex_t mutex;
pthread_mutex_t *forks;
int hunger_duration, eating_duration, sleeping_duration;

void test(int ph_num) {
	if (state[ph_num] == HUNGRY && state[(ph_num + num_philosophers - 1) % num_philosophers] != EATING && state[(ph_num + 1) % num_philosophers] != EATING) {
		state[ph_num] = EATING;
		sleep(2);
		printf("Philosopher %d takes fork %d and %d\n", ph_num + 1, (ph_num + num_philosophers - 1) % num_philosophers + 1, ph_num + 1);
		printf("Philosopher %d is Eating\n", ph_num + 1);
		pthread_mutex_unlock(&forks[ph_num]);
	}
}

void take_fork(int ph_num) {
	pthread_mutex_lock(&mutex);
	state[ph_num] = HUNGRY;
	printf("Philosopher %d is Hungry\n", ph_num + 1);
	test(ph_num);
	pthread_mutex_unlock(&mutex);
	pthread_mutex_lock(&forks[ph_num]);
	sleep(1);
}

void put_fork(int ph_num) {
	pthread_mutex_lock(&mutex);
	state[ph_num] = THINKING;
	printf("Philosopher %d putting fork %d and %d down\n", ph_num + 1, (ph_num + num_philosophers - 1) % num_philosophers + 1, ph_num + 1);
	printf("Philosopher %d is thinking\n", ph_num + 1);
	test((ph_num + num_philosophers - 1) % num_philosophers);
	test((ph_num + 1) % num_philosophers);
	pthread_mutex_unlock(&mutex);
}

void *philospher(void *num) {
	while (1) {
		int *i = num;
		usleep(hunger_duration);
		take_fork(*i);
		usleep(eating_duration);
		put_fork(*i);
		usleep(sleeping_duration);
	}
}

int main() {
	int i;
	printf("Enter the number of philosophers: ");
	scanf("%d", &num_philosophers);

	printf("Enter hunger duration for all philosophers: ");
	scanf("%d", &hunger_duration);

	printf("Enter eating duration for all philosophers: ");
	scanf("%d", &eating_duration);

	printf("Enter sleeping duration for all philosophers: ");
	scanf("%d", &sleeping_duration);

	philosophers = malloc(num_philosophers * sizeof(int));
	state = malloc(num_philosophers * sizeof(int));
	forks = malloc(num_philosophers * sizeof(pthread_mutex_t));

	pthread_t thread_id[num_philosophers];
	pthread_mutex_init(&mutex, NULL);

	for (i = 0; i < num_philosophers; i++) {
		pthread_mutex_init(&forks[i], NULL);
		philosophers[i] = i;
		state[i] = THINKING;
		pthread_create(&thread_id[i], NULL, philospher, &philosophers[i]);
	}

	for (i = 0; i < num_philosophers; i++) {
		pthread_join(thread_id[i], NULL);
	}

	free(philosophers);
	free(state);
}
