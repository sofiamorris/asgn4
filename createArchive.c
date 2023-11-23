#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mytar.h"

void createArchive(char *path, int file, int v, int S){
    struct stat fileStat;

    if(stat(path, &fileStat) == 0){
        if(S_ISDIR(fileStat.st_mode)){
            writeDir(path, file, v, S);
        }
        else if(S_ISREG(fileStat.st_mode)){
            writeFile(path, file, v, S);
        }
        else if (S_ISLNK(fileStat.st_mode)){
            writeSym(path, file, v, S);
        }
        else{
            makeDirHier(path);
        }
    }
}
