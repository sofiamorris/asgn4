#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <utime.h>
#include <sys/fcntl.h>
#include <dirent.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <pwd.h>
#include <grp.h>

#ifndef PATH_MAX
#define PATH_MAX 256
#endif
#define BLOCK_SIZE 512

/*Field Definitions (No magic number here!)*/
#define HD_NAME 100
#define HD_MODE 8
#define HD_UID 8
#define HD_GID 8
#define HD_SIZE 12
#define HD_MTIME 12
#define HD_CHKSUM 8
#define HD_TYPEFLAG 1
#define HD_LINKNAME 100
#define HD_MAGIC 6
#define HD_VERSION 2
#define HD_UNAME 32
#define HD_GNAME 32
#define HD_DEVMAJOR 8
#define HD_DEVMINOR 8
#define HD_PREFIX 155
#define HD_PADDING 12

#define OFF_NAME 0
#define OFF_MODE 100
#define OFF_UID 108
#define OFF_GID 116
#define OFF_SIZE 124
#define OFF_MTIME 136
#define OFF_CHKSUM 148
#define OFF_TYPEFLAG 156
#define OFF_LINKNAME 157
#define OFF_MAGIC 257
#define OFF_VERSION 263
#define OFF_UNAME 265
#define OFF_GNAME 297
#define OFF_DEVMAJOR 329
#define OFF_DEVMINOR 337
#define OFF_PREFIX 345

#define ALLMODE 777

typedef struct __attribute__ ((__packed__)) header_st{
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char padding[12];
}header;

int main(int argc, char *argv[]);
void createArchive(char *pathNames[], int file, int argc,  int v, int S);
void writeDir(char *path, int file, int v, int S);
void writeFile(char *path, int file, int v, int S);
void writeSym(char *path, int file, int v, int S);
void createTable(char *pathNames[], int file, int argc, int v, int S);
void extractArchive(char *pathNames[], int file, int argc, int v, int S);
void extractFile(char *name, int blocks,\
 mode_t perms, int file, time_t mtime, int size, int v);
void extractDir(char *name, mode_t perms, time_t mtime, int v);
void extractSymLink(char *name, char *linkname, time_t mtime, int v);
void makeDirHier(char *path);
size_t fillZeros(int fileSize, char *filePtr);
header makeHeader(char name[], struct stat fileStat,\
char typeflag, const char * symlink, int size, int S);
header extractHeader(char *extractedHeader);
ssize_t readlink(const char *path, char *buf, size_t bufsiz);
int symlink(const char *target, const char *linkpath);
int insert_special_int(char *where, size_t size, int32_t val);
