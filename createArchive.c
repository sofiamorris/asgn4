#include <stdio.h>
#include <dirent.h>
#include "mytar.h"

archive createArchive(char *path){
    header h;
    DIR *dir;

    if(dir = opendir(path) != NULL){
        closedir(dir);
        writeDir(path);
    }
    else{
        writeFile(path);
    }

}