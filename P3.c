#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int semafor;
key_t keyForsemaphore;

static void utworz_nowy_semafor(void);
static void semafor_p(int);
static void semafor_v(int);

int main(int argc, char* argv[]){
    if(argc !=2){
        perror("Problem with size of list of arguments.");
        exit(2);
    }
    keyForsemaphore = atoi(argv[1]);
    printf("%s\n", argv[1]);
    utworz_nowy_semafor();

    semafor_p(0);

    printf("Sekcja t31 procesu o PID=%d\n", getpid());
    sleep(1);
    printf("Sekcja t32 procesu o PID=%d\n", getpid());
    sleep(1);

    semafor_v(1);
    semafor_p(4);

    printf("Sekcja t33 procesu o PID=%d\n", getpid());
    sleep(1);
}

static void utworz_nowy_semafor(){
    semafor=semget(keyForsemaphore, 5, 0600|IPC_CREAT);
    if(semafor==-1){
        perror("Nie moglem utworzyc nowego semafora.");
        exit(EXIT_FAILURE);
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
    }
}
