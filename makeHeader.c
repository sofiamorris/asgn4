#include "mytar.h"
#define FULLPERMS 0777
#define CHKSUM 256

/* makeHeader expects the caller to input a string 
   array,(1) representing the name of the archived file
   (no need to check for length, and null termination
   not required), a stat, fileStat (2), of the file, a
   typeflag (3) represented as a single character, a 
   symlink (4) represented as a const char * pointer 
   to the path, ...*/
header makeHeader(char name[], struct stat fileStat, char typeflag,\
    const char * symlink, int size, int S)
{
    struct passwd *user_info = getpwuid(fileStat.st_uid);
    struct group *group_info = getgrgid(fileStat.st_gid);

    if (user_info == NULL || group_info == NULL)
    {
        perror("getpwuid or getgrgid");
        exit(EXIT_FAILURE);
    }

    mode_t mode = fileStat.st_mode;
    uid_t uid = fileStat.st_uid;
    gid_t gid = fileStat.st_gid;
    time_t mtime = fileStat.st_mtime;

    if(name[0] == '\0')
        {
            perror("makeHeader: Empty file name");
        }
    char modeStr[HD_MODE] = {'\0'};
    char gidStr[HD_GID] = {'\0'};
    char sizeStr[HD_SIZE] = {'\0'};
    char mtimeStr[HD_MTIME] = {'\0'};
    char chksumStr[HD_CHKSUM] = {'\0'};

    header header_st = {
    .name = {0},
    .mode = {0},
    .uid = {0},
    .gid = {0},
    .size = {0},
    .mtime = {0},
    .chksum = {0},
    .typeflag = {0},
    .linkname = {0},
    .magic = {0},
    .version = {0},
    .uname = {0},
    .gname = {0},
    .devmajor = {0},
    .devminor = {0},
    .prefix = {0},
    .padding = {0}
    };

 /*name, prefix*/

   if (strlen(name) > HD_NAME + HD_PREFIX) {
            perror("makeHeader: name too long");
            exit(EXIT_FAILURE);
        }
    else if (strlen(name) < HD_NAME) {
        strncpy(header_st.name, name, HD_NAME - 1);
        header_st.name[HD_NAME - 1] = '\0';
        if (typeflag == '5'){
            strcat(header_st.name, "/");
        }
        header_st.prefix[0] = '\0';
    } else {
        int lastSlash = -1;
        for (int i = HD_NAME; i >= 0; i--) {
            if (name[i] == '/') {
                lastSlash = i;
                break;
            }
        }

        int nameLength, prefixLength;

        if (lastSlash == -1) {
            nameLength = HD_NAME - 1;
            prefixLength = strlen(name) - nameLength;
        } else {
            nameLength = lastSlash;
            prefixLength = strlen(name) - lastSlash - 1;
        }


        strncpy(header_st.name, name, nameLength);
        if (nameLength != 0)
         {
             header_st.name[nameLength] = '\0';
         }

        if (prefixLength > 0) {
            strncpy(header_st.prefix, name + lastSlash + 1, prefixLength);
            header_st.prefix[prefixLength] = '\0';
            if (typeflag == '5'){
                strncat(header_st.prefix, "/",\
                     sizeof(header_st.prefix) - prefixLength - 1);
            }
        } else {
            header_st.prefix[0] = '\0';
        }
    }

/*mode*/
    int permissions = mode & FULLPERMS;  
    snprintf(modeStr, HD_MODE, "%07o", permissions);
    strcpy(header_st.mode, modeStr);

/*uid*/
    insert_special_int(header_st.uid, (size_t)HD_UID, uid); 

/*gid*/

    snprintf(gidStr, sizeof(gidStr), "%07o", gid);
    strcpy(header_st.gid, gidStr);

/*size*/
    snprintf(sizeStr, sizeof(sizeStr), "%011o", size);
    strcpy(header_st.size, sizeStr);

/*mtime*/

    snprintf(mtimeStr, sizeof(mtimeStr), "%011lo", (unsigned long)mtime);
    strcpy(header_st.mtime, mtimeStr);

/*typeflag*/
    
    if (typeflag == '0' || typeflag == '\0'){
        strcpy(header_st.typeflag, "0");
    }
    else if (typeflag == '5'){
        strcpy(header_st.typeflag, "5");                              
    }
    else{
        strcpy(header_st.typeflag, "0");                              
    }

/*linkname*/

    if (header_st.typeflag == '2')  /*'2' is the typeflag rep
                                    for a symlink*/
        {
            if (symlink != NULL)
            {
                ssize_t linkLen = readlink(symlink, header_st.linkname,
                                          HD_LINKNAME - 1);
                if (linkLen > HD_LINKNAME)
                    {
                        perror("makeHeader: sym link too long");
                    }
                if (linkLen != -1)
                    {
                        header_st.linkname[linkLen] = '\0';
                    }
                else
                    {
                        perror("makeHeader: readlink");
                    }
            }
            else
            {
                perror("makeHeader: symlink path is NULL");            }
        }
    
    else
        {
            /*do nothing*/
        }

/*magic*/

    memcpy(header_st.magic, "ustar\0", HD_MAGIC);

/*version*/

    memcpy(header_st.version, "00", HD_VERSION);

/*uname*/

    strncpy(header_st.uname, user_info->pw_name, HD_UNAME);
    header_st.uname[HD_UNAME - 1] = '\0';

/*gname*/

    strncpy(header_st.gname, group_info->gr_name, HD_GNAME);
    header_st.gname[HD_GNAME - 1] = '\0';

/*leave devmajor and devminor empty*/

/*padding (just in case its vals got changed)*/

for (int i = 0; i < HD_PADDING; i++)
{
header_st.padding[i] = '\0';
}

/*chksum*/

    unsigned char *bytes = (unsigned char *) &header_st;
    unsigned int sum = 0;
    for (size_t j = 0; j < sizeof(header_st); j++) {
        sum += bytes[j];
    }
    sum += CHKSUM;
    snprintf(chksumStr, sizeof(header_st.chksum), "%07o", sum);
    strcpy(header_st.chksum, chksumStr);


    return header_st;
}
