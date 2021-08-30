#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
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

/* mutex for changing meal counter */
pthread_mutex_t mealCountMutex;

/* prototype */
void *philosopher(void *);

int main() {
    pthread_t tid[thread_num];
    void *status;
    int i,j;

    srand( (long)time(NULL) ); // TODO what is this for?

    // initialize mutex for meal count 
    // and lock so first philosopher can't eat inmediately
    pthread_mutex_init(&mealCountMutex, NULL);
    pthread_mutex_lock(&mealCountMutex);

    // initialize mutexes for each chopstick
    for(int i = 0; i < thread_num; i++) {
        pthread_mutex_init(&m[i], NULL);
    }

    /* create 5 threads representing
    5 dinning philosopher */
    for (i = 0; i < thread_num; i++) {
        if(pthread_create( tid + i, 0, philosopher, p + i ) != 0) {
            perror("pthread_create() failure.");
            exit(1);
        }
    }

    // now we can indead say that all 5 are sitting at table and none eating
    printf("Initial number of meals = %d.\n", max_meals);
    printf("All philosophers are sitting at the table.\n\n");

    // unlock mutex so everyone can start eating
    pthread_mutex_unlock(&mealCountMutex);

    /* wait for the join of 5 threads */
    for (i = 0; i < thread_num; i++) {
        if(!pthread_join(tid[i], &status ) == 0) {
            perror("thr_join() failure.");
            exit(1);
        }
    }

    printf("\n");
    for(i=0; i<thread_num; i++)
        printf("Philosopher %d ate %d meals.\n", i, numMeals[i]);

    printf("\nmain(): The philosophers have left. I am going to exit!\n\n");

    return (0);
}


void *philosopher(void *arg) {
    int left = * (int *)arg;
    int right = (left + 1) % thread_num;

    while(1){
        pthread_mutex_lock(&mealCountMutex);
        if(mealCount < max_meals) {
            pthread_mutex_unlock(&mealCountMutex);
            printf("philosopher %d: I am going to eat!\n", left);

            pthread_mutex_lock(&m[left]);
            if(chopstick[left]) {
                chopstick[left] = 0;
                pthread_mutex_unlock(&m[left]);
                printf("Philosopher %d: I got the left one!\n", left);

                pthread_mutex_lock(&m[right]);
                if(chopstick[right]) {
                    chopstick[right] = 0;
                    pthread_mutex_unlock(&m[right]);
                    printf("Philosopher %d: I got two chopsticks!\n", left);

                    pthread_mutex_lock(&mealCountMutex);
                    if(mealCount < max_meals) {
                        mealCount++;
                        pthread_mutex_unlock(&mealCountMutex);

                        numMeals[left]++;
                        printf("philosopher %d: I am eating!\n", left);
                        sleep(rand() % 5 + 1); // comer un tiempo
                    } else {
                        pthread_mutex_unlock(&mealCountMutex);
                    }

                    pthread_mutex_lock(&m[right]);
                    chopstick[right] = 1;
                    pthread_mutex_unlock(&m[right]);
                } else {
                    pthread_mutex_unlock(&m[right]);
                    printf("Philosopher %d: I cannot get the right one!\n", left);
                }

                pthread_mutex_lock(&m[left]);
                chopstick[left] = 1;
                pthread_mutex_unlock(&m[left]);
            } else {
                pthread_mutex_unlock(&m[left]);
                printf("Philosopher %d: I cannot even get the left chopstick!\n", left);
            }
            sleep(rand() % 5 + 1); // pensar entre 1 y 5 segundos
            
        } else {
            pthread_mutex_unlock(&mealCountMutex);
            printf("Philosopher %d has finished the dinner and is leaving!\n", left);
            return NULL;
        }
    }
}
