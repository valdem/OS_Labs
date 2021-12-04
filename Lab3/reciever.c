#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct data {
    time_t time;
    pid_t pid;
} data;

int main(int argc, char** argv) {
    key_t key = ftok("file", 5);
    
    int id;
    char* at;
    
    if ((id = shmget(key, sizeof(data), 0666)) < 0) {
        printf("Shmget error: %s\n", strerror(errno));
        exit(0);
    }
    
    if ((at = shmat(id, NULL, 0)) < 0) {
        printf("Shmat error: %s\n", strerror(errno));
        exit(0);
    }
    
    time_t cur_time = time(0);
    printf("Reciever time = %s", ctime(&cur_time));
    printf("Reciever pid = %d\n", getpid());
    printf("%s", at);
    
    return 0;
}
