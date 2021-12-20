#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int id;

typedef struct data {
    time_t time;
    pid_t pid;
} data;

void myexit() {
    printf("Sender done\n");
    struct shmid_ds buf;
    shmctl(id, IPC_RMID, &buf);
}

int main(int argc, char** argv) {
    signal(SIGINT, myexit);
    key_t key = ftok("file", 5);
    
    if ((id = shmget(key, sizeof(data), IPC_CREAT | 0666)) < 0) {
        printf("Shmget error: %s\n", strerror(errno));
        exit(0);
    }
    
    struct shmid_ds buf;
    shmctl(id, IPC_STAT, &buf);
    
    if (buf.shm_nattch > 0) {
        printf("Sender already exists\n");
        exit(0);
    }
    
    char* at = shmat(id, NULL, 0);
    if (at < 0) {
        printf("Shmat error: %s\n", strerror(errno));
        exit(0);
    }
    
    time_t cur_time = time(0);
    time_t buf_time = cur_time;
    
    while (1) {
        cur_time = time(0);
        if (cur_time != buf_time) {
            buf_time = cur_time;
            sleep(3);
            sprintf(at, "Sender time = %sSender pid = %d\n", ctime(&cur_time), getpid());
        }
        
    }
    
    shmdt(at);
    return 0;
}
