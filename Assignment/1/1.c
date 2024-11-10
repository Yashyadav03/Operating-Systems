#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


pthread_mutex_t forks[5];
pthread_mutex_t bowls_mutex;
pthread_cond_t bowls_cond;

int bowls;

void eating(int id) {
    printf("Philosopher %d is eating with forks %d and %d and a bowl.\n", id, id - 1, id % 5);
    sleep(2); 
}

void thinking(int id) {
    printf("Philosopher %d is thinking.\n", id);
    sleep(2); 
}

void *philosopher(void *args)
{
    int id = *((int *)args);

    while (1)
    {
        thinking(id);

        pthread_mutex_lock(&bowls_mutex);
        while (bowls == 0)
        {
            pthread_cond_wait(&bowls_cond, &bowls_mutex);
        }
        bowls--;
        pthread_mutex_unlock(&bowls_mutex);
        pthread_mutex_lock(&forks[id - 1]);
        pthread_mutex_lock(&forks[id % 5]);

       
        eating(id);

        pthread_mutex_unlock(&forks[id - 1]);
        pthread_mutex_unlock(&forks[id % 5]);
        pthread_mutex_lock(&bowls_mutex);
        bowls++;
        pthread_cond_signal(&bowls_cond);
        pthread_mutex_unlock(&bowls_mutex);
    }

}

int main()
{
    pthread_t philosophers[5];
    int ids[5];

    pthread_mutex_init(&bowls_mutex, NULL);
    pthread_cond_init(&bowls_cond, NULL);

    bowls = 2;
    
    int i = 0;
    while(i < 5)
    {
        pthread_mutex_init(&forks[i], NULL);
        i++;
    }
    
    int j = 0;
    while(j < 5)
    {

        ids[j] = j + 1;
        pthread_create(&philosophers[j], NULL, philosopher, (void *)&ids[j]);
        j++;
    }
    
    int k = 0;
    while(k < 5)
    {
        pthread_join(philosophers[k], NULL);
        k++;
    }

    return 0;
}
