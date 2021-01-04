#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/errno.h>
#include <sys/wait.h>


int semaphoreId;

static void programDescription(void);
static void getSemaphore(void);
static void deleteSemaphore(void);
static void setSemaphore(void);

int const count = 3;
int const sizeOfSemafor = 5;
key_t keyForsemaphore;

int main(){
    programDescription();
    getSemaphore();
    setSemaphore();

    char pathToProgram[20];
    char firstArgument[20];
    char secondArgument[20];
    
    for(int i = 0; i < count; i++)
    {
        int statusFork = fork();
        switch(statusFork)
        {
            case -1:
                perror("Problem with fork");
                exit(1);
                break;
            case 0:
                sprintf(pathToProgram, "./P%d", i+1);
                sprintf(firstArgument, "P%d", i+1);
                sprintf(secondArgument, "%d", keyForsemaphore);
                if(execl(pathToProgram, firstArgument, secondArgument, NULL) == -1){
                    perror("Execl error");
                    exit(2);
                }
                break;
        }
    }

    for(int i = 0; i < count; i++)
    {
        int status;
        int pidWait = wait(&status);
        printf("Proces %d has ended with status: %d \n", pidWait, status);
    }

    deleteSemaphore();
}

static void programDescription(void){
    printf("Main program.\n");
}

static void getSemaphore(){
    if((keyForsemaphore = ftok(".",'Z')) == -1)
    {
        perror("Problem with generate a key!");
        exit(2);
    }
    semaphoreId=semget(keyForsemaphore, 5, 0600|IPC_CREAT);
    if(semaphoreId==-1){
        perror("Problem with create a semaphore.");
        exit(EXIT_FAILURE);
    } else{
        printf("Semaphore has created! Id - %d\n", semaphoreId);
    }  
}

static void deleteSemaphore(void){
    int sem;
    sem = semctl(semaphoreId, 0, IPC_RMID);

    if(sem==-1){
        perror("Problem with delete a semaphore.");
        exit(EXIT_FAILURE);
    } else{
        printf("Semaphore has deleted.\n");
    }
}

static void setSemaphore(void){
    int ustaw_sem;

    for(int i = 0; i < sizeOfSemafor; i++){
        ustaw_sem = semctl(semaphoreId, i, SETVAL, 0);
        if(ustaw_sem == -1){
            char errorMessage[100];
            sprintf(errorMessage, "Problem with set a semaphore - %d", i);
            perror(errorMessage);
            exit(EXIT_FAILURE);
        }
    }
}
