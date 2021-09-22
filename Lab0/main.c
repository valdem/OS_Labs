#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char** argv) {
    DIR* dir;
	struct dirent* dirent;
	struct stat st;
	struct passwd* pwd;
	struct group* grp;	
	
	char path[512];
	char buf[512];

	int opt, l_flag;
    
    while ((opt = getopt(argc, argv, ":l")) != -1) {
        switch (opt) {
            case 'l':
                l_flag = 1;
            break;
        }
    }
    
	if (l_flag == 1) {
        if (argc < 3) {
            strcpy(path, ".");
        }
        else {
            strcpy(path, argv[2]);
        }
        dir = opendir(path);
        if (!dir) {
            perror("no such directory");
            return -1;
        }
        int blocks;
                        
        while((dirent = readdir(dir)) != NULL) {
            sprintf(buf, "%s/%s", path, dirent->d_name);
            stat(buf, &st);
            blocks+=st.st_blocks;
                
            switch (st.st_mode & S_IFMT) {
                case S_IFBLK:  printf("b"); break;
                case S_IFCHR:  printf("c"); break;
                case S_IFDIR:  printf("d"); break;
                case S_IFIFO:  printf("p"); break;
                case S_IFLNK:  printf("l"); break;
                case S_IFSOCK: printf("s"); break;
                default:       printf("-"); break;
            }
                    
            printf((st.st_mode & S_IRUSR) ? "r" : "-");
            printf((st.st_mode & S_IWUSR) ? "w" : "-");
            printf((st.st_mode & S_IXUSR) ? "x" : "-");
            printf((st.st_mode & S_IRGRP) ? "r" : "-");
            printf((st.st_mode & S_IWGRP) ? "w" : "-");
            printf((st.st_mode & S_IXGRP) ? "x" : "-");
            printf((st.st_mode & S_IROTH) ? "r" : "-");
            printf((st.st_mode & S_IWOTH) ? "w" : "-");
            printf((st.st_mode & S_IXOTH) ? "x" : "-");
                
            printf(" %-7d", st.st_nlink);

            if ((pwd = getpwuid(st.st_uid)) == 0) {
                printf("  ");
            }
            else {
                printf(" %-10s", pwd->pw_name);
            }
             
            grp = getgrgid(st.st_gid);
            printf(" %-10s", grp->gr_name);

            printf(" %-10lld", st.st_size);
					
            char* ct = ctime(&st.st_mtime);
            ct[strcspn(ct, "\n")] = '\0';
            printf("%-20s", ct);
            printf(" %-10s\n", dirent->d_name);
        }
        printf("total %d \n", blocks);
        closedir(dir);
    }
	else {
		if (argc < 2) {
            strcpy(path, ".");
        }
        else {
            strcpy(path, argv[1]);
        }
	
        dir = opendir(path);
		if (!dir) {
			perror("no such directory");
			return -1;
		}
        while((dirent = readdir(dir)) != NULL) {
            printf("%s\n", dirent->d_name);
        }
		closedir(dir);	
	}
	return 0;
}
