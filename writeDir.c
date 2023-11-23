#include "mytar.h"

void writeDir(char *path, int file, int v, int S){
    DIR *dir;
    struct dirent *ent;
    char currentPath[PATH_MAX];
    struct stat dirStat, fileStat;
    header h;

    if(stat(path, &dirStat) == -1){
        perror("error stating dir");
    }
    /*write header for directory*/
    h = makeHeader(path, dirStat, '5', "", 0, S);
    if (write(file, &h, BLOCK_SIZE) == -1){
        perror("cannot write header");
    }
    if((dir = opendir(path)) == NULL){
        perror("can't open directory");
    }
    else{
        /*iterate through objects coming out of directory*/
        while((ent = readdir(dir)) != NULL){
            if (strcmp(ent->d_name, ".") == 0 ||\
                strcmp(ent->d_name, "..") == 0) {
                continue;
            }
            /*get path of object*/
            snprintf(currentPath, PATH_MAX, "%s/%s", path, ent->d_name);
            /*check object pointed to by directory and call write*/
            if(stat(currentPath, &fileStat) == 0){
                if(S_ISDIR(fileStat.st_mode)){
                    writeDir(currentPath, file, v, S);
                }
                else if(S_ISREG(fileStat.st_mode)){
                    writeFile(currentPath, file, v, S);
                }
                else if (S_ISLNK(fileStat.st_mode)){
                    writeSym(currentPath, file, v, S);
                }
                else{
                    makeDirHier(currentPath);
                    writeDir(currentPath, file, v, S);
                }
            }
        }
    }
    closedir(dir);
    return;
}
