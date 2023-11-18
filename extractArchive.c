#include "mytar.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

void extractArchive(char *pathNames, int file){
    u_int8_t i = 0, byte = 0, off;
    ssize_t bytesRead;
    char header[BLOCK_SIZE];
    char dataBlock[BLOCK_SIZE];
    char *endptr;
    char chksum[HD_CHKSUM];
    char size[HD_SIZE], mode[HD_MODE], uid[HD_UID], gid[HD_GID];
    char fullName[PATH_MAX];
    long intSize, intChksum, intMode, int;
    int nullBlocks = 0;
    int reachedPath = 0;
    int pathIt, numBlocks;
    char pathEnd[PATH_MAX];
    int extractedFile;
    
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
            for(off = OFF_SIZE; off < HD_SIZE; off++){
                size[byte] = header[off];
                byte++;
            }
            intSize = strtol(size, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            /*check type, if file then copy, else go to next header*/
            if (header[OFF_TYPEFLAG] == '0' || header[OFF_TYPEFLAG] == '\0'){
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
                /*get permissions and owners*/
                for(off = OFF_MODE; off < HD_MODE; off++){
                    mode[byte] = header[off];
                    byte++;
                }
                byte = 0;
                intMode = strtol(mode, &endptr, 8);
                if (*endptr != '\0'){
                    perror("error converting size to int");
                    exit(EXIT_FAILURE);
                }
               /* for(off = OFF_UID; off < HD_UID; off++){
                    uid[byte] = header[off];
                    byte++;
                }
                byte = 0;
                for(off = OFF_GID; off < HD_GID; off++){
                    gid[byte] = header[off];
                    byte++;
                }*/
                /*open new file and give same attributes as archived file*/
                if ((extractedFile = open(fullName, /*permissions, owners*/, intMode)) == -1){
                    perror("unable to create new file");
                    exit(EXIT_FAILURE);
                }
                numBlocks = intSize / BLOCK_SIZE;
                while ((bytesRead = read(file, dataBlock, BLOCK_SIZE) > 0)){
                    if (bytesRead == -1){
                            perror("cannot read file");
                            exit(EXIT_FAILURE);
                    }
                    if (write(extractedFile, dataBlock, bytesRead) == -1){
                        perror("cannot write to extracted file");
                        return;
                    }
                }
            }
            else{
                i = BLOCK_SIZE + (BLOCK_SIZE * (intSize / BLOCK_SIZE));
                lseek(file, i, SEEK_CUR);
            }
        }
    } 
    else{

    }
}