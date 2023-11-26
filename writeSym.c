#include "mytar.h"

void writeSym(char *path, int file, int v, int S){
    header h; 
    ssize_t size, bytes;
    char *newPath;
    struct stat fileStat;
    char symPath[PATH_MAX];

    /*get size of symlink*/
    if ((size = readlink(path, symPath, PATH_MAX)) == -1){
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
        h = makeHeader(path, fileStat, '2', newPath, 0, S);
        if (write(file, &h, BLOCK_SIZE) == -1){
            perror("cannot write header");
        }
    }
    if(v){
        printf("%s\n", path);
    }
    free(newPath);
    return;
}
