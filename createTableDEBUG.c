#include "mytar.h"
#define PERM_STRING 10

void createTable(char *pathNames[], int file, int argc, int v, int S){
    u_int8_t i = 0, byte = 0;
    unsigned int off;
    ssize_t bytesRead;
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

  printf("Debug: Inside createTable function\n");


    /*check if any paths were entered in command line*/
    if(pathNames == NULL){
        while(1){
            /*read bytes into header array*/
            bytesRead = read(file, extractedHeader, BLOCK_SIZE);
            if (bytesRead == -1){
                    perror("cannot read header");
                    exit(EXIT_FAILURE);
            }


            if (bytesRead == BLOCK_SIZE\
                && strncmp(extractedHeader + 257, "ustar", 5) == 0) {
                /*nothing*/
            } else {
                close(file);
             perror("file not valid");
                exit(EXIT_FAILURE);
            }

        h = extractHeader(extractedHeader);
              printf("Debug: Header information - name: %s, mode: %s,  size: %s\n", h.name, h.mode, h.size);
    /*check for S*/
            if (S){
                if (h.magic[HD_MAGIC - 1] != '\0'\
                    || strcmp((h.version),"00") != 0){
                    return;
                }
            }
            else{
                if (memcmp(h.magic, "ustar", 5) != 0){
                        printf("FAIL 1\n");
                        return;
                }
            }
            /*convert strings to ints*/
            intChksum = strtol(h.chksum, &endptr, 8);
            if (*endptr != '\0'){
                        printf("FAIL 2\n");
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            /*end loop if 2 null bloxks*/
            if (intChksum == 0){
                nullBlocks++;
          printf("Debug: Null blocks encountered: %d\n", nullBlocks);

                if (nullBlocks == 2){
                    break;
                }
            }
            /* fill fullName combining offsets of name and prefix*/
            for(off=(unsigned char)OFF_NAME;\
                off<(unsigned char)(OFF_NAME + HD_NAME);off++){
            if(extractedHeader[off] == '\0'){
                break;
                }
                fullName[byte] = extractedHeader[off];
                byte++;
            }
            fullName[byte] = '/';
            byte++;
            for(off=(unsigned char)OFF_PREFIX;\
                off<(unsigned char)(OFF_PREFIX+HD_PREFIX);off++){
                if(extractedHeader[off] == '\0'){
                    break;
                }
                fullName[byte] = extractedHeader[off];
                byte++;
            }
            if (off == (unsigned char)(OFF_PREFIX + HD_PREFIX)){
                fullName[PATH_MAX] = '\0';
            }
            if (v){
                /*convert permissions*/
                intMode = strtol(h.mode, &endptr, 8);
                        printf("if (v) statement\n");
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
                    printf("%s%s %s/%s %s %s %s\n", typeFlag,\
                    permString, h.uid, h.gid, h.size, h.mtime, fullName);
                }
                else{
                    /*use uname and gname*/
                    printf("%s%s %s/%s %s %s %s\n", typeFlag,\
                    permString, h.uname, h.gname, h.size, h.mtime, fullName);
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
            i = BLOCK_SIZE * (intSize / BLOCK_SIZE);
            lseek(file, i, SEEK_CUR);
        }
    }
    else{
        while(1){
            /*read bytes into header array*/
                        printf("Inside while (line ~150)\n");
            bytesRead = read(file, extractedHeader, BLOCK_SIZE);
            if (bytesRead == -1){
                    perror("cannot read header");
                    exit(EXIT_FAILURE);
            }
printf("Extracted Header Content: %.*s\n", BLOCK_SIZE, extractedHeader);
printf("bytesRead: %zd  BLOCK_SIZE: %d\n", bytesRead, BLOCK_SIZE);

int ustarCheck = strncmp(extractedHeader + 257, "ustar", 5);
printf("strncmp: %d\n", ustarCheck);

for (int i = 0; i < 10; ++i) {
    printf("Char at %d: %c (ASCII: %d)\n", i + 257, extractedHeader[i + 257], extractedHeader[i + 257]);
}

if (ustarCheck != 0) {
    close(file);
    perror("file not validi");
    exit(EXIT_FAILURE);
}

            if (bytesRead != BLOCK_SIZE\
                || strncmp(extractedHeader + 257, "ustar", 5) != 0) {

                close(file);
    printf("Extracted Header Content: %.*s\n", BLOCK_SIZE, extractedHeader);
printf("bytesRead: %zd  BLOCK_SIZE: %d", bytesRead, BLOCK_SIZE);
printf("\nstrncmp: %d\n", strncmp(extractedHeader + 257, "ustar", 5) == 0);

printf("\n");
                perror("file not valid");
                exit(EXIT_FAILURE);
            }

            h = extractHeader(extractedHeader);
      printf("Debug: Header information - name: %s, mode: %s,    size: %s\n", h.name, h.mode, h.size);
printf("Debug: Header information - magic: %s, version: %s\n", h.magic, h.version);

                        printf("checking for S..\n");
           /*check for S*/
            if (S){
                if (h.magic[HD_MAGIC - 1] != '\0'\
                    || strcmp((h.version),"00") != 0){

                        printf("FAIL MAGIC\n");
                        return;
                }
            }
            else{
                if (memcmp(h.magic, "ustar", 5) != 0){

                        printf("FAIL USTAR\n");
                        return;
                }
            }
                printf("line ~180\n");
            /*convert chksum to an int and check for last 2 null blocks*/
            intChksum = strtol(h.chksum, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            if (intChksum == 0){
                nullBlocks++;
      printf("Debug: Null blocks encountered: %d\n", nullBlocks);
                if (nullBlocks == 2){
                    break;
                }
            }
            /* fill fullName combining offsets of name and prefix*/
            for(off=(unsigned char)OFF_NAME;\
                off<(unsigned char)(OFF_NAME + HD_NAME);off++){
            if(extractedHeader[off] == '\0'){
                break;
                }
                fullName[byte] = extractedHeader[off];
                byte++;
            }
            fullName[byte] = '/';
            byte++;
            for(off=(unsigned char)OFF_PREFIX;\
                off < (unsigned char)(OFF_PREFIX + HD_PREFIX); off++){
                if(extractedHeader[off] == '\0'){
                    break;
                }
                fullName[byte] = extractedHeader[off];
                byte++;
            }
            byte = 0;
            if (off == (unsigned char)(OFF_PREFIX + HD_PREFIX)){
                fullName[PATH_MAX] = '\0';
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
            i = BLOCK_SIZE * (intSize / BLOCK_SIZE);
            lseek(file, i, SEEK_CUR);
        }
    }
    printf("End reached!\n");
    return;
}

                                                                                                      