#include "mytar.h"

void writeFile(char *path, int file, int v, int S){
    int fd; 
    ssize_t bytesRead;
    char buffer[BLOCK_SIZE];
    char buf[BLOCK_SIZE];
    int size = 0;
    header h;
    struct stat fileStat;

    if ((fd = open(path, O_RDONLY, S_IRUSR)) == -1){
        perror("file can't be opened");
        return;
    }
    /*get size of file for header*/
    while ((bytesRead = read(fd, buffer, BLOCK_SIZE)) > 0){
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
        return;
    }
    /*write header for file*/
    h = makeHeader(path, fileStat, '0', "", size, S);
    if (write(file, &h, BLOCK_SIZE) == -1){
        perror("cannot write header");
        close(fd);
        return;  
    }
    /*write dataBlock*/
    while((bytesRead = read(fd, buf, BLOCK_SIZE)) > 0){
        if (bytesRead == -1){
            perror("cannot read byte in file");
            return;
        }
        /*add padding to buffer*/
        fillZeros(bytesRead, buf);
        if (write(file, buf, BLOCK_SIZE) == -1){
            perror("cannot write file");
            close(fd);
            return;
        }
    }
    if(v){
        printf("%s\n", path);
    }
    close(fd);
    return;
}
