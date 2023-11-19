#include "mytar.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <utime.h>

void extractSymLink(char *name, char *linkname, time_t mtime){
    struct utimbuf times;

    if (symlink(name, linkname) == -1){
        perror("Error creating symlink");
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