#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>

struct file {
    char* name;
    int size_name;
    char* content;
    int size_content;
};

void input(char *arch_name, char *file_name) {
    FILE *in, *archive;
    struct file infile;
    
    infile.name = file_name;
    infile.size_name = strlen(file_name);
    
    int count = 0;
    
    if ((archive = fopen(arch_name,"r")) != NULL) {
            fscanf(archive, "%d", &count);
            fgetc(archive);
    }
    
    int size_name[count];
    int size_content[count];
    char* file_names[count];
    char* file_content[count];
    
    if (count != 0) {
        for (int i = 0; i < count; i++){
            fscanf(archive, "%d", &size_name[i]);
            fgetc(archive);
            fscanf(archive, "%d", &size_content[i]);
            fgetc(archive);
            file_names[i] = malloc(size_name[i]);
            fread(file_names[i], 1, size_name[i], archive);
            fgetc(archive);
            file_content[i] = malloc(size_content[i]);
            fread(file_content[i], 1, size_content[i], archive);
            fgetc(archive);
        }
    }
    fclose(archive);
    
    if ((in = fopen(file_name, "r")) != NULL) {
        fseek (in, 0, SEEK_END);
        infile.size_content = ftell(in);
        fseek (in, 0, SEEK_SET);
        infile.content = malloc(infile.size_content);
        fread(infile.content, 1, infile.size_content, in);
        fclose(in);
    }
    else {
        perror("No such file");
        exit(1);
    }
    
    if ((archive = fopen(arch_name, "w")) != NULL) {
        count++;
        fprintf(archive, "%d\n", count);
        
        if (count != 1) {
            for (int i = 0; i < count-1; i++){
                fprintf(archive, "%d\n", size_name[i]);
                fprintf(archive, "%d\n", size_content[i]);
                fwrite(file_names[i], 1, size_name[i], archive);
                fputc('\n', archive);
                fwrite(file_content[i], 1, size_content[i], archive);
                fputc('\n', archive);
            }
        }
        
        fprintf(archive, "%d\n", infile.size_name);
        fprintf(archive, "%d\n", infile.size_content);
        fprintf(archive, "%s\n", infile.name);
        fwrite(infile.content, 1, infile.size_content, archive);
        fputc('\n', archive);
        
        fclose(archive);
        
        for (int i = 0; i < count-1; i++) {
                free(file_content[i]);
                free(file_names[i]);
        }
        free(infile.content);
    }
    else {
        perror("No such archive");
        exit(1);
    }
    
}

void extract(char *arch_name, char *file_name) {
    
}

void stat(char *arch_name) {
    FILE *archive;
    
    int count = 0;
    
    if ((archive = fopen(arch_name,"r")) != NULL) {
            fscanf(archive, "%d", &count);
            fgetc(archive);
    }
    else {
        perror("No such archive");
        exit(1);
    }
    
    int size_name[count];
    int size_content[count];
    char* file_names[count];
    char* file_content[count];
    
    for (int i = 0; i < count; i++){
        fscanf(archive, "%d", &size_name[i]);
        fgetc(archive);
        fscanf(archive, "%d", &size_content[i]);
        fgetc(archive);
        file_names[i] = malloc(size_name[i]);
        fread(file_names[i], 1, size_name[i], archive);
        fgetc(archive);
        file_content[i] = malloc(size_content[i]);
        fread(file_content[i], 1, size_content[i], archive);
        fgetc(archive);
    }
    
    fclose(archive);
    
    int arch_size;
    
    if ((archive = fopen(arch_name, "r")) != NULL) {
        fseek (archive, 0, SEEK_END);
        arch_size = ftell(archive);
        fseek (archive, 0, SEEK_SET);
        
        printf("Archive - %s\n", arch_name);
        printf("Size of archive = %d\n", arch_size);
        printf("Files and sizes: \n");
        for (int i = 0; i < count;  i++) {
            printf("%s ", file_names[i]);
            printf("%d\n", size_content[i]);
        }
        
        fclose(archive);
    }
    
    for (int i = 0; i < count-1; i++) {
            free(file_content[i]);
            free(file_names[i]);
    }
    
}


int main(int argc, char** argv) {
    int opt;
    char *arch_name, *file_name;
    
    while ((opt = getopt(argc, argv, "i:e:s:h")) != -1) {
        switch (opt) {
            case 'i':
                if (argc < 4) {
                    perror("Incorrect input. Print -h to info.\n");
                    exit(1);
                    break;
                }
                arch_name = argv[1];
                file_name = argv[3];
                input(arch_name, file_name);
                break;
            case 'e':
                if (argc < 4) {
                    perror("Incorrect input. Print -h to info.\n");
                    exit(1);
                    break;
                }
                arch_name = argv[1];
                file_name = argv[3];
                extract(arch_name, file_name);
                break;
            case 's':
                if (argc < 3) {
                    perror("Incorrect input. Print -h to info.\n");
                    exit(1);
                    break;
                }
                arch_name = argv[2];
                stat(arch_name);
                break;
            case 'h':
                printf("Print ./archiver archive_name -i file_name to input file to archive\n");
                printf("Print ./archiver archive_name -e file_name to extract file from archive\n");
                printf("Print ./archiver archive_name -s file_name to get info about archive\n");
                break;
            case '?':
                perror("Incorrect flag. Print -h to info.\n");
                exit(1);
                break;
            default:
                printf("%s\n", strerror(errno));
                break;
        }
    }
    
    return 0;
}
