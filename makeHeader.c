#include "mytar.h"

/* makeHeader expects the caller to input a string 
   array,(1) representing the name of the archived file
   (no need to check for length, and null termination
   not required), a stat, fileStat (2), of the file, a
   typeflag (3) represented as a single character, a 
   symlink (4) represented as a const char * pointer 
   to the path, ...*/
header makeHeader(char name[], stat fileStat, char typeflag,
                  const char * symlink, )
{
    struct header header_st;

    struct passwd *user_info = getpwuid(uid);
    struct group *group_info = getgrgid(gid);

    if (user_info == NULL || group_info == NULL)
    {
        perror("getpwuid or getgrgid");
        exit(EXIT_FAILURE);
    }

    mode_t mode = fileStat.st_mode;
    uid_t uid = fileStat.st_uid;
    gid_t gid = fileStat.st_gid;
    time_t mtime = fileStat.st_mtime;
    dev_t device = fileStat.st_dev;

    if(name[0] == '\0')
        {
            perror("makeHeader: Empty file name");
        }
    char modeStr[HD_MODE] = {'\0'};
    char uidStr[HD_UID] = {'\0'};
    char gidStr[HD_GID] = {'\0'};
    char mtimeStr[HD_MTIME] = {'\0'};
    char chksumStr[HD_CHKSUM] = {'\0'};
    char linknameStr[HD_LINKNAME] = {'\0'};
    char devmajorStr[HD_DEVMAJOR] = {'\0'};
    char devminorStr[HD_DEVMINOR] = {'\0'};

 /*name, prefix*/

 header_st.prefix[0] = '\0';
/*Need to implement: if the name continues onto the prefix uncleanly (splits one of the file path name), then move that split name to
the prefix. Also, when going from name to prefix, remove the last slash in between the name and the prefix*/
 if (strlen(name) == HD_NAME + 1 &&
     name[HD_NAME] == '\0')
     {
        memcpy(header_st.name, name, HD_NAME);

     }
else if (strlen(name) == HD_NAME + HD_PREFIX + 1 &&
         name[HD_NAME + HD_PREFIX] == '\0')
     {
        memcpy(header_st.name, name, HD_NAME);
        memcpy(header_st.prefix, name + HD_NAME, HD_PREFIX);
     }

 else if (strlen(name) > HD_NAME)
    {
        strncpy(header_st.name, name, HD_NAME);
        if (strlen(name) > HD_NAME + HD_PREFIX)
            {
                perror("makeHeader:name too long");
                strncpy(header_st.prefix, name + HD_NAME, HD_PREFIX);
            }
        else
        {
        strncpy(header_st.prefix, name + HD_NAME, strlen(name) - HD_NAME);
        if (strlen(name) != HD_NAME + HD_PREFIX &&
            header_st.prefix[strlen(name) - HD_NAME - 1 ] != '\0')
            {
                header_st.prefix[strlen(name) - HD_NAME] = '\0';
            }
        }
    }
else
    {
        strncpy(header_st.name, name, strlen(name));
        if (strlen(name) != HD_NAME &&
            header_st.name[strlen(name) - 1] != '\0' )
            {
                header_st.name[strlen(name)] = '\0';
            }
        header_st.prefix[0] = '\0';
    }

/*mode*/
   
    if(mode & (S_IXUSR | S_IXGRP | S_IXOTH))   /* if anyone has
                                            execute permissions,
                                            grant to all*/
        {
            mode |= (S_IXUSR | S_IXGRP | S_IXOTH);
        }
    snprintf(modeStr, sizeof(modeStr), "%o", mode);
    header_st.mode = modeStr;

/*uid*/

    snprintf(uidStr, sizeof(uidStr), "%o", uid);
    header_st.uid = uidStr;

/*gid*/

    snprintf(gidStr, sizeof(gidStr), "%o", gid);
    header_st.gid = gidStr;

/*mtime*/

    snprintf(uidStr, sizeof(uidStr), "%o", fileStat.st_uid);
    header_st.mtime = mtimeStr;

/*typeflag*/

    header_st.typeflag = typeflag;

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
            header_st.linkname[HD_LINKNAME - 1] = '\0';
            strncpy(header_st.linkname, symlink, HD_LINKNAME);
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

/*devmajor*/

    snprintf(devmajorStr, sizeof(devmajorStr), "%o", major(device));
    strncpy(header_st.devmajor, devmajorStr, HD_DEVMAJOR);

/*devminor*/

    snprintf(devminorStr, sizeof(devminorStr), "%o", minor(device));
    strncpy(header_st.devminor, devminorStr, HD_DEVMINOR);

/*padding*/

for (int i = 0; i < HD_PADDING; i++)
{
header_st.padding[i] = '\0';
}

/*chksum*/

    unsigned char *bytes = (unsigned char *) &header_st;
    unsigned int sum = 0;
    for (size_t i = 0; i < sizeof(header_st); i++)
        {
            sum += bytes[i];
        }

    snprintf(chksumStr, sizeof(header_st.chksum), "%o", sum);
    header_st.chksum =chksumStr;


    return header_st;
}