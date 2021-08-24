#include<stdio.h>
#include<pthread.h>
#define thread_num 5
#define max_meals 20

/* initial status of chopstick */
/* 1 - the chopstick is available */
/* 0 - the chopstick is not available */
int chopstick[thread_num]={1,1,1,1,1};

/* mutex locks for each chopstick */
pthread_mutex_t m[thread_num];

/* philosopher ID */
int p[thread_num]={0,1,2,3,4};

/* number of meals consumed by each philosopher
*/
int numMeals[thread_num]={0,0,0,0,0};

/* counter for the number of meals eaten */
int mealCount = 0;

/* prototype */
void *philosopher(void *);

int main() {
    pthread_t tid[thread_num];
    void *status;
    int i,j;

    srand( (long)time(NULL) );

    /* create 5 threads representing
    5 dinning philosopher */
    for (i = 0; i < thread_num; i++) {
        if(pthread_create( tid + i, 0, philosopher, p + i ) != 0) {
            perror("pthread_create() failure.");
            exit(1);
        }
    }
    /* wait for the join of 5 threads */
    for (i = 0; i < thread_num; i++) {
        if(!pthread_join(tid[i], &status )==0) {
            perror("thr_join() failure.");
            exit(1);
        }
    }

    printf("Initial number of meals = %d.\n", max_meals);
    printf("All philosophers are sitting at the table.\n\n");
    for(i=0; i<thread_num; i++)
        printf("Philosopher %d ate %d meals.\n", i, numMeals[i]);

    printf("\nmain(): The philosophers have left. I am going to exit!\n\n");

    return (0);
}


void *philosopher(void *arg) {
    
}
