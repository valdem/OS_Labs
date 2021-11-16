#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct data {
    time_t time;
    pid_t pid;
} data;

int main(int argc, char** argv) {
    int status;
    int fd[2];
    pid_t pid;
    pipe(fd);
    
    switch (pid = fork()) {
        case -1:
            perror("Error fork\n");
            return -1;
        case 0:
            printf("[CHILD]\n");
            data rec_data;
            close(fd[1]);
            read(fd[0], &rec_data, sizeof(data));
            close(fd[0]);
            sleep(5);
            time_t rec_curtime = time(0);
            printf("Current time: %s", ctime(&rec_curtime));
            printf("Sent time: %s", ctime(&rec_data.time));
            printf("Sent pid: %d\n", rec_data.pid);
            return 0;
        default:
            printf("[PARENT]\n");
            close(fd[0]);
            time_t send_curtime = time(0);
            pid_t send_pid = getpid();
            data send_data = {send_curtime, send_pid};
            write(fd[1], &send_data, sizeof(data));
            close(fd[1]);
            printf("Data was sent\n");
            wait(&status);
            return 0;
    }
    return 0;
}
