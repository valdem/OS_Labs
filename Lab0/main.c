#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char* argv[]) {
	DIR* dir;
	struct dirent* dirent;
	struct stat st;
	struct passwd* pwd;
	struct group* grp;	
	
	char path[512];
	char buf[512];

	int opt;
	
	if ((opt = getopt(argc, argv, "l")) != -1) {
		switch(opt) {
			case 'l': 
				if (argc < 3) {
                                	strcpy(path, ".");
                        	}
                        	else {  
                                	strcpy(path, argv[2]);
                        	}

				dir = opendir(path);

				while((dirent = readdir(dir)) != NULL) {
                			sprintf(buf, "%s/%s", path, dirent->d_name);
                			stat(buf, &st);

                			printf("%s\n", dirent->d_name);

                			printf((st.st_mode & S_IRUSR) ? "r" : "-");
                			printf((st.st_mode & S_IWUSR) ? "w" : "-");
                			printf((st.st_mode & S_IXUSR) ? "x" : "-");
                			printf((st.st_mode & S_IRGRP) ? "r" : "-");
                			printf((st.st_mode & S_IWGRP) ? "w" : "-");
                			printf((st.st_mode & S_IXGRP) ? "x" : "-");
                			printf((st.st_mode & S_IROTH) ? "r" : "-");
                			printf((st.st_mode & S_IWOTH) ? "w" : "-");
                			printf((st.st_mode & S_IXOTH) ? "x" : "-");

                			printf(" %d", st.st_nlink);

                			pwd = getpwuid(st.st_uid);
                			printf(" %s", pwd->pw_name);

                			grp = getgrgid(st.st_gid);
                			printf(" %s", grp->gr_name);

                			printf(" %lld", st.st_size);
                			printf(" %s", ctime(&st.st_mtime));
        			
				}
				closedir(dir);
			break;
		}
	}
	else {
                if (argc < 2) {
                	strcpy(path, ".");
                }
                else {
                	strcpy(path, argv[1]);
                }

                        dir = opendir(path);

                        while((dirent = readdir(dir)) != NULL) {
        	                printf("%s\n", dirent->d_name);
                        }
	}
}
