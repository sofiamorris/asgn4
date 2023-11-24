#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mytar.h"

void createArchive(char *pathNames[], int file, int argc, int v, int S){
    struct stat fileStat;
    int i;
    char nullBlocks[2 * BLOCK_SIZE] = {0};

    for(i = 0; i < argc; i++){
        if(stat(pathNames[i], &fileStat) == 0){
            if(S_ISDIR(fileStat.st_mode)){
                writeDir(pathNames[i], file, v, S);
            }
            else if(S_ISREG(fileStat.st_mode)){
                writeFile(pathNames[i], file, v, S);
            }
            else if (S_ISLNK(fileStat.st_mode)){
                writeSym(pathNames[i], file, v, S);
            }
            else{
                perror("path is not a supported type");
            }
        }
        else{
            perror("path does not exist");
        }
    }
    if (write(file, nullBlocks, 2 * BLOCK_SIZE) == -1) {
        perror("Error writing to file");
        close(file);
        exit(EXIT_FAILURE);
    }
}
