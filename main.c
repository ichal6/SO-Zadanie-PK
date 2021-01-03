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
static void ustaw_semafor(void);
static void semafor_p(int);
static void semafor_v(int);
static void usun_semafor(void);

int const count = 3;

int main(){
    poczatek();
    utworz_nowy_semafor();

    char cmd1[20];
    char cmd2[20];
    
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
                if(execl(cmd1, cmd2, NULL) == -1){
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
    semafor=semget(1001, 5, 0600|IPC_CREAT);
    if(semafor==-1){
        perror("Nie moglem utworzyc nowego semafora.");
        exit(EXIT_FAILURE);
    } else{
        printf("Semafor zostal utworzony: %d\n", semafor);
    }  
}

static void ustaw_semafor(void){
    int ustaw_sem;
    ustaw_sem = semctl(semafor, 0, SETVAL, 0);
    if(ustaw_sem == -1){
        perror("Nie mozna ustawic semafora.");
        exit(EXIT_FAILURE);
    } else{
        printf("Semafor zostal ustawiony.\n");
    }
}
static void semafor_p(int nr){
    int zmien_sem;
    struct sembuf bufor_sem;
    bufor_sem.sem_num = nr;
    bufor_sem.sem_op = -1;
    bufor_sem.sem_flg = SEM_UNDO;
    zmien_sem = semop(semafor, &bufor_sem, 1);
    if(zmien_sem == -1){
        perror("Nie moglem zamknac seamfora");
        exit(EXIT_FAILURE);
    } else{
        printf("Semafor zostal zamkniety.\n");
    }
}
static void semafor_v(int nr){
    int zmien_sem;
    struct sembuf bufor_sem;
    bufor_sem.sem_num = nr;
    bufor_sem.sem_op = 1;
    bufor_sem.sem_flg = SEM_UNDO;
    zmien_sem = semop(semafor, &bufor_sem, 1);
    if(zmien_sem == -1){
        perror("Nie moglem otworzyc seamfora");
        exit(EXIT_FAILURE);
    } else{
        printf("Semafor zostal otwarty.\n");
    }
}
static void usun_semafor(void){
    int sem;
    sem = semctl(semafor, 0, IPC_RMID);

    if(sem==-1){
        perror("Nie mozna usunac seamfora.");
        exit(EXIT_FAILURE);
    } else{
        printf("Seamfor zostal usuniety: %d\n", sem);
    }
}