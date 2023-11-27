#include "mytar.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <utime.h>
#include <sys/time.h>

void extractSymLink(char *name, char *linkname, time_t mtime, int v){
    char copy[PATH_MAX];

    strcpy(copy,name); 
    makeDirHier(copy);
    if (symlink(linkname, name) == -1){
        perror("Error creating symlink");
        exit(EXIT_FAILURE);
    }
    if (v){
        printf("%s\n", name);
    }
    return;
}
