/* exercise 4-1 */

/* mytee.c

   Copy the stdin to stdout and to a new file named in argv[1](if exists).
*/
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include "tlpi_hdr.h"

#define printable(ch) (isprint((unsigned char) ch) ? ch : '#')
#ifndef BUF_SIZE        /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif

static void             /* Print "usage" message and exit */
usageError(char *progName, char *msg, int opt)
{
    if (msg != NULL && opt != 0)
        fprintf(stderr, "%s (-%c)\n", msg, printable(opt));
    fprintf(stderr, "Usage: %s [-p arg] [-x]\n", progName);
    exit(EXIT_FAILURE);
}
int
main(int argc,char *argv[]){
    char buf[BUF_SIZE];
    int outputFd=-1,openFlags,append=0,opt;
    mode_t filePerms;
    ssize_t numRead;

    while ((opt = getopt(argc, argv, "a")) != -1) {
        printf("opt =%3d (%c); optind = %d", opt, printable(opt), optind);
        if (opt == '?' || opt == ':')
            printf("; optopt =%3d (%c)", optopt, printable(optopt));
        printf("\n");

        switch (opt) {
        case 'a': append = 1;        break;
        case ':': usageError(argv[0], "Missing argument", optopt);
        default:  fatal("Unexpected case in switch()");
        }
    }
    argv += optind;
    argc -= optind;

    openFlags = append?O_WRONLY|O_CREAT|O_APPEND:O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;      /* rw-rw-rw- */
    if(argc>0){
        outputFd = open(argv[0], openFlags, filePerms);
        if (outputFd == -1)
            errExit("opening file %s", argv[2]);
    }
    while((numRead = read(0,buf,BUF_SIZE))>0){
        if(write(1,buf,numRead)!=numRead)
            fatal("couldn't write whole buffer");
        if(outputFd>0&&write(outputFd,buf,numRead)!=numRead)
            fatal("couldn't write whole buffer to outputFd%d",outputFd);
    }
    if(numRead==-1)
        errExit("read");
    if(close(outputFd)==-1)
        errExit("close outputFd");

    exit(EXIT_SUCCESS);
    
}
