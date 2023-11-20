#include "mytar.h"

header extractHeader(char *extractedHeader){
    header h;
    char name[HD_NAME], prefix[HD_PREFIX], typeflag[HD_TYPEFLAG];
    char mode[HD_MODE], uid[HD_UID], gid[HD_GID],chksum[HD_CHKSUM];
    char size[HD_SIZE], mtime[HD_MTIME], linkname[HD_LINKNAME];
    char magic[HD_MAGIC], version[HD_VERSION], uname[HD_UNAME];
    char gname[HD_GNAME], devmajor[HD_DEVMAJOR], devminor[HD_DEVMINOR];
    u_int8_t byte = 0, off;
    
    /*get name*/
    for(off = OFF_NAME; off < HD_NAME; off++){
        name[byte] = extractedHeader[off];
        byte++;
    }
    strcpy(h.name, name);
    /*get permissions*/
    for(off = OFF_MODE; off < HD_MODE; off++){
        mode[byte] = extractedHeader[off];
        byte++;
    }      
    strcpy(h.mode, mode); 
    byte = 0;
    /*get owner*/
    for(off = OFF_UID; off < HD_UID; off++){
        uid[byte] = extractedHeader[off];
        byte++;
    }
    strcpy(h.uid, uid);
    byte = 0;
    /*get group owner*/
    for(off = OFF_GID; off < HD_GID; off++){
        gid[byte] = extractedHeader[off];
        byte++;
    }
    strcpy(h.gid, gid);
    byte = 0;
    /*get size*/
    for(off = OFF_SIZE; off < HD_SIZE; off++){
        size[byte] = extractedHeader[off];
        byte++;
    }
    strcpy(h.size, size);
    byte = 0;
    /*get modification time*/
    for(off = OFF_MTIME; off < HD_MTIME; off++){
        mtime[byte] = extractedHeader[off];
        byte++;
    }
    strcpy(h.mtime, mtime);
    byte = 0;
    /*get chksum*/
    for(off = OFF_CHKSUM; off < HD_CHKSUM; off++){
        chksum[byte] = extractedHeader[off];
        byte++;
    }
    strcpy(h.chksum, chksum);
    byte = 0;
    /*get typeflag*/
    for(off = OFF_TYPEFLAG; off < HD_TYPEFLAG; off++){
        typeflag[byte] = extractedHeader[off];
        byte++;
    }
    strcpy(h.typeflag, typeflag);
    byte = 0;
    /*get linkname*/
    for(off = OFF_LINKNAME; off < HD_LINKNAME; off++){
        linkname[byte] = extractedHeader[off];
        byte++;
    }
    strcpy(h.linkname, linkname);
    byte = 0;
    /*get magic*/
    for(off = OFF_MAGIC; off < HD_MAGIC; off++){
        magic[byte] = extractedHeader[off];
        byte++;
    }
    strcpy(h.magic, magic);
    byte = 0;
    /*get version*/
    for(off = OFF_VERSION; off < HD_VERSION; off++){
        version[byte] = extractedHeader[off];
        byte++;
    }
    strcpy(h.version, version);
    byte = 0;
    /*get uname*/
    for(off = OFF_UNAME; off < HD_UNAME; off++){
        uname[byte] = extractedHeader[off];
        byte++;
    }
    strcpy(h.uname, uname);
    byte = 0;
    /*get gname*/
    for(off = OFF_GNAME; off < HD_GNAME; off++){
        gname[byte] = extractedHeader[off];
        byte++;
    }
    strcpy(h.gname, gname);
    byte = 0;
    /*get devmajor*/
    for(off = OFF_DEVMAJOR; off < HD_DEVMAJOR; off++){
        devmajor[byte] = extractedHeader[off];
        byte++;
    }
    strcpy(h.devmajor, devmajor);
    byte = 0;
    /*get devminor*/
    for(off = OFF_DEVMINOR; off < HD_DEVMINOR; off++){
        devminor[byte] = extractedHeader[off];
        byte++;
    }
    strcpy(h.devminor, devminor);
    byte = 0;
    /*get prefix*/
    for(off = OFF_PREFIX; off < HD_PREFIX; off++){
        prefix[byte] = extractedHeader[off];
        byte++;
    }
    strcpy(h.prefix, prefix);
    byte = 0;
    return h;
}