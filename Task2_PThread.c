#include <time.h> 
#include "sem.h"		
#include <unistd.h>	
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// initializing each mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t agentmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t paper = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tobacco = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t match = PTHREAD_MUTEX_INITIALIZER;

// threads
pthread_t agent1, paper1, tobacco1, match1;
 
// smoker class
void *smoker(void *ptr) { 

  const char *id_char = (char *)ptr;
  int n = atoi(id_char);

  while (1) {

    switch (n) {
      case 1:  // opens up the tobacco smoker
      pthread_mutex_lock(&tobacco);    
      pthread_mutex_lock(&lock);
      printf("Tobacco picked up match and paper\n");
      pthread_mutex_unlock(&agentmutex);
      pthread_mutex_unlock(&lock);
      printf("Tobacco rolled a cigarette\n\n");
      break;
      
      case 2:  // opens up the match smoker
      pthread_mutex_lock(&match);  
      pthread_mutex_lock(&lock);
      printf("Match picked up paper and tobacco\n");
      pthread_mutex_unlock(&agentmutex);        
      pthread_mutex_unlock(&lock);
      printf("Match rolled up a cigarette\n\n");
      break;
      
      case 3:  // opens up the paper smoker
      pthread_mutex_lock(&paper);  
      pthread_mutex_lock(&lock);
      printf("Paper picked up match and tobacco \n");        
      printf("Paper rolled up a cigarette\n\n");
      pthread_mutex_unlock(&agentmutex);  
      pthread_mutex_unlock(&lock);  
      break;
    }
  }
}

// agent class
void *agent() {
  
  int count = 0;
  for (int i = 0; i < 10; i++) {
    pthread_mutex_lock(&lock);
    
    // number to pick smoker
    int random = rand() % 3 + 1; 
    
    if (random == 1) {   //paper number
      printf("The agent put tobacco and match on the table. So Paper gets activated\n");
      pthread_mutex_unlock(&paper);
    }
    
    else if (random == 2) {  //tobacco number
      printf("The agent put match and paper on table. So Tobacco gets activated\n");
      pthread_mutex_unlock(&tobacco);
    }
    
    else {  // match number
      printf("The agent put tobacco and paper on the table. So Match gets activated\n");
      pthread_mutex_unlock(&match);
    }
    pthread_mutex_unlock(&lock);
    pthread_mutex_lock(&agentmutex);
  }

  pthread_mutex_lock(&lock);
  pthread_cancel(paper1);
  pthread_cancel(tobacco1);
  pthread_cancel(match1);
  pthread_mutex_unlock(&lock);
}

// main class
int main() {
  
  // random time
  srand(time(0));
  
  // smoker ID
  char *paperid = "1"; // paper
  char *tobaccoid = "2"; // tobacco
  char *matchid = "3"; // narch

  //initializing the mutex
  pthread_mutex_lock(&agentmutex);
  pthread_mutex_lock(&paper);
  pthread_mutex_lock(&tobacco);
  pthread_mutex_lock(&match);

  // create threads
  pthread_create(&agent1, NULL, agent, NULL);
  pthread_create(&paper1, NULL, smoker, (void *)paperid);
  pthread_create(&tobacco1, NULL, smoker, (void *)tobaccoid);
  pthread_create(&match1, NULL, smoker, (void *)matchid);
  
  // agent finish
  pthread_join(agent1, NULL);

  printf("No more Smoking!!\n");

  // exits the code
  exit(0);
}