#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5  // Toplam filozof sayısı
#define MEALS_TO_EAT 3      // Her filozofun yemesi gereken yemek sayısı

pthread_mutex_t forks[NUM_PHILOSOPHERS];  // Her çatal için bir mutex
pthread_t philosophers[NUM_PHILOSOPHERS]; // Filozof iş parçacığı dizisi

void *philosopher(void *arg) {
    int id = *(int *)arg;
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;

    for (int i = 0; i < MEALS_TO_EAT; ++i) {
        printf("Philosopher %d is thinking...\n", id);
        usleep(rand() % 1000000); // Rastgele düşünme süresi

        printf("Philosopher %d is hungry...\n", id);

        // Çatal alma işlemleri
        pthread_mutex_lock(&forks[left_fork]);
        pthread_mutex_lock(&forks[right_fork]);

        printf("Philosopher %d is eating...\n", id);
        usleep(100000000); // Rastgele yeme süresi

        pthread_mutex_unlock(&forks[left_fork]);
        pthread_mutex_unlock(&forks[right_fork]);

        printf("Philosopher %d finished eating.\n", id);
    }

    return NULL;
}

int main() {
    srand(time(NULL));

    // Çatal mutex'lerini başlat
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Filozof iş parçacıklarını oluştur ve başlat
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&philosophers[i], NULL, philosopher, id);
    }

    // İş parçacıklarının tamamlanmasını bekle
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_join(philosophers[i], NULL);
    }

    // Çatal mutex'lerini temizle
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_mutex_destroy(&forks[i]);
    }

    return 0;
}