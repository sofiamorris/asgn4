#include <stdio.h>
#include <sys/stat.h>
#include "mytar.h"

void writeSym(char *path, int file){
    header h; 
    ssize_t size, bytes;
    char *newPath;
    struct stat fileStat;

    /*write the header to the file*/
    h = makeHeader(/*parameters*/);
    if (write(file, h, BLOCK_SIZE)){
        perror("cannot write header");
    }
    /*get size of symlink*/
    if ((size = readlink(path, NULL, 0)) == -1){
        perror("error with symbolic link size");
        exit(EXIT_FAILURE);
    }
    if ((newPath = (char*)malloc(size)) == NULL){
        perror("Null pointer to new path for symlink");
        exit(EXIT_FAILURE);
    }
    /*read symlink into newPath*/
    if ((bytes = readlink(path, newPath, size)) == -1){
        perror("error reading symlink");
        free(newPath);
        exit(EXIT_FAILURE);
    }
    /*check object pointed to by symlink and call write*/
    if(stat(newPath, &fileStat) == 0){
        if(S_ISDIR(fileStat.st_mode)){
            writeDir(newPath, file);
        }
        else if(S_ISREG(fileStat.st_mode)){
            writeFile(newPath, file);
        }
        else if (S_ISLNK(fileStat.st_mode)){
            writeSym(newPath, file);
        }
        else{
            perror("path does not exist");
        }
    }
    free(newPath);
}