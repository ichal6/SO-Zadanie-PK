#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int semafor;

static void poczatek(void);
static void utworz_nowy_semafor(void);
static void semafor_p(int);
static void semafor_v(int);

int main(){
    poczatek();
    utworz_nowy_semafor();

    printf("Sekcja t21 procesu o PID=%d\n", getpid());
    sleep(1);

    semafor_v(0);

    semafor_p(1);

    printf("Sekcja t22 procesu o PID=%d\n", getpid());
    sleep(1);

    semafor_v(2);

    semafor_p(3);

    printf("Sekcja t23 procesu o PID=%d\n", getpid());
    sleep(1);

    semafor_v(4);
}

static void poczatek(void){
    printf("Proces P2.\n");
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
