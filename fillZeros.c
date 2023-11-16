#include "mytar.h"

size_t fillZeros(int fileSize, char *filePtr)
{   
    if (fileSize < 0)
        {
            perror("fillZeros: invalid file size");
            exit(EXIT_FAILURE);
        }
    if ((BLOCK_SIZE % fileSize) == 0)   /*If the fileSize
                                     is a perfect multiple of
                                    BLOCK_SIZE, no padding
                                    should be added, and it
                                    returns the filesize
                                    unmodified*/
        {
            return fileSize;
        }
    int padding = BLOCK_SIZE % (BLOCK_SIZE - fileSize);
    for (int i = 0; i < padding; i++)
    {
        filePtr[fileSize + i] = '\0';
    }
return fileSize + padding;
}