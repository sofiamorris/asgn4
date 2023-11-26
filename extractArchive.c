#include "mytar.h"


void extractArchive(char *pathNames[], int file, int argc, int v, int S){
    char extractedHeader[BLOCK_SIZE];
    header h;
    char *endptr;
    char fullName[PATH_MAX], parentPath[PATH_MAX];
    long intSize, intMode, intMtime;
    ssize_t bytesRead;
    int nullBlocks = 0;
    int reachedPath = 0;
    int pathIt, numBlocks;
    u_int32_t i = 0;
    mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    
    if (pathNames[0] == NULL){
        while(1){
            /*read bytes into header array*/
            bytesRead = read(file, extractedHeader, BLOCK_SIZE);
            if (bytesRead == -1){
                    perror("cannot read header");
                    exit(EXIT_FAILURE);
            }
            if (extractedHeader[0] != '\0'){
                if (bytesRead == BLOCK_SIZE\
                    && strncmp(extractedHeader + 257, "ustar", 5) == 0) {
                    /*nothing*/
                } else {
                    close(file);
                    perror("file not valid");
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
            /* convert strings to ints*/
            intSize = strtol(h.size, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            intMode = strtol(h.mode, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            intMtime = strtol(h.mtime, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            /* fill fullName combining offsets of name and prefix*/
            strcpy(fullName, h.name);
            if (h.prefix[0] != '\0'){
                strcat(fullName,"/");
                strcat(fullName, h.prefix);
            }
            /*check type, if directory, then create directory*/
            if (extractedHeader[OFF_TYPEFLAG] == '5'){
                perms = perms | (S_IXUSR | S_IXGRP | S_IXOTH);
                perms = perms & ~umask(0);
                perms = perms | (intMode & (ALLMODE));
                extractDir(fullName, perms, (time_t)intMtime, v);
            }
            /*if file, then create file*/
            else if (extractedHeader[OFF_TYPEFLAG] =='0'\
             || extractedHeader[OFF_TYPEFLAG] =='\0'){
                if (intMode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
                    perms = perms | (S_IXUSR | S_IXGRP | S_IXOTH);
                }
                perms = perms & ~umask(0);
                perms = perms | (intMode & (ALLMODE));
                numBlocks = intSize / BLOCK_SIZE;
                extractFile(fullName, numBlocks,\
                 perms, file, (time_t)intMtime, intSize, v);
            }
            /*if symlink, then create symlink*/
            else if (extractedHeader[OFF_TYPEFLAG] == '2'){
                extractSymLink(fullName, h.linkname, (time_t)intMtime, v);
            }
            else{
                perror("file type unidentifiable");
                exit(EXIT_FAILURE);
            }
        }
    } 
    else{
        while(1){
            long position = lseek(file, 0, SEEK_CUR);
            /*read bytes into header array*/
            bytesRead = read(file, extractedHeader, BLOCK_SIZE);
            if (bytesRead == -1){
                    perror("cannot read header");
                    exit(EXIT_FAILURE);
            }
            position = lseek(file, 0, SEEK_CUR);                 
            if (extractedHeader[0] != '\0'){
                if (bytesRead == BLOCK_SIZE\
                    && strncmp(extractedHeader + 257, "ustar", 5) == 0) {
                    /*nothing*/
                } else {
                    close(file);
                    perror("file not valid");
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
            /* convert strings to ints*/
            intSize = strtol(h.size, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            intMode = strtol(h.mode, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            intMtime = strtol(h.mtime, &endptr, 8);
            if (*endptr != '\0'){
                perror("error converting size to int");
                exit(EXIT_FAILURE);
            }
            /* fill fullName combining offsets of name and prefix*/
            strcpy(fullName, h.name);                                 
            if (h.prefix[0] != '\0'){                                 
                 strcat(fullName,"/");                                 
                 strcat(fullName, h.prefix);                           
             }  
            /*iterate through list of pathNames and check if path is reached*/
            for(pathIt = 0; pathIt < argc; pathIt++){
                if (strcmp(fullName, pathNames[pathIt]) == 0){
                    reachedPath = 1;
                    strcpy(parentPath, fullName);
                    break;
                }
            }
            /*if reachedPath, start extracting directory and descendents*/
            if(reachedPath){
                /*check if a descendent of directory, else reachedPath = 0*/
                if(strstr(fullName, parentPath) == NULL){
                    reachedPath = 0;
                    i = BLOCK_SIZE *( (intSize + BLOCK_SIZE - 1) / BLOCK_SIZE);
                    lseek(file, i, SEEK_CUR);
                }
                else{
                    /*check type, if directory, then create directory*/
                    if (extractedHeader[OFF_TYPEFLAG] == '5'){
                        perms = perms | (S_IXUSR | S_IXGRP | S_IXOTH);
                        perms = perms & ~umask(0);
                        perms = perms | (intMode & (ALLMODE));
                        extractDir(fullName, perms, (time_t)intMtime, v);
                    }
                    /*if file, then create file*/
                    else if (extractedHeader[OFF_TYPEFLAG] =='0'\
                    || extractedHeader[OFF_TYPEFLAG] =='\0'){
                        if (intMode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
                            perms = perms | (S_IXUSR | S_IXGRP | S_IXOTH);
                        }
                        perms = perms & ~umask(0);
                        perms = perms | (intMode & (ALLMODE));
                        numBlocks = intSize / BLOCK_SIZE;
                        extractFile(fullName,numBlocks,perms,\
                        file,(time_t)intMtime, intSize, v);
                    }
                    /*if symlink, then create symlink*/
                    else if (extractedHeader[OFF_TYPEFLAG] == '2'){
                        extractSymLink(fullName, h.linkname,\
                            (time_t)intMtime, v);
                    }
                    else{
                        perror("file type unidentifiable");
                    }
                }
            }
            i = BLOCK_SIZE *( (intSize + BLOCK_SIZE - 1) / BLOCK_SIZE);
            lseek(file, i, SEEK_CUR); 
        }
    }
    return;
}
