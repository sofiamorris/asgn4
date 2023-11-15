#include <stdio.h>
#include <dirent.h>
#include "mytar.h"

void writeDir(char *path){
    DIR *dir;
    struct dirent *ent;

    if(dir = opendir(path) == NULL){
        perror("can't open directory");
    }
    else{
        while((ent = readdir(dir)) != NULL){
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }
            /*write header and dataBlock*/
            /*get next path somehow*/
            if (ent->d_type == DT_DIR) {
                writeDir(/*next path*/);
            }
            else {
                writeFile(/*next path*/);
            }
        } 
    }
}