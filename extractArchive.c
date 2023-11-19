#include "mytar.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <utime.h>

void extractArchive(char *pathNames, int file){
    u_int8_t i = 0, byte = 0, off;
    char header[BLOCK_SIZE];
    char *endptr;
    char chksum[HD_CHKSUM], mtime[HD_MTIME], linkname[HD_LINKNAME];
    char size[HD_SIZE], mode[HD_MODE], uid[HD_UID], gid[HD_GID];
    char fullName[PATH_MAX];
    long intSize, intChksum, intMode, intMtime;
    int nullBlocks = 0;
    int reachedPath = 0;
    int pathIt, numBlocks;
    char pathEnd[PATH_MAX];
    mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    
    if (strcmp(pathNames, "") == 0){
        while(1){
            /*read bytes into header array*/
            while ((bytesRead = read(file, header, BLOCK_SIZE) > 0)){
                if (bytesRead == -1){
                        perror("cannot read header");
                        exit(EXIT_FAILURE);
                }
            }
            /*use offset to check if checksum is 0, if so increase nullCount*/
            /*if 2 blocks of '\0', break*/
            for(off = OFF_CHKSUM; off < HD_CHKSUM; off++){
                chksum[byte] = header[off];
                byte++;
            }
            byte = 0;
            intChksum = strtol(chksum, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            if (intChksum == 0){
                nullBlocks++;
                if (nullBlocks == 2){
                    break;
                }
            }
            /*get size of dataBlock from header*/
            for(off = OFF_SIZE; off < HD_SIZE; off++){
                size[byte] = header[off];
                byte++;
            }
            intSize = strtol(size, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            /* fill fullName combining offsets of name and prefix*/
            for(off = OFF_NAME; off < HD_NAME; off++){
            if(header[off] == '\0'){
                break;
                }
                fullName[byte] = header[off];
                byte++;
            }
            fullName[byte] = '/';
            byte++;
            for(off = OFF_PREFIX; off < HD_PREFIX; off++){
                if(header[off] == '\0'){
                    break;
                }
                fullName[byte] = header[off];
                byte++;
            }
            byte = 0;
            printf("%s\n", fullName);
            /*get permissions*/
            for(off = OFF_MODE; off < HD_MODE; off++){
                mode[byte] = header[off];
                byte++;
            }
            intMode = strtol(mode, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            /*get modification time*/
            for(off = OFF_MTIME; off < HD_MTIME; off++){
                mtime[byte] = header[off];
                byte++;
            }
            byte = 0;
            intMtime = strtol(mtime, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            /*check type, if directory, then create directory*/
            if (header[OFF_TYPEFLAG] == '5'){
                perms = perms | (S_IXUSR | S_IXGRP | S_IXOTH);
                perms = perms & ~umask(0);
                perms = perms | (mode & (ALLMODE));
                extractDir(fullName, perms, (time_t)intMtime);
            }
            /*if file, then create file*/
            else if (header[OFF_TYPEFLAG]=='0' || header[OFF_TYPEFLAG]=='\0'){
                if (intMode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
                    perms = perms | (S_IXUSR | S_IXGRP | S_IXOTH);
                }
                perms = perms & ~umask(0);
                perms = perms | (mode & (ALLMODE));
                numBlocks = intSize / BLOCK_SIZE;
                extractFile(fullName,numBlocks,perms,file,(time_t)intMtime);
            }
            /*if symlink, then create symlink*/
            else{
                /*get linkname from header*/
                for(off = OFF_LINKNAME; off < HD_LINKNAME; off++){
                    linkname[byte] = header[off];
                    byte++;
                }
                byte = 0;
                perms = perms & ~umask(0);
                perms = perms | (mode & (ALLMODE));
                extractSymLink(fullName, linkname, (time_t)intMtime);
            }
        }
    } 
    else{

    }
}