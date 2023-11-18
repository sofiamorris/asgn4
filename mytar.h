#include <stdio.h>
#include <stddef.h>

#ifndef PATH_MAX
#define PATH_MAX 256
#endif
#define BLOCK_SIZE 512
#include <string.h>

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

typedef struct __attribute__ ((__packed__)) header_t{
	char name[HD_NAME];
	char mode[HD_MODE];
	char uid[HD_UID];
	char gid[HD_GID];
	char size[HD_SIZE];
	char mtime[HD_MTIME];
	char chksum[HD_CHKSUM];
	char typeflag[HD_TYPEFLAG];
	char linkname[HD_LINKNAME];
	char magic[HD_MAGIC];
	char version[HD_VERSION];
	char uname[HD_UNAME];
	char gname[HD_GNAME];
	char devmajor[HD_DEVMAJOR];
	char devminor[HD_DEVMINOR];
	char prefix[HD_PREFIX];
	char padding[HD_PADDING];
}header;

typedef struct archive_t{
	header h; 
	/*some num of dataBlocks*/
}archive;


int main(int argc, char *argv[]);
void createArchive(char *path);
void createTable(char *path, int file);
int fillZeros(int fileSize, char *filePtr);

/* cases for options*/
extern int c = 0, t = 0, x = 0, v = 0; f = 0, S = 0;