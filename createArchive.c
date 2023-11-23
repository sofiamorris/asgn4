#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mytar.h"

void createArchive(char *pathNames[], int file, int argc, int v, int S){
    struct stat fileStat;
    int i;

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
                makeDirHier(pathNames[i]);
            }
        }
    }
}
