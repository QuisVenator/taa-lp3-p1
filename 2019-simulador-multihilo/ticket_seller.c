#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>

pthread_mutex_t mutex;
int maxTiempo;
int ticketCount;

void *vendedor(void *arg);
void sleepFor(int min, int max);

int main(int argc, char** argv) {
    void *status;
    srand(time(NULL));

    if(argc < 4) {
        printf("Not enough parameters");
        exit(1);
    }

    int numHilos = atoi(argv[1]);
    maxTiempo = atoi(argv[2]);
    ticketCount = atoi(argv[3]);

    pthread_t tId[numHilos];
    int vendedorNum[numHilos];

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_lock(&mutex);

    for(int i = 0; i < numHilos; i++) {
        vendedorNum[i] = i;
        if(pthread_create(&tId[i], NULL, vendedor, &vendedorNum[i])) {
            perror("thr_join() failure.");
            exit(1);
        }
    }

    pthread_mutex_unlock(&mutex);

    for(int i = 0; i < numHilos; i++) {
        if(pthread_join(tId[i], &status )) {
            perror("thr_join() failure.");
            exit(1);
        }
    }
}

void *vendedor(void *arg) {
    int selfSold = 0;
    pthread_mutex_lock(&mutex);
    while (ticketCount > 0)
    {
        printf("Seller #%d sold one (%d left)\n", *(int *)arg, --ticketCount);
        pthread_mutex_unlock(&mutex);
        selfSold++;

        sleepFor(1, maxTiempo);
        pthread_mutex_lock(&mutex);
    }
    printf("Seller #%d noticed all tickets sold! (I sold %d myself)\n", *(int *)arg, selfSold);
    pthread_mutex_unlock(&mutex);
}

void sleepFor(int min, int max) {
    sleep(rand() % (maxTiempo - min + 1) + min);
}