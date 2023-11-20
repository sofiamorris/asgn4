#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "mytar.h"

void createTable(char *pathNames, int file, int argc){
    u_int8_t i = 0, byte = 0, off;
    ssize_t bytesRead;
    char extractedHeader[BLOCK_SIZE];
    header h;
    char fullName[PATH_MAX];
    char *endptr;
    long intSize, intChksum;
    int nullBlocks = 0;
    int reachedPath = 0;
    int pathIt;
    char pathEnd[PATH_MAX];

    /*check if any paths were entered in command line*/
    if(strcmp(pathNames, "") == 0){
        while(1){
            /*read bytes into header array*/
            while ((bytesRead = read(file, extractedHeader, BLOCK_SIZE) > 0)){
                if (bytesRead == -1){
                        perror("cannot read header");
                        exit(EXIT_FAILURE);
                }
            }
            h = extractHeader(extractedHeader);
            /*convert strings to ints*/
            intChksum = strtol(h.chksum, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            /*end loop if 2 null bloxks*/
            if (intChksum == 0){
                nullBlocks++;
                if (nullBlocks == 2){
                    break;
                }
            }
            /* fill fullName combining offsets of name and prefix*/
            for(off = OFF_NAME; off < HD_NAME; off++){
            if(extractedHeader[off] == '\0'){
                break;
                }
                fullName[byte] = extractedHeader[off];
                byte++;
            }
            fullName[byte] = '/';
            byte++;
            for(off = OFF_PREFIX; off < HD_PREFIX; off++){
                if(extractedHeader[off] == '\0'){
                    break;
                }
                fullName[byte] = extractedHeader[off];
                byte++;
            }
            byte = 0;   
            if (v){
                /*create string consisting of permissions,
                 owner/group, size, and Mtime using offsets*/
                printf("%s %s/%s %s %s %s %s\n",\
                     h.mode, h.uid, h.gid, h.size, h.mtime, fullName);
            }
            else{
                /*read path name and print it*/
                printf("%s\n", fullName);
            }
            intSize = strtol(h.size, &endptr, 8);
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
            while ((bytesRead = read(file, extractedHeader, BLOCK_SIZE) > 0)){
                if (bytesRead == -1){
                        perror("cannot read header");
                        exit(EXIT_FAILURE);
                }
            }
            h = extractHeader(extractedHeader);
            intChksum = strtol(h.chksum, &endptr, 8);
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
            if(extractedHeader[off] == '\0'){
                break;
                }
                fullName[byte] = extractedHeader[off];
                byte++;
            }
            fullName[byte] = '/';
            byte++;
            for(off = OFF_PREFIX; off < HD_PREFIX; off++){
                if(extractedHeader[off] == '\0'){
                    break;
                }
                fullName[byte] = extractedHeader[off];
                byte++;
            }
            byte = 0;       
            /*iterate through list of pathNames and check if path is reached*/
            for(pathIt = 0; pathIt < argc; pathIt++){
                if (strcmp(fullName, pathNames[pathIt]) == 0){
                    reachedPath = 1;
                    break;
                }
            }
            /*if reachedPath, start printing directory and descendents*/
            if(reachedPath){
                /*check if a descendent of directory, else reachedPath = 0*/
                if(strstr(fullName, pathNames[pathIt]) == NULL){
                    reachedPath = 0;
                }
                else{
                    if (v){
                        /*create string consisting of permissions,
                        owner/group, size, and Mtime using offsets*/
                        printf("%s %s/%s %s %s %s %s\n",\
                        h.mode, h.uid, h.gid, h.size, h.mtime, fullName);
                    }
                    else{
                        /*read path name and print it*/
                        printf("%s\n", fullName);
                    }
                }
            }
            intSize = strtol(h.size, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            /*read SIZE and increase i accordingly*/
            i = BLOCK_SIZE + (BLOCK_SIZE * (intSize / BLOCK_SIZE));
            lseek(file, i, SEEK_CUR);
        }
    }
    return;
}