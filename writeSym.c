#include "mytar.h"

void writeSym(char *path, int file){
    header h; 
    ssize_t size, bytes;
    char *newPath;
    struct stat fileStat;

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
    /*get info on link and and write header*/
    if(stat(newPath, &fileStat) == 0){
        /*write the header to the file*/
        h = makeHeader(path, fileStat, '2', newPath);
        if (write(file, &h, BLOCK_SIZE) == -1){
            perror("cannot write header");
        }
    }
    free(newPath);
    return;
}