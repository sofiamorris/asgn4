#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "mytar.h"

void createTable(char *path, int file){
    header h;
    u_int8_t i = 0, byte = 0, off;
    ssize_t bytesRead;
    char header[BLOCK_SIZE];
    char *endptr;
    char chksum[HD_CHKSUM];
    char perm[HD_MODE], uid[HD_UID], gid[HD_GID];
    char size[HD_SIZE], mtime[HD_MTIME];
    char fullName[PATH_MAX];
    long intSize, intChksum;
    int nullBlocks = 0;
    int reachedPath = 0;
    int pathIt, endIt = 0;
    char pathEnd[PATH_MAX];

    if(strcmp(path, "") == 0){
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
            if (v){
                /*create string consisting of permissions,
                 owner/group, size, and Mtime using offsets*/
                for(off = OFF_MODE; off < HD_MODE; off++){
                    perm[byte] = header[off];
                    byte++;
                }
                byte = 0;
                for(off = OFF_UID; off < HD_UID; off++){
                    uid[byte] = header[off];
                    byte++;
                }
                byte = 0;
                for(off = OFF_GID; off < HD_GID; off++){
                    gid[byte] = header[off];
                    byte++;
                }
                byte = 0;
                for(off = OFF_SIZE; off < HD_SIZE; off++){
                    size[byte] = header[off];
                    byte++;
                }
                byte = 0;
                for(off = OFF_MTIME; off < HD_MTIME; off++){
                    mtime[byte] = header[off];
                    byte++;
                }
                byte = 0;
                printf("%s %s/%s %s %s %s %s\n",\
                     perm, uid, gid, size, mtime, fullName);
            }
            else{
                /*read path name and print it*/
                printf("%s\n", fullName);
            }
            intSize = strtol(size, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            /*read SIZE and increase i accordingly*/
            i = BLOCK_SIZE + (BLOCK_SIZE * (intSize / BLOCK_SIZE));
            lseek(file, i, SEEK_CUR);
        }

    }
    else{
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
            /*check if path is reached*/
            if (strcmp(fullName, path) == 0){
                reachedPath = 1;
            }
            /*if reachedPath, start printing directory and descendents*/
            if(reachedPath){
                /*check if a descendent of directory, break if not*/
                if(strstr(fullName, path) == NULL){
                    break;
                }
                if (v){
                    /*create string consisting of permissions,
                    owner/group, size, and Mtime using offsets*/
                    for(off = OFF_MODE; off < HD_MODE; off++){
                        perm[byte] = header[off];
                        byte++;
                    }
                    byte = 0;
                    for(off = OFF_UID; off < HD_UID; off++){
                        uid[byte] = header[off];
                        byte++;
                    }
                    byte = 0;
                    for(off = OFF_GID; off < HD_GID; off++){
                        gid[byte] = header[off];
                        byte++;
                    }
                    byte = 0;
                    for(off = OFF_SIZE; off < HD_SIZE; off++){
                        size[byte] = header[off];
                        byte++;
                    }
                    byte = 0;
                    for(off = OFF_MTIME; off < HD_MTIME; off++){
                        mtime[byte] = header[off];
                        byte++;
                    }
                    byte = 0;
                    printf("%s %s/%s %s %s %s %s\n",\
                         perm, uid, gid, size, mtime, fullName);
                }
                else{
                    /*read path name and print it*/
                    printf("%s\n", fullName);
                }
            }
            intSize = strtol(size, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            /*read SIZE and increase i accordingly*/
            i = BLOCK_SIZE + (BLOCK_SIZE * (intSize / BLOCK_SIZE));
            lseek(file, i, SEEK_CUR);
        }
    }
}