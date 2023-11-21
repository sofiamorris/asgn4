#include "mytar.h"

void makeDirHier(char *path){
    char *token;
    char *currentPath;
    DIR *dir;
    struct dirent *ent;

    /*split path up by "/" and check if paths exist*/
    token = strtok(path, '/');
    if(token == NULL){
        perror("No path provided");
        exit(EXIT_FAILURE);
    }
    currentPath = token;
    token = strok(NULL, '/');
    while(token != NULL){
        /*if path does not exist, make directory and get next path*/
        if((dir = opendir(currentPath)) == NULL){
            mkdir(currentPath, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP\
             | S_IROTH | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH);
            snprintf(currentPath, PATH_MAX, "%s/%s", currentPath, token);
            token = strtok(NULL, '/');
        }
        else{
            return;
        }
    }
    return;
}