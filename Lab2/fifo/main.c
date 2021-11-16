#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct data {
    time_t time;
    pid_t pid;
} data;

int main(int argc, char** argv) {
    int status;
    pid_t pid;
    
    unlink("myfifo");
    if((mkfifo("myfifo", 0777)) == -1) {
        printf("Can't create FIFO");
        return -1;
    }
    switch (pid = fork()) {
        case -1:
            perror("Error fork\n");
            return -1;
        case 0:
            printf("[CHILD]\n");
            int read_fifo = open("myfifo", O_RDONLY);
            if (read_fifo < 0) {
                perror("Read fifo error\n");
                return -1;
            }
            data rec_data;
            read(read_fifo, &rec_data, sizeof(data));
            close(read_fifo);
            sleep(5);
            time_t rec_curtime = time(0);
            printf("Current time: %s", ctime(&rec_curtime));
            printf("Sent time: %s", ctime(&rec_data.time));
            printf("Sent pid: %d\n", rec_data.pid);
            return 0;
        default:
            printf("[PARENT]\n");
            int write_fifo = open("myfifo", O_WRONLY);
            if (write_fifo < 0) {
                perror("Write fifo error\n");
                return -1;
            }
            time_t send_curtime = time(0);
            pid_t send_pid = getpid();
            data send_data = {send_curtime, send_pid};
            write(write_fifo, &send_data, sizeof(data));
            close(write_fifo);
            printf("Data was sent\n");
            wait(&status);
            return 0;
    }
    return 0;
}
