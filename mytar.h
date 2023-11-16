#define PATH_MAX 256
#define BLOCK_SIZE 512

typedef struct __attribute__ ((__packed__)) header_t{
	char name[101];
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
void createArchive(char *path, int file);
void createTable(char *path, int file);
void writeDir(char *path, int file);
void writeFile(char *path, int file);
void writeSym(char *path, int file)
extern int c = 0, t = 0, x = 0, v = 0; f = 0, S = 0;