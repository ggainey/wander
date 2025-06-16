#include <stdio.h>
#include <stdlib.h>
#include "wanddef.h"

/* WANDSYS -- System dependent routines for Wander
** Interfaces to stdio routines to keep track of file position
** Copyright (c) by Peter S. Langston - New York, N.Y.
*/

/* Uncomment next line for systems without ftell() */
//#define NOFTELL

#ifdef NOFTELL
static const char *whatwand = "@(#)wandsys.c\t1.1  WITHOUT FTELL() 2/22/84 -- (c) psl";
#else
static const char *whatwand = "@(#)wandsys.c\t1.1  with ftell() 2/22/84 -- (c) psl";
#endif
static const char *wand_h = H_SCCS;

FILE *fpungot = NULL;

#ifdef NOFTELL
#define NUMWTELL 2
struct wtellstr {
    FILE *wt_fp;
    long wt_addr;
} wt[NUMWTELL + 1];
#endif

FILE *wopen(const char *file, const char *rwflg) {
#ifdef NOFTELL
    int i;
    FILE *fp;
    if ((fp = fopen(file, rwflg)) != NULL) {
        i = wtfind(NULL);
        wt[i].wt_fp = fp;
        wt[i].wt_addr = 0L;
    }
    return fp;
#else
    return fopen(file, rwflg);continue wand2.c
#endif
}

int wseek(FILE *fp, long addr, int mode) {
#ifdef NOFTELL
    int i = wtfind(fp);
    if (mode == 0)
        wt[i].wt_addr = addr;
    else if (mode == 1)
        wt[i].wt_addr += addr;
    else if (mode == 2)
        fprintf(stderr, "Illegal wseek(%p, %ld, %d)\n", (void*)fp, addr, mode);
#endif
    if (fpungot == fp)
        fpungot = NULL;
    return fseek(fp, addr, mode);
}

int wgetc(FILE *fp) {
#ifdef NOFTELL
    int i;
    if (fp != stdin) {
        i = wtfind(fp);
        wt[i].wt_addr++;
    }
#endif
    return getc(fp);
}

long wtell(FILE *fp) {
#ifdef NOFTELL
    int i;
    if (fp == stdin)
        return 0L;
    i = wtfind(fp);
    return wt[i].wt_addr;
#else
    return ftell(fp);
#endif
}

int wclose(FILE *fp) {
#ifdef NOFTELL
    int i = wtfind(fp);
#endif
    if (fpungot == fp)
        fpungot = NULL;
    return fclose(fp);
}

#ifdef NOFTELL
int wtfind(FILE *fp) {
    for (int i = 0; i < NUMWTELL; i++)
        if (fp == wt[i].wt_fp)
            return i;
    fprintf(stderr, "wtfind(%p): Couldn't find a record\n", (void*)fp);
    return NUMWTELL;
}
#endif