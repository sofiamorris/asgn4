#include <stdio.h>
#include "mytar.h"
#include <sys/fcntl.h>

void writeFile(char *path, int file){
    int fd; 
    ssize_t bytesRead;
    char buffer[BLOCK_SIZE];
    int size = 0;
    header h;

    if ((fd = open(path, O_RDONLY, S_IRUSR)) == -1){
        perror("file can't be read");
        return;
    }
    /*get size of file for header*/
    while ((bytesRead = read(fd, buffer, BLOCK_SIZE) > 0)){
        if (bytesRead == -1){
            perror("cannot read byte in file");
            return;
        }
        size+=bytesRead;
    }
    if (lseek(fd, 0, SEEK_SET) == -1){
        perror("lseek error");
        return;
    }
    /*write header for file*/
    h = makeHeader(/*parameters*/);
    if (write(file, &h, BLOCK_SIZE) == -1){
        perror("cannot write header");
        close(fd);
        return;
    }
    /*write dataBlock*/
    while ((bytesRead = read(fd, buffer, BLOCK_SIZE) > 0)){
        if (write(file, buffer, bytesRead) == -1){
            perror("cannot write file");
            close(fd);
            return;
        }
    /*write padding here*/
    if(v){
        fprintf("%s\n", path);
    }
    close(fd);
}