#include <string.h>                                                   
#include "mytar.h"
#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <utime.h>

void extractFile(char *name, int blocks,\
 mode_t perms, int file, time_t mtime, int size, int v){
    char dataBlock[BLOCK_SIZE];
    int extractedFile, numBlocks;
    ssize_t bytesRead;
    struct utimbuf times;
    int timesRead = 0;
    char copy[PATH_MAX];

    strcpy(copy,name); 
    makeDirHier(copy);
    /*open new file and give it calculated perms*/
    if((extractedFile = open(name,O_WRONLY | O_CREAT | O_TRUNC, perms))==-1){
        perror("unable to create new file");
        exit(EXIT_FAILURE);
    }
    numBlocks = (size / BLOCK_SIZE);
    /*write data to new file*/
    while ((bytesRead = read(file, dataBlock, BLOCK_SIZE)) > 0){
        if (bytesRead == -1){
                perror("cannot read file");
                exit(EXIT_FAILURE);
        }
        if (size < BLOCK_SIZE){
            bytesRead = size;
        }
        else{
            size = size - bytesRead;
        }
        if (write(extractedFile, dataBlock, bytesRead) == -1){
            perror("cannot write to extracted file");
            exit(EXIT_FAILURE);
        }
        timesRead++;
        if (timesRead > numBlocks){
            break;
        }
    }
    times.modtime = mtime;
    if (utime(name, &times) == -1) {
        perror("error setting mtime");
        exit(EXIT_FAILURE);
    }
    if (v){
        printf("%s\n", name);
    }
    close(extractedFile);
    return;
}
