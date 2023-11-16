#include "mytar.h"

void writeFile(char *path, int file){
    int fd; 
    ssize_t byte, i;
    char buffer[BLOCK_SIZE];
    int size;
    header h;

    if ((fd = open(path, O_RDONLY, S_IRUSR)) == -1){
        perror("file can't be read");
    }
    /*get size of file for header*/
    while ((byte = read(fd, buffer, BLOCK_SIZE) > 0)){
        if (byte == -1){
            perror("cannot read byte in file");
        }
        for (i = 0; i < byte; i++){
            size++;
        }
    }
    if (lseek(fd, 0, SEEK_SET) == -1){
        perror("lseek error");
    }
    /*write header for file*/
    h = makeHeader(/*parameters*/);
    if (write(file, h, BLOCK_SIZE)){
        perror("cannot write header");
    }
    /*write dataBlock*/
    if (write(file, fd, size) == -1){
        perror("cannot write file");
    }
    /*write padding here*/
    close(fd);
}
