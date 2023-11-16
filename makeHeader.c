#include "mytar.c"

header makeHeader(char name[],)
{
    struct header header_st;
 /*name*/
 if (strlen(name) > HD_NAME)
    {
        strncpy(header.name, name, HD_NAME);
        if (strlen(name) > HD_NAME + HD_PREFIX)
            {
                perror("makeHeader:name too long");
                strncpy(header.prefix, name + HD_NAME, HD_PREFIX);
            }
        else
        {
        strncpy(header.prefix, name + HD_NAME, strlen(name) - HD_NAME);
        if (strlen(name) != HD_NAME + HD_PREFIX)
            {
                header.prefix[strlen(name) - HD_NAME] = '\0';
            }
        }
    }
else
    {
        strncpy(header.name, name, strlen(name));
        if (strlen(name) != HD_NAME)
            {
                header.name[strlen(name)] = '\0';
            }
    }
 /*preix padding?*/



    return header_st;
}

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
	char padding[12];
}header;
