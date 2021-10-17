#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

void mychmod(int who, int operation, int r, int w, int x, char* file_name) {

    struct stat buff;
    stat(file_name, &buff);
    mode_t past_mode;
    past_mode = buff.st_mode;

    if (r == 1) {
        switch (who) {
            case 1:
                if (operation == 1) {
                    chmod(file_name, past_mode | S_IRUSR);
                    past_mode = past_mode | S_IRUSR;
                }
                else if (operation == 2) {
                    chmod(file_name, past_mode ^ S_IRUSR);
                    past_mode = past_mode ^ S_IRUSR;
                }
                break;
            case 2:
                if (operation == 1) {
                    chmod(file_name, past_mode | S_IRGRP);
                    past_mode = past_mode | S_IRGRP;
                }
                else if (operation == 2) {
                    chmod(file_name, past_mode ^ S_IRGRP);
                    past_mode = past_mode ^ S_IRGRP;
                }
                break;
            case 3:
                if (operation == 1) {
                    chmod(file_name, past_mode | S_IROTH);
                    past_mode = past_mode | S_IROTH;
                }
                else if (operation == 2) {
                    chmod(file_name, past_mode ^ S_IROTH);
                    past_mode = past_mode ^ S_IROTH;
                }
                break;
        }
    }

    if (w == 1) {
        switch (who) {
            case 1:
                if (operation == 1) {
                    chmod(file_name, past_mode | S_IWUSR);
                    past_mode = past_mode | S_IWUSR;
                }
                else if (operation == 2) {
                    chmod(file_name, past_mode ^ S_IWUSR);
                    past_mode = past_mode ^ S_IWUSR;
                }
                break;
            case 2:
                if (operation == 1) {
                    chmod(file_name, past_mode | S_IWGRP);
                    past_mode = past_mode | S_IWGRP;
                }
                else if (operation == 2) {
                    chmod(file_name, past_mode ^ S_IWGRP);
                    past_mode = past_mode ^ S_IWGRP;
                }
                break;
            case 3:
                if (operation == 1) {
                    chmod(file_name, past_mode | S_IWOTH);
                    past_mode = past_mode | S_IWOTH;
                }
                else if (operation == 2) {
                    chmod(file_name, past_mode ^ S_IWOTH);
                    past_mode = past_mode ^ S_IWOTH;
                }
                break;
        }
    }

    if (x == 1) {
        switch (who) {
            case 1:
                if (operation == 1) {
                    chmod(file_name, past_mode | S_IXUSR);
                    past_mode = past_mode | S_IXUSR;
                }
                else if (operation == 2) {
                    chmod(file_name, past_mode ^ S_IXUSR);
                    past_mode = past_mode ^ S_IXUSR;
                }
                break;
            case 2:
                if (operation == 1) {
                    chmod(file_name, past_mode | S_IXGRP);
                    past_mode = past_mode | S_IXGRP;
                }
                else if (operation == 2) {
                    chmod(file_name, past_mode ^ S_IXGRP);
                    past_mode = past_mode ^ S_IXGRP;
                }
                break;
            case 3:
                if (operation == 1) {
                    chmod(file_name, past_mode | S_IXOTH);
                    past_mode = past_mode | S_IXOTH;
                }
                else if (operation == 2) {
                    chmod(file_name, past_mode ^ S_IXOTH);
                    past_mode = past_mode ^ S_IXOTH;
                }
                break;
        }
    }
}

int main(int argc, char** argv) {

    int opt;
    char* file_name;
    int who = -1;
    int operation = -1;
    int modes[3] = {-1, -1, -1};

    if (argc < 3) {
        perror("Incorrect input\n");
        exit(1);
    }
    else {
        file_name = argv[2];
    }

    while ((opt = getopt(argc, argv, "ugoarwx")) != -1) {
        switch (opt) {
            case 'u':
                who = 1;
                break;
            case 'g':
                who = 2;
                break;
            case 'o':
                who = 3;
                break;
            case 'a':
                operation = 1;
                break;
            case 'r':
                if (operation == -1) {
                    operation = 2;
                }
                else if ((operation == 1 || operation == 2) && modes[0] == -1) {
                    modes[0] = 1;
                }
                else {
                    perror("Incorrect input\n");
                    exit(1);
                }
                break;
            case 'w':
                modes[1] = 1;
                break;
            case 'x':
                modes[2] = 1;
                break;
            case '?':
                perror("Incorrect input\n");
                exit(1);
            }
    }

    if (who != -1 && operation != -1 && (modes[0] != -1 || modes[1] != -1 || modes[2] != -1) && file_name != NULL) {
        mychmod(who, operation, modes[0], modes[1], modes[2], file_name);
    }
    else {
        perror("Incorrect input\n");
        exit(1);
    }

    return 0;
}
