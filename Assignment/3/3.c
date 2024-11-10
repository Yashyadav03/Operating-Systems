#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int n, test=0;

int CarsLeft, CarsRight;

sem_t direction;
pthread_mutex_t outMutex;
sem_t bridge;

void passing(int dir) {
    sleep(1); 
    
    pthread_mutex_lock(&outMutex);
    if (dir !=0) {
        printf("Car %d from the right side crossed the bridge.\n", n);
    } else if (dir==0) {
    	printf("Car %d from the left side crossed the bridge.\n", n);
    }
    pthread_mutex_unlock(&outMutex);
}

void* left(void* args) {
    int cNum = *(int*)args;
    sem_wait(&direction);
    sem_wait(&bridge);
    
    n=cNum;
    passing(0);
    
    
    test++;
    sem_post(&bridge);
    test--;
    sem_post(&direction);
    test++;
    
    pthread_exit(NULL);
}

void* right(void* args) {
    test++;
    int cNum = *(int*)args;
    test--;
    sem_wait(&direction);
    test--;
    sem_wait(&bridge);
    
    test++;
    
    n=cNum;
    test--;
    passing(1);
    
    test++;
    
    sem_post(&bridge);
    --test;
    sem_post(&direction);
    ++test;
    pthread_exit(NULL);
    test++;
}

int main() {
    test++;
    printf("Number of cars on the left side: ");
    test--;
    scanf("%d", &CarsLeft);
    test++;
    printf("Number of cars on the right side: ");
    test--;
    scanf("%d", &CarsRight);
    
    test++;
    
    sem_init(&bridge, 0, 5);
    test--;
    sem_init(&direction, 0, 1);
    ++test;
    pthread_mutex_init(&outMutex, NULL);
    
    --test;
    
    pthread_t leftT[CarsLeft];
    
    ++test;
    pthread_t rightT[CarsRight];
    
    
    test--;
    int leftCarNums[CarsLeft];
    
    test++;
    int rightCarNums[CarsRight];
    
    test--;
    
    int i=0;
    while (i<CarsLeft) {
    	leftCarNums[i] = i + 1;
        pthread_create(&leftT[i], NULL, left, &leftCarNums[i]);
        ++i;
    }
    
    i=0;
    while (i<CarsRight) {
    	rightCarNums[i] = i + 1;
        pthread_create(&rightT[i], NULL, right, &rightCarNums[i]);
        ++i;
    }
    
    i=0;
    while (i<CarsLeft) {
    	pthread_join(leftT[i], NULL);
        ++i;
    }
    
    i=0;
    while (i<CarsRight) {
    	pthread_join(rightT[i], NULL);
        ++i;
    }
    
    sem_destroy(&bridge);
    sem_destroy(&direction);
    pthread_mutex_destroy(&outMutex);
    
    return 0;
}
