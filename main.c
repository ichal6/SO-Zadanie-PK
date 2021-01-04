#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/errno.h>
#include <sys/wait.h>


int semafor;

static void poczatek(void);
static void utworz_nowy_semafor(void);
static void usun_semafor(void);
static void ustaw_semafor(void);

int const count = 3;
key_t keyForsemaphore;

int main(){
    poczatek();
    utworz_nowy_semafor();
    ustaw_semafor();

    char cmd1[20];
    char cmd2[20];
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
                sprintf(cmd1, "./P%d", i+1);
                sprintf(cmd2, "P%d", i+1);
                sprintf(secondArgument, "%d", keyForsemaphore);
                if(execl(cmd1, cmd2, secondArgument, NULL) == -1){
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

    usun_semafor();
}

static void poczatek(void){
    printf("Main program.\n");
}

static void utworz_nowy_semafor(){
    if((keyForsemaphore = ftok(".",'Z')) == -1)
    {
        perror("Problem with generate a key!");
        exit(2);
    }
    semafor=semget(keyForsemaphore, 5, 0600|IPC_CREAT);
    if(semafor==-1){
        perror("Nie moglem utworzyc nowego semafora.");
        exit(EXIT_FAILURE);
    } else{
        printf("Semafor zostal utworzony: %d\n", semafor);
    }  
}

static void usun_semafor(void){
    int sem;
    sem = semctl(semafor, 0, IPC_RMID);

    if(sem==-1){
        perror("Nie mozna usunac seamfora.");
        exit(EXIT_FAILURE);
    } else{
        printf("Semafor zostal usuniety: %d\n", sem);
    }
}

static void ustaw_semafor(void){
    int ustaw_sem;
    const int sizeOfSemafor = 5;

    for(int i = 0; i < sizeOfSemafor; i++){
        ustaw_sem = semctl(semafor, i, SETVAL, 0);
        if(ustaw_sem == -1){
            perror("Nie mozna ustawic semafora.");
            exit(EXIT_FAILURE);
        }
    }
}
