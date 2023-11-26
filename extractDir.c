#include "mytar.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <utime.h>
#define SYSTEMCALL 9

void extractDir(char *name, mode_t perms, time_t mtime, int v){
    struct utimbuf times;
    char command[SYSTEMCALL + PATH_MAX];
    char copy[PATH_MAX];

    strcpy(copy,name); 
    makeDirHier(copy);    
    if (access(name, F_OK) != -1) {
        snprintf(command, sizeof(command), "rm -r %s", name);
        if (system(command) == -1){
            perror("could not remove directory");
            return;
        }
    }   
    if (mkdir(name, perms) == -1){
        perror("Error creating directory");
        exit(EXIT_FAILURE);
    }
    times.modtime = mtime;
    if (utime(name, &times) == -1) {
        perror("error setting mtime");
        exit(EXIT_FAILURE);
    }
    if (v){
        printf("%s\n", name);
    }
    return;
}
