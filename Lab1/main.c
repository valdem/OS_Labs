#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handle_exit() {
    printf("Finished %d\n", getpid());
}

int main(int argc, char** argv) {
    pid_t pid = fork();
    atexit(handle_exit);
    
    switch (pid) {
        case -1:
            perror("no pid");
            break;
        case 0:
            printf("Child pid = %d\n", getpid());
            printf("Child ppid = %d\n", getppid());
            break;
        default:
            wait(NULL);
            printf("Parent pid = %d\n", getpid());
            printf("Parent ppid = %d\n", getppid());
            break;
    }
    
    return 0;
}
