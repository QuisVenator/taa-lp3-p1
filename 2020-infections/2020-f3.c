#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int maxTimeToInfect;
int infectiousAgentCount;
int infectionCount;
int maxInfection;
pthread_mutex_t mutex;

void *infect(void *params)
{
  int agent_number = *((int *)params);
  int timeToWait = rand() % maxTimeToInfect + 1;
  sleep(timeToWait);
  
  pthread_mutex_lock(&mutex);
  while (infectionCount < maxInfection)
  {
    infectionCount++;
    printf("Agente #%d infecto a una persona luego de %d seg. (total contagios: %d)\n", agent_number, timeToWait, infectionCount);
    pthread_mutex_unlock(&mutex);
    
    timeToWait = rand() % maxTimeToInfect + 1;
    sleep(timeToWait);
    pthread_mutex_lock(&mutex);
  }
  pthread_mutex_unlock(&mutex);
}

int main(int argc, char *const argv[])
{
  srand(time(NULL));

  infectiousAgentCount = atoi(argv[1]);
  maxTimeToInfect = atoi(argv[2]);
  maxInfection = atoi(argv[3]);

  pthread_t *tId = (pthread_t *)malloc(infectiousAgentCount * sizeof(pthread_t));
  int *agents_number = (int *)malloc(infectiousAgentCount * sizeof(int));

  long start = time(NULL);

  // Create all the threads
  for (int i = 0; i < infectiousAgentCount; i++)
  {
    agents_number[i] = i + 1;
    pthread_create(&tId[i], NULL, &infect, &agents_number[i]);
  }

  // Join all the threads
  for (int i = 0; i < infectiousAgentCount; i++)
  {
    pthread_join(tId[i], NULL);
  }

  printf("Total de %d contagios alcanzados en %ld segundos.\n", infectionCount, (long)time(NULL) - start);

  free(tId);
  free(agents_number);

  return 0;
}
