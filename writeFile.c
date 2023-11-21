#include "mytar.h"

void writeFile(char *path, int file){
    int fd; 
    ssize_t bytesRead;
    char buffer[BLOCK_SIZE];
    int size = 0;
    header h;
    struct stat fileStat;

    if ((fd = open(path, O_RDONLY, S_IRUSR)) == -1){
        perror("file can't be opened");
        return;
    }
    /*get size of file for header*/
    while ((bytesRead = read(fd, buffer, BLOCK_SIZE) > 0)){
        if (bytesRead == -1){
            perror("cannot read byte in file");
            return;
        }
        size+=bytesRead;
    }
    if (lseek(fd, 0, SEEK_SET) == -1){
        perror("lseek error");
        return;
    }
    if(stat(path, &fileStat) == -1){
        perror("error stating dir");
    }
    /*write header for file*/
    h = makeHeader(path, fileStat, '0', "");
    if (write(file, &h, BLOCK_SIZE) == -1){
        perror("cannot write header");
        close(fd);
        return;
    }
    /*write dataBlock*/
    while ((bytesRead = read(fd, buffer, BLOCK_SIZE) > 0)){
        if (bytesRead == -1){
            perror("cannot read byte in file");
            return;
        }
        /*add padding to buffer*/
        fillZeros(h.size, buffer);
        if (write(file, buffer, bytesRead) == -1){
            perror("cannot write file");
            close(fd);
            return;
        }
    if(v){
        fprintf("%s\n", path);
    }
    close(fd);
    }
    return;
}