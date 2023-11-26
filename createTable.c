#include "mytar.h"
#define PERM_STRING 10

void createTable(char *pathNames[], int file, int argc, int v, int S){
    ssize_t bytesRead, i;
    char extractedHeader[BLOCK_SIZE];
    header h;
    char fullName[PATH_MAX + 1];
    char *endptr;
    long intSize, intChksum, intMode;
    int nullBlocks = 0;
    int reachedPath = 0;
    int pathIt, j, k, myPerms;
    char fullPerms[] = "rwx";
    char permString[PERM_STRING];
    char typeFlag[HD_TYPEFLAG + 1];

    /*check if any paths were entered in command line*/
             if (pathNames[0] == NULL){
        while(1){
            /*read bytes into header array*/
            bytesRead = read(file, extractedHeader, BLOCK_SIZE);
            if (bytesRead == -1){
                    perror("cannot read header");
                    exit(EXIT_FAILURE);
            }
            if (extractedHeader[0] != '\0'){
                if (bytesRead != BLOCK_SIZE\
                    || strncmp(extractedHeader + 257, "ustar", 5) != 0) {

printf("bytesRead(1): %zd\n", bytesRead);
printf("strcmp: %d\n", strncmp(extractedHeader + 257, "ustar", 5));
                    close(file);
                    perror("XXfile not valid");
                    exit(EXIT_FAILURE);
                }
                h = extractHeader(extractedHeader);
                if (S){
                    if ((h.magic)[HD_MAGIC - 1]!='\0'||\
                        strcmp(h.version,"00")!=0){
                        return;
                    }
                }
                else{
                    if (memcmp(h.magic, "ustar", 5) != 0){
                        return;
                    }
                }
            }
            if (extractedHeader[OFF_CHKSUM] == '\0'){
                nullBlocks++;
                if (nullBlocks == 2){
                    break;
                }
                continue;
            }
            /*convert strings to ints*/
            intChksum = strtol(h.chksum, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            /*end loop if 2 null blocks*/
            if (intChksum == 0){
                nullBlocks++;
                if (nullBlocks == 2){
                    break;
                }
            }
            /* fill fullName combining offsets of name and prefix*/
            if (h.prefix[0] != '\0'){
                 strcpy(fullName,h.prefix);
                 strcat(fullName,"/");
                 strcat(fullName, h.name);
            }
            else{
                strcpy(fullName, h.name);
            }
            if (v){
                /*convert permissions*/
                intMode = strtol(h.mode, &endptr, 8);
                if (*endptr != '\0'){
                    perror("error converting size to int");
                    exit(EXIT_FAILURE);
                }
                for (j = 2; j >= 0; j--){
                    myPerms = (intMode >> (j * 3)) & 0x7;
                    for (k = 2; k >= 0; k--){
                        permString[(2 - k) + ((2 - j) * 3)]\
                        = (myPerms & (1 << k)) ? fullPerms[k] : '-';
                    }
                }
                permString[PERM_STRING - 1] = '\0';
                if (strcmp(h.typeflag,"5") == 0){
                    typeFlag[0] = 'd';
                }
                else if (strcmp(h.typeflag,"2") == 0){
                    typeFlag[0] = 'l';
                }
                else{
                    typeFlag[0] = '-';
                }
                typeFlag[HD_TYPEFLAG] = '\0';
                /*create string consisting of permissions,
 *                 owner/group, size, and Mtime using offsets*/
                if(h.uname[0] == '\0' || h.gname[0] == '\0'){
                    /*use uid and gid if symbolic names unavailable*/
                    printf("%s%s %s/%s %d %s %s\n", typeFlag,\
                    permString, h.uid, h.gid, intSize, h.mtime, fullName);
                }
                else{
                    /*use uname and gname*/
                    printf("%s%s %s/%s %d %s %s\n", typeFlag,\
                    permString, h.uname, h.gname, intSize, h.mtime, fullName);
                }
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
            i = BLOCK_SIZE *( (intSize + BLOCK_SIZE - 1) / BLOCK_SIZE);
            lseek(file, i, SEEK_CUR);
        }
    }
    else{
        while(1){
            /*read bytes into header array*/
            bytesRead = read(file, extractedHeader, BLOCK_SIZE);
            if (bytesRead == -1){
                    perror("cannot read header");
                    exit(EXIT_FAILURE);
            }

            if (bytesRead != BLOCK_SIZE\
                || strncmp(extractedHeader + 257, "ustar", 5) != 0) {
                close(file);
printf("bytesRead(2): %zd\n", bytesRead);
                perror("file not valid");
                exit(EXIT_FAILURE);
            }
            h = extractHeader(extractedHeader);
            /*check for S*/
            if (S){
                if (h.magic[HD_MAGIC - 1] != '\0'\
                    || strcmp((h.version),"00") != 0){
                    return;
                }
            }
            else{
                if (memcmp(h.magic, "ustar", 5) != 0){
                    return;
                }
            }
            /*convert chksum to an int and check for last 2 null blocks*/
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
            if (h.prefix[0] != '\0'){
                 strcpy(fullName,h.prefix);
                 strcat(fullName,"/");
                 strcat(fullName, h.name);
            }
            else{
                strcpy(fullName, h.name);
            }
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
                        /*convert permissions*/
                        intMode = strtol(h.mode, &endptr, 8);
                        if (*endptr != '\0'){
                            perror("error converting size to int");
                            exit(EXIT_FAILURE);
                        }
                        for (j = 2; j >= 0; j--){
                            myPerms = (intMode >> (j * 3)) & 0x7;
                            for (k = 2; k >= 0; k--){
                                permString[(2 - k) + ((2 - j) * 3)]\
                                = (myPerms & (1 << k)) ? fullPerms[k] : '-';
                            }
                        }
                        permString[PERM_STRING - 1] = '\0';
                        if (strcmp(h.typeflag,"5") == 0){
                            typeFlag[0] = 'd';
                        }
                        else if (strcmp(h.typeflag,"2") == 0){
                            typeFlag[0] = 'l';
                        }
                        else{
                            typeFlag[0] = '-';
                        }
                        typeFlag[HD_TYPEFLAG] = '\0';
                        /*create string consisting of permissions,
 *                         owner/group, size, and Mtime using offsets*/
                        if(h.uname[0] == '\0' || h.gname[0] == '\0'){
                            /*use uid and gid if symbolic names unavailable*/
                            printf("%s%s %s/%s %d %s %s\n", typeFlag,\
                             permString, h.uid, h.gid,\
                              intSize, h.mtime, fullName);
                        }
                        else{
                            /*use uname and gname*/
                            printf("%s%s %s/%s %d %s %s\n", typeFlag,\
                            permString, h.uname, h.gname,\
                             intSize, h.mtime, fullName);
                        }
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
            if (intChksum == 0){
                nullBlocks++;
                if (nullBlocks == 2){
                    break;
                }
            }
            /* fill fullName combining offsets of name and prefix*/
            if (h.prefix[0] != '\0'){
                 strcpy(fullName,h.prefix);
                 strcat(fullName,"/");
                 strcat(fullName, h.name);
            }
            else{
                strcpy(fullName, h.name);
            }
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
                        /*convert permissions*/
                        intMode = strtol(h.mode, &endptr, 8);
                        if (*endptr != '\0'){
                            perror("error converting size to int");
                            exit(EXIT_FAILURE);
                        }
                        for (j = 2; j >= 0; j--){
                            myPerms = (intMode >> (j * 3)) & 0x7;
                            for (k = 2; k >= 0; k--){
                                permString[(2 - k) + ((2 - j) * 3)]\
                                = (myPerms & (1 << k)) ? fullPerms[k] : '-';
                            }
                        }
                        permString[PERM_STRING - 1] = '\0';
                        if (strcmp(h.typeflag,"5") == 0){
                            typeFlag[0] = 'd';
                        }
                        else if (strcmp(h.typeflag,"2") == 0){
                            typeFlag[0] = 'l';
                        }
                        else{
                            typeFlag[0] = '-';
                        }
                        typeFlag[HD_TYPEFLAG] = '\0';
                        /*create string consisting of permissions,
 *                         owner/group, size, and Mtime using offsets*/
                        if(h.uname[0] == '\0' || h.gname[0] == '\0'){
                            /*use uid and gid if symbolic names unavailable*/
                            printf("%s%s %s/%s %s %s %s\n", typeFlag,\
                             permString, h.uid, h.gid,\
                              h.size, h.mtime, fullName);
                        }
                        else{
                            /*use uname and gname*/
                            printf("%s%s %s/%s %s %s %s\n", typeFlag,\
                            permString, h.uname, h.gname,\
                             h.size, h.mtime, fullName);
                        }
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
            i = BLOCK_SIZE *( (intSize + BLOCK_SIZE - 1) / BLOCK_SIZE);
            i += BLOCK_SIZE;          /*Account for header size */
             lseek(file, i, SEEK_CUR);
        }
    }
    return;
}

