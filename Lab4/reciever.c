#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/sem.h>

int shmid;

typedef struct data {
    time_t time;
    pid_t pid;
} data;

struct sembuf sem_lock = {0, -1, 0}, sem_open = {0, 1, 0};

void myexit() {
    printf("Reciever done\n");
    struct shmid_ds buf;
    shmctl(shmid, IPC_RMID, &buf);
}

int main(int argc, char** argv) {
    signal(SIGINT, myexit);
    key_t key = ftok("file", 5);
    
    int semid;
    
    if ((shmid = shmget(key, sizeof(data), 0666)) < 0) {
        printf("Shmget error: %s\n", strerror(errno));
        exit(0);
    }
    
    if ((semid = semget(key, sizeof(data), 0666)) < 0) {
        printf("Semget error: %s\n", strerror(errno));
        exit(0);
    }
    
    char* at = shmat(shmid, NULL, 0);
    if (at < 0) {
        printf("Shmat error: %s\n", strerror(errno));
        exit(0);
    }
    
    semop(semid, &sem_open, 1);
    time_t cur_time = time(0);
    printf("Reciever time = %s", ctime(&cur_time));
    printf("Reciever pid = %d\n", getpid());
    printf("%s", at);
    semop(semid, &sem_lock, 1);
    
    shmdt(at);
    return 0;
}
