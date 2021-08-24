#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>

//definir cuanto tiempo toma usar el bano
#define bathroom_time_max 5
#define bathroom_time_min 1
#define people_to_simulate 20
#define arrive_time_max 3
#define arrive_time_min 1

// prototype
void * male(void * args);
void * female(void * args);
void use_bathroom();

sem_t empty;
sem_t male_mutex;
sem_t male_multiplex;
int male_counter = 0;
sem_t female_mutex;
sem_t female_multiplex;
int female_counter = 0;

pthread_t t_id[people_to_simulate];


int main() {
    void * status;

    srand( (long)time(NULL) );

    printf("Simulando para %d personas. Usar el banho toma entre %d y %d segundos.\n",
        people_to_simulate, bathroom_time_min, bathroom_time_max);
    
    sem_init(&empty, 0, 1); // at beginning bathroom is empty
    sem_init(&male_mutex, 0, 1);
    sem_init(&male_multiplex, 0, 3); // 3 places left in bathroom for men
    sem_init(&female_mutex, 0, 1);
    sem_init(&female_multiplex, 0, 3); // 3 places left in bathroom for women

    int is_female;
    for(int i = 0; i < people_to_simulate; i++) {
        // wait for next person to arrive
        sleep(rand() % (arrive_time_max - arrive_time_min + 1) + arrive_time_min);
        
        is_female = rand() % 2;
        if(pthread_create(&t_id[i], 0, is_female ? female : male, NULL) != 0) {
            perror("pthread_create() failure.");
            exit(1);
        }
    }

    for(int i = 0; i < people_to_simulate; i++) {
        if(!pthread_join(t_id[i], &status ) == 0) {
            perror("thr_join() failure.");
            exit(1);
        }
    }
}


void * male(void * args) {
    sem_wait(&male_mutex);
    if(++male_counter == 1)
        sem_wait(&empty);
    sem_post(&male_mutex);

    sem_wait(&male_multiplex);
    printf("Hombre entra\n");
    fflush(stdout);
    use_bathroom();
    printf("Hombre sale\n");
    fflush(stdout);
    sem_post(&male_multiplex);

    sem_wait(&male_mutex);
    if(--male_counter == 0)
        sem_post(&empty);
    sem_post(&male_mutex);
}

void * female(void * args) {
    sem_wait(&female_mutex);
    if(++female_counter == 1)
        sem_wait(&empty);
    sem_post(&female_mutex);

    sem_wait(&female_multiplex);
    printf("Mujer entra\n");
    fflush(stdout);
    use_bathroom();
    printf("Mujer sale\n");
    fflush(stdout);
    sem_post(&female_multiplex);

    sem_wait(&female_mutex);
    if(--female_counter == 0)
        sem_post(&empty);
    sem_post(&female_mutex);
}

void use_bathroom() {
    sleep(rand() % (bathroom_time_max - bathroom_time_min + 1) + bathroom_time_min);
}
