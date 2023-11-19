#include "mytar.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <utime.h>

void extractDir(char *name, mode_t perms, time_t mtime){
    struct utimbuf times;

    if (mkdir(name, perms) == -1){
        perror("Error creating directory");
        exit(EXIT_FAILURE);
    }
    times.modtime = mtime;
    if (utime(name, &times) == -1) {
        perror("error setting mtime");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", name);
    return;
}