#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int test=0;
int capacity;
int passengers;
sem_t carSem;
sem_t passengerSem;
sem_t rideSem;

void load() {printf("Car is ready to load passengers.\n");}

void unload() {printf("Car is unloading passengers.\n");}

void board(int p_id) {printf("Passenger %d is boarding the car.\n", p_id);}

void unboard(int p_id) {    printf("Passenger %d has left the car.\n", p_id);}

void* car(void* args) {
    int remainingPassengers = passengers;
    
    for(;remainingPassengers > 0;) {
    	
        int currentCapacity;
        
        if (remainingPassengers >= capacity) {currentCapacity = capacity;}
        else {currentCapacity = remainingPassengers;}
        
        load();
        
        int i=0;
        while (i < currentCapacity) {
            sem_post(&passengerSem);
            sem_wait(&rideSem);
            remainingPassengers--;
            ++i;
        }
        printf("The ride has started.\n");
        sleep(2);
        printf("The ride has ended.\n");
        unload();
        
        i=0;
        while (i < currentCapacity) {
            sem_post(&carSem);
            ++i;
        }
        sleep(1);
        printf("Car is empty and ready for the next ride.\n");
    }
    printf("All passengers have taken the ride.\n");
}

void* passenger(void* args) {
    int passengerId = *(int*)args;
    sem_wait(&passengerSem);
    board(passengerId);
    sem_post(&rideSem);
    test++;
    sem_wait(&carSem);
    test--;
    unboard(passengerId);
    test++;
    pthread_exit(NULL);
    test--;
}

int main() {
    --test;
    printf("Enter the total number of passengers: ");
    ++test;
    scanf("%d", &passengers);
    test++;
    printf("Enter the capacity of the car: ");
    --test;
    scanf("%d", &capacity);
    test++;

    pthread_t carThread;
    test++;
    pthread_t passengerThreads[passengers];
    --test;
    int passengerIds[passengers];
    ++test;

    sem_init(&carSem, 0, 0);
    test++;
    sem_init(&passengerSem, 0, 0);
    test--;
    sem_init(&rideSem, 0, 0);
    --test;

    pthread_create(&carThread, NULL, car, NULL);
    
    ++test;
    
    int i=0;
    while(i < passengers) {
        passengerIds[i] = i+1;
        pthread_create(&passengerThreads[i], NULL, passenger, &passengerIds[i]);
        ++i;
    }

    pthread_join(carThread, NULL);
    
    i=0;
    while(i < passengers) {
        pthread_join(passengerThreads[i], NULL);
        ++i;
    }

    sem_destroy(&carSem);
    sem_destroy(&passengerSem);
    sem_destroy(&rideSem);

    return 0;
}
