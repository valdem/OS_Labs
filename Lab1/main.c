#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void handle_exit() {
    printf("Finished %d\n", getpid());
}

int main(int argc, char** argv) {
    int status;
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
            printf("Parent child_pid = %d\n", pid);
            printf("Parent pid = %d\n", getpid());
            printf("Parent ppid = %d\n", getppid());
            wait(&status);
            printf("Exit status: %d\n", WEXITSTATUS(status));
            break;
    }

    return 0;
}
