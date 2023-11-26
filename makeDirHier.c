#include "mytar.h"

void makeDirHier(char *path){
    char *token;
    char *currentPath;
    char temp[PATH_MAX];
    DIR *dir;

    /*split path up by "/" and check if paths exist*/
    token = strtok(path, "/");
    if(token == NULL){
        perror("No path provided");
        exit(EXIT_FAILURE);
    }
    currentPath = token;
    while(token != NULL){
        /*if path does not exist, make directory and get next path*/
        if((dir = opendir(currentPath)) == NULL){
            if ((token = strtok(NULL, "/")) == NULL){
                return;
            }
            mkdir(currentPath, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP\
             | S_IROTH | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH);
            strncpy(temp, currentPath, PATH_MAX);
            snprintf(currentPath, PATH_MAX, "%s/%s", temp, token);
        }
        else{
            return;
        }
    }
    return;
}
