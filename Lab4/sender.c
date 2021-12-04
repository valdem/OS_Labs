#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/sem.h>

typedef struct data {
    time_t time;
    pid_t pid;
} data;

struct sembuf sem_lock = {0, -1, 0}, sem_open = {0, 1, 0};

int main(int argc, char** argv) {
    key_t key = ftok("file", 5);
    
    int shmid, semid;
    char* at;
    
    if ((shmid = shmget(key, sizeof(data), IPC_CREAT | 0666)) < 0) {
        printf("Shmget error: %s\n", strerror(errno));
        exit(0);
    }
    
    if ((semid = semget(key, sizeof(data), IPC_CREAT | 0666)) < 0) {
        printf("Semget error: %s\n", strerror(errno));
        exit(0);
    }
    
    if ((at = shmat(shmid, NULL, 0)) < 0) {
        printf("Shmat error: %s\n", strerror(errno));
        exit(0);
    }
    
    if (strlen(at) != 0) {
        printf("Sender already exists\n");
        exit(0);
    }
    
    time_t cur_time = time(0);
    time_t buf_time = cur_time;
    
    while (1) {
        cur_time = time(0);
        if (cur_time != buf_time) {
            semop(semid, &sem_open, 1);
            buf_time = cur_time;
            sleep(3);
            sprintf(at, "Sender time = %sSender pid = %d\n", ctime(&cur_time), getpid());
            semop(semid, &sem_lock, 1);
        }
    }
    return 0;
}