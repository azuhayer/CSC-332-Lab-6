#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/wait.h>
#include "sem.h"

// define perms for access
#define PERMS 0666

int main() {

    // random time
    srand(time(NULL));

    // declaring each of the semaphores
    int lock = semget(IPC_PRIVATE, 1, PERMS | IPC_CREAT);
    int agent = semget(IPC_PRIVATE, 1, PERMS | IPC_CREAT);
    int paper = semget(IPC_PRIVATE, 1, PERMS | IPC_CREAT);
    int tobacco = semget(IPC_PRIVATE, 1, PERMS | IPC_CREAT);
    int match = semget(IPC_PRIVATE, 1, PERMS | IPC_CREAT);

    // initializing each of the semaphores
    sem_create(lock, 1);
    sem_create(agent, 0);
    sem_create(paper, 0);
    sem_create(tobacco, 0);
    sem_create(match, 0);

    // forks the paper process
    int paper1 = fork();

    if (paper1 == 0) {
        int count = 0; //to count the steps till 10
        while (count < 10) {
            P(paper);
            P(lock);
            printf("Paper picked up match and tobacco\n");
            V(agent);
            V(lock);
            printf("Paper rolled up a cigarette\n");
            printf("\n");
            count++;
        }
    }

    // forks the tobacco process
    int tobacco1 = fork();

        if (tobacco1 == 0) {
        int count = 0; //to count the steps till 10
        while (count < 10) {
            P(tobacco);
            P(lock);
            printf("Tobacco picked up match and paper\n");
            V(agent);
            V(lock);
            printf("Tobacco rolled up a cigarette\n");
            printf("\n");
            count++; //keep the count going
        }
    }

    // forks the match process
    int match1 = fork();

    if (match1 == 0) {
        int count = 0; //to count the steps till 10
        while (count < 10) {
            P(match);
            P(lock);
            printf("Match picked up tobacco and paper\n");
            V(agent);
            V(lock);
            printf("Match rolled up a cigarette\n");
            printf("\n");
            count++;
        }
    }

  // agent process
    int count = 0;
    if (fork() == 0) {
        while(count < 10) {
            P(lock);
            // random number to pick smoker
            int random = rand() % 3; 
            // If random == 0 then it wakes up paper 
            if (random == 0) {
                printf("The agent put match and tobacco on the table. So Paper gets activated\n");
                V(paper);
            }
            // If random == 1 then it wakes up tobacco 
            else if (random == 1) {
                printf("The agent put match and paper on the table. So Tobacco gets activated\n");
                V(tobacco);
            }
            // If random == 2 then it wakes up match 
            else if (random == 2) {
                printf("The agent put tobacco and paper on the table. So Match gets activated\n");
                V(match);
            }
            V(lock);
            P(agent);
            count++;
        }

        // kill the processes
        kill(paper1, SIGTERM);
        kill(tobacco1, SIGTERM);
        kill(match1, SIGTERM);

        printf("No More Smoking!!\n");
    }

    wait(NULL);
    return 0;

}