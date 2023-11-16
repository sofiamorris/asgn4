#include "mytar.h"

void writeDir(char *path, int file){
    DIR *dir;
    struct dirent *ent;
    char currentPath[PATH_MAX];
    struct stat fileStat;
    header h;
    
    /*write header for directory*/
    h = makeHeader(/*parameters*/);
    if (write(file, h, BLOCK_SIZE)){
        perror("cannot write header");
    }
    if(dir = opendir(path) == NULL){
        perror("can't open directory");
    }
    else{
        /*iterate through objects coming out of directory*/
        while((ent = readdir(dir)) != NULL){
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }
            /*get path of object*/
            snprintf(currentPath, PATH_MAX, "%s/%s", path, ent->d_name);
            /*check object pointed to by directory and call write*/
            if(stat(currentPath, &fileStat) == 0){
                if(S_ISDIR(fileStat.st_mode)){
                    writeDir(currentPath, file);
                }
                else if(S_ISREG(fileStat.st_mode)){
                    writeFile(currentPath, file);
                }
                else if (S_ISLNK(fileStat.st_mode)){
                    writeSym(currentPath, file);
                }
                else{
                    perror("path does not exist");
                }
            }
        }
    }
    close(dir);
}
