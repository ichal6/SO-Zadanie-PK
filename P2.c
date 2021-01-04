#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int semaphoreId;
key_t keyForsemaphore;

static void getSemaphore(void);
static void semaphoreDown(int);
static void semaphoreUp(int);

int main(int argc, char* argv[]){
        if(argc !=2){
        perror("Problem with size of list of arguments.");
        exit(2);
    }
    keyForsemaphore = atoi(argv[1]);
    printf("%s\n", argv[1]);
    getSemaphore();

    printf("Sekcja t21 procesu o PID=%d\n", getpid());
    sleep(1);

    semaphoreUp(0);

    semaphoreDown(1);

    printf("Sekcja t22 procesu o PID=%d\n", getpid());
    sleep(1);

    semaphoreUp(2);

    semaphoreDown(3);

    printf("Sekcja t23 procesu o PID=%d\n", getpid());
    sleep(1);

    semaphoreUp(4);
}

static void getSemaphore(){
    semaphoreId=semget(keyForsemaphore, 5, 0400|IPC_CREAT);
    if(semaphoreId==-1){
        perror("Problem with create a semaphore.");
        exit(EXIT_FAILURE);
    }
}

static void semaphoreDown(int nr){
    int returnValue;
    struct sembuf semaphoreBufor;
    semaphoreBufor.sem_num = nr;
    semaphoreBufor.sem_op = -1;
    semaphoreBufor.sem_flg = SEM_UNDO;
    returnValue = semop(semaphoreId, &semaphoreBufor, 1);
    if(returnValue == -1){
        perror("Problem with close a semaphore.");
        exit(EXIT_FAILURE);
    }
}
static void semaphoreUp(int nr){
    int returnValue;
    struct sembuf semaphoreBufor;
    semaphoreBufor.sem_num = nr;
    semaphoreBufor.sem_op = 1;
    semaphoreBufor.sem_flg = SEM_UNDO;
    returnValue = semop(semaphoreId, &semaphoreBufor, 1);
    if(returnValue == -1){
        perror("Problem with close a semaphore.");
        exit(EXIT_FAILURE);
    }
}
