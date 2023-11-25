#include "mytar.h"

int insert_special_int(char *where, size_t size, int32_t val) {
/* For interoperability with GNU tar. GNU seems to
 * * set the high–order bit of the first byte, then
 * * treat the rest of the field as a binary integer
 * * in network byte order.
 * * Insert the given integer into the given field
 * * using this technique. Returns 0 on success, nonzero
 * * otherwise
 * */
int err=0;
if ( val < 0 || ( size < sizeof(val)) ) {
/* if it’s negative, bit 31 is set and we can’t use the flag
 * * if len is too small, we can’t write it. Either way, we’re
 * * done.
 * */
err++;
} else {
/* game on....*/
memset(where, 0, size); /* Clear out the buffer */
*(int32_t *)(where + size - sizeof(val)) = htonl(val); /* place the int */
*where |= 0x80; /* set that high–order bit */
}
return err;
}
