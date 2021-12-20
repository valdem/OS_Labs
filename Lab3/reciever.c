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
    printf("Reciever done\n");
    struct shmid_ds buf;
    shmctl(id, IPC_RMID, &buf);
}

int main(int argc, char** argv) {
    signal(SIGINT, myexit);
    key_t key = ftok("file", 5);
    
    
    if ((id = shmget(key, sizeof(data), 0666)) < 0) {
        printf("Shmget error: %s\n", strerror(errno));
        exit(0);
    }
    
    char* at = shmat(id, NULL, 0);
    if (at < 0) {
        printf("Shmat error: %s\n", strerror(errno));
        exit(0);
    }
    
    time_t cur_time = time(0);
    printf("Reciever time = %s", ctime(&cur_time));
    printf("Reciever pid = %d\n", getpid());
    printf("%s", at);
    
    shmdt(at);
    return 0;
}
