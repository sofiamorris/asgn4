#include "mytar.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <utime.h>

void extractFile(char *name, int blocks,\
 mode_t perms, int file, time_t mtime, int size){
    char dataBlock[BLOCK_SIZE];
    int extractedFile, numBlocks;
    ssize_t bytesRead;
    struct utimbuf times;
    char stringFile[PATH_MAX];
    int timesRead = 0;

    /*open new file and give it calculated perms*/
    if((extractedFile = open(name,O_WRONLY | O_CREAT | O_TRUNC, perms))==-1){
        perror("unable to create new file");
        exit(EXIT_FAILURE);
    }
    numBlocks = (size / BLOCK_SIZE);
    /*write data to new file*/
    while ((bytesRead = read(file, dataBlock, BLOCK_SIZE)) > 0\
     && timesRead <= numBlocks ){
        if (bytesRead == -1){
                perror("cannot read file");
                exit(EXIT_FAILURE);
        }
        if (write(extractedFile, dataBlock, bytesRead) == -1){
            perror("cannot write to extracted file");
            exit(EXIT_FAILURE);
        }
    }
    sprintf(stringFile, "%d", extractedFile);
    times.modtime = mtime;
    if (utime(stringFile, &times) == -1) {
        perror("error setting mtime");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", name);
    close(extractedFile);
    return;
}
