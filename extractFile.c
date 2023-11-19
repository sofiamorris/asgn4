#include "mytar.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <utime.h>

void extractFile(char *name,int blocks,mode_t perms,int file,time_t mtime){
    u_int8_t off;
    char dataBlock[BLOCK_SIZE];
    int extractedFile;
    ssize_t bytesRead;
    struct utimbuf times;

    /*open new file and give it calculated perms*/
    if((extractedFile = open(name,O_WRONLY | O_CREAT | O_TRUNC, perms))==-1){
        perror("unable to create new file");
        exit(EXIT_FAILURE);
    }
    /*write data to new file*/
    while ((bytesRead = read(file, dataBlock, BLOCK_SIZE) > 0)){
        if (bytesRead == -1){
                perror("cannot read file");
                exit(EXIT_FAILURE);
        }
        if (write(extractedFile, dataBlock, bytesRead) == -1){
            perror("cannot write to extracted file");
            exit(EXIT_FAILURE);
        }
    }
    close(extractedFile);
    times.modtime = mtime;
    if (utime(extractedFile, &times) == -1) {
        perror("error setting mtime");
        exit(EXIT_FAILURE);
    }
    return;
}