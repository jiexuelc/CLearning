/*
 *  SHA1File.c
 *
 *  Description:
 *      This file will exercise the SHA-1 code performing the three
 *      tests documented in FIPS PUB 180-1 plus one which calls
 *      SHA1Input with an exact multiple of 512 bits, plus a few
 *      error test checks.
 *
 *  Portability Issues:
 *      None.
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sha1.h"

/* Digests a file and prints the result.
 */
static int SHA1File (const char* filename)
{
    int fd;
    SHA1Context sha;
    int i, len, err;
    uint8_t buffer[1024];
    uint8_t Message_Digest[20];
  
    err = SHA1Reset(&sha);
    if (err)
    {
        fprintf(stderr, "SHA1Reset Error %d.\n", err );
        return 1;
    }

    if ((fd = open (filename, O_RDONLY)) == -1)
    {
        printf ("%s can't be opened\n", filename);
        return 1;
    }
    
    while (len = read (fd, buffer, sizeof(buffer)))
    {
        //printf("len=%d\nstrlen=%d\n%s\n", len, strlen(buffer),buffer);
        
        err = SHA1Input(&sha, (const uint8_t *)buffer, len);
        if (err)
        {
            fprintf(stderr, "SHA1Input Error %d.\n", err );
            //break;    /* out of for i loop */
        }
    }
    
    close (fd);
    
    err = SHA1Result(&sha, Message_Digest);
    if (err)
    {
        fprintf(stderr,
        "SHA1Result Error %d, could not compute message digest.\n",
        err );
    }
    else
    {
        printf("SHA1: ");
        for(i = 0; i < 20 ; ++i)
        {
            printf("%02X", Message_Digest[i]);
        }
        printf("\n");
    }

}


int main(int argc, char* argv[])
{
    SHA1File (argv[1]);
    
    return 0;
}


