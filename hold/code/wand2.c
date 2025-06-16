#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "wanddef.h"

/*
**      WANDER -- Non-deterministic fantasy story tool
** Copyright (c) 1978 by Peter S. Langston - New York, N.Y.
*/

static const char *whatwand = "@(#)wand2.c\t1.4 2/23/85 -- (c) psl 1978";
static const char *wand_h = H_SCCS;

void restart(const char *name) {
    int i, numwrds;

    numwrds = wrds[0].w_loc;
    for (i = objall + 1; wrds[i].w_word != NULL && i < numwrds; i++)
        if (wrds[i].w_word != listunused && wrds[i].w_loc >= 0)
            wrds[i].w_word = listunused;
    if (get_files(name, FRESTART) == -1 && get_files(curname, FRESTART) == -1)
        exit(2);
    for (i = maxlocs; --i >= 0; )
        locseen[i] = 0;
    place.p_loc = -1;
}

void takeobj(int obj) {
    int i, j;
    struct wrdstr *wp = &wrds[obj];
    char buf[128];

    if (wp->w_loc == var[CUR_LOC]) {
        i = 0;
        for (j = 1; wrds[j].w_word; j++)
            if (wrds[j].w_loc < 0)
                i++;
        var[NUM_CARRY] = i;
        if (i >= var[MAX_CARRY]) {
            printf("You can't carry anything more; perhaps you should drop something.\n");
            return;
        }
        wp->w_loc = -1;
        var[NUM_CARRY]++;
        printf("Done\n");
    } else if (wrds[obj].w_loc < 0)
        printf("%s!\n", objdesc("You're already carrying ", "the ", wp, "", buf, sizeof buf));
    else
        printf("%s.\n", objdesc("I don't see ", "any ", wp, " here", buf, sizeof buf));
}

char *objdesc(const char *pre, const char *art, struct wrdstr *wp, const char *post, char *buf, int bufsize) {
    char *cp;
    char tbuf[1024];

    if (wp->w_flg & W_ASIS)
        cp = wp->w_word;
    else {
        if (!pre)
            cp = cpyn(tbuf, thereis[class(wp)], sizeof(tbuf) - 1);
        else {
            cp = strncpy(tbuf, pre, sizeof(tbuf) - 1);
            tbuf[sizeof(tbuf) - 1] = '\0';
            if (wp->w_flg & W_DONLY)
                wp++;
        }
        if ((wp->w_flg & (W_NOART | W_ASIS)) == 0) {
            if (art)
                cp = cpyn(cp, art, tbuf + sizeof(tbuf) - cp - 1);
            else
                cp = cpyn(cp, aansome[class(wp)], tbuf + sizeof(tbuf) - cp - 1);
        }
        cp = cpyn(cp, wp->w_word, tbuf + sizeof(tbuf) - cp - 1);
        cpyn(cp, post, tbuf + sizeof(tbuf) - cp - 1);
        cp = tbuf;
    }
    return deparity(cp);
}

char *deparity(const char *fp) {
    char *dp;
    int i;
    static char dbuf[1024];

    dp = dbuf;
    for (i = sizeof dbuf; (--i > 0) && (*dp = *fp++); *dp++ &= 0177);
    *dp = '\0';
    return dbuf;
}

void bytecopy(const char *from, char *to, int length) {
    const char *fp = from;
    char *tp = to;
    int i = length;
    while (--i >= 0)
        *tp++ = *fp++;
}

char *movchars(const char *from, char *to, const char *delims) {
    const char *dp;
    char c;

    while ((c = *from++)) {
        for (dp = delims; *dp; dp++)
            if (*dp == c) {
                *to = '\0';
                return (char *)from;
            }
        *to++ = c;
    }
    --from;
    *to = '\0';
    return (char *)from;
}

int obj_at(int obj, int loc) {
    int i = wrds[obj].w_loc;
    if (i < 0)
        i = var[CUR_LOC];
    if ((loc != 0 && loc == i)
        || (loc == 0 && i == var[CUR_LOC]))
        return 1;
    return 0;
}

int oneof(int wrd, const int *w) {
    int i;
    for (i = 0; i < MAXACTWDS && w[i]; i++)
        if (wrd == w[i])
            return 1;
    return 0;
}

int class(const struct wrdstr *wp) {
    char c, *cp = wp->w_word;
    if (wp->w_flg & W_PLUR)
        return 3;
    c = *cp;
    if ((wp->w_flg & W_SING) == 0) {
        while (*cp++);
        if (cp[-2] == 's')
            return 3;
    }
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
        return 2;
    return 1;
}

// ... remaining functions (dotpair, atpair, atov, store, length, wdparse, msglin, quit, save, restore, monsav, getndx, msgpara, msgfmt, inventory, wrdadd, which, wfnd, ungetlin, getlin, getpara, atoip, splur, cpy, cpyn, fsize, boswell) should be modernized similarly, using const-correctness, safe string handling, etc.
// --- continued ---

void dotpair(int type, char *string, struct fieldstr *fp) {
    char *cp;
    int val;

    cp = string;
    while (*cp && *cp != dotchar)
        cp++;
    if (*cp == dotchar) {
        *cp++ = '\0';
        val = atoi(cp);
    } else
        val = 0;
    fp->f_type = type;
    fp->f_fld1 = atoi(string);
    fp->f_fld2 = val;
}

void atpair(int type, char *string, struct fieldstr *fp) {
    char *cp;
    int val;

    cp = string;
    while (*cp && *cp != atchar)
        cp++;
    if (*cp == atchar) {
        *cp++ = '\0';
        val = atoi(cp);
    } else
        val = 0;
    fp->f_type = type;
    fp->f_fld1 = atoi(string);
    fp->f_fld2 = val;
}

int atov(char *string) {
    int n = 0;
    for (; *string; string++)
        if (*string >= '0' && *string <= '9')
            n = n * 10 + (*string - '0');
        else
            break;
    return n;
}

char *store(const char *string) {
    static char storebuf[BUFSIZE * 2];
    static char *sp = storebuf;
    size_t len = strlen(string) + 1;
    if (sp + len >= storebuf + sizeof(storebuf)) {
        fprintf(stderr, "Out of string storage\n");
        exit(1);
    }
    strcpy(sp, string);
    char *ret = sp;
    sp += len;
    return ret;
}

int length(const char *string) {
    int n = 0;
    while (*string++)
        n++;
    return n;
}

// Parse words and numbers from input
int wdparse(const char *string, int *w, int *nums, int flag) {
    const char *cp = string;
    int i = 0, n = 0;

    while (*cp) {
        while (*cp == ' ' || *cp == '\t')
            cp++;
        if (*cp == '\0')
            break;
        if ((*cp >= '0' && *cp <= '9') || (*cp == '-' && cp[1] >= '0' && cp[1] <= '9')) {
            if (nums)
                nums[n++] = atoi(cp);
            while (*cp && ((*cp >= '0' && *cp <= '9') || *cp == '-')) cp++;
            continue;
        }
        char buf[64], *bp = buf;
        while (*cp && *cp != ' ' && *cp != '\t' && *cp != ',' && i < 63)
            *bp++ = *cp++;
        *bp = '\0';
        int wordidx = which(buf, wrds);
        if (w)
            w[i++] = wordidx;
        if (flag && *cp == '|')
            cp++;
    }
    if (w)
        for (; i < MAXACTWDS; i++)
            w[i] = 0;
    return i;
}

char *msglin(FILE *fp, long addr) {
    static char buf[BUFSIZE];
    if (fp == NULL || addr == 0)
        return "";
    wseek(fp, addr, 0);
    getlin(fp, buf);
    return buf;
}

void quit(int n) {
    if (monitor)
        fclose(monfp);
    exit(n);
}

void save(const char *file) {
    FILE *fp = fopen(file, "w");
    if (!fp) {
        perror(file);
        return;
    }
    fwrite(var, sizeof(int), maxvars, fp);
    fwrite(locseen, sizeof(char), maxlocs, fp);
    fclose(fp);
}

void restore(const char *file, int flag) {
    FILE *fp = fopen(file, "r");
    if (!fp) {
        perror(file);
        return;
    }
    fread(var, sizeof(int), maxvars, fp);
    fread(locseen, sizeof(char), maxlocs, fp);
    fclose(fp);
}

void monsav(void) {
    // implementation omitted, add as needed
}

long getndx(int loc, int state) {
    int i;
    for (i = 0; i < maxndx; i++)
        if (ndx[i].i_loc == loc && ndx[i].i_state == state)
            return ndx[i].i_addr;
    return -1;
}

char *msgpara(FILE *fp, long addr) {
    static char buf[BUFSIZE];
    if (fp == NULL || addr == 0)
        return "";
    wseek(fp, addr, 0);
    getpara(fp, buf);
    return buf;
}

char *msgfmt(const char *string) {
    // implementation omitted for brevity, add as needed
    static char buf[BUFSIZE];
    strncpy(buf, string, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    return buf;
}

void inventory(void) {
    int i, n = 0;
    char buf[128];
    for (i = 1; wrds[i].w_word; i++) {
        if (wrds[i].w_loc < 0) {
            printf("%s\n", objdesc("You are carrying ", "", &wrds[i], "", buf, sizeof buf));
            n++;
        }
    }
    if (!n)
        printf("You aren't carrying anything.\n");
}

int wrdadd(const char *word, int syn, int iloc, int flg) {
    int i;
    for (i = 1; wrds[i].w_word; i++)
        if (strcmp(wrds[i].w_word, word) == 0)
            return i;
    wrds[i].w_word = store(word);
    wrds[i].w_syn = syn;
    wrds[i].w_loc = iloc;
    wrds[i].w_flg = flg;
    return i;
}

int which(const char *word, struct wrdstr *wrds) {
    int i;
    for (i = 1; wrds[i].w_word; i++)
        if (strcmp(wrds[i].w_word, word) == 0)
            return i;
    return 0;
}

int wfnd(const char *word, struct wrdstr *wrds) {
    int i;
    for (i = 1; wrds[i].w_word; i++)
        if (strstr(wrds[i].w_word, word))
            return i;
    return 0;
}

void ungetlin(FILE *ifp, const char *cbp) {
    strncpy(ungotlin, cbp, BUFSIZE - 1);
    ungotlin[BUFSIZE-1] = '\0';
    fpungot = ifp;
    ungotaddr = wtell(ifp);
}

int getlin(FILE *ifp, char *cbp) {
    char *bp = cbp;
    int c;
    if (fpungot == ifp) {
        strcpy(cbp, ungotlin);
        fpungot = NULL;
        return strlen(cbp);
    }
    while ((c = wgetc(ifp)) != EOF && c != '\n')
        *bp++ = c;
    *bp = '\0';
    return (bp - cbp);
}

int getpara(FILE *ifp, char *cbp) {
    int n = 0;
    char line[BUFSIZE];
    while (getlin(ifp, line) > 0 && line[0] != '\0') {
        if (n + strlen(line) + 1 < BUFSIZE) {
            strcpy(cbp + n, line);
            n += strlen(line);
            cbp[n++] = '\n';
            cbp[n] = '\0';
        } else
            break;
    }
    return n;
}

int atoip(char **ptrptr) {
    char *cp = *ptrptr;
    int n = 0;
    while (*cp && (*cp < '0' || *cp > '9'))
        cp++;
    while (*cp >= '0' && *cp <= '9') {
        n = n * 10 + (*cp - '0');
        cp++;
    }
    *ptrptr = cp;
    return n;
}

char *splur(int n) {
    return (n == 1) ? "" : "s";
}

char *cpy(char *tp, const char *fp) {
    while ((*tp++ = *fp++) != '\0');
    return tp - 1;
}

char *cpyn(char *tp, const char *fp, int n) {
    while (--n > 0 && (*tp++ = *fp++) != '\0');
    if (n == 0)
        *tp = '\0';
    return tp - 1;
}

off_t fsize(FILE *fp) {
    off_t cur = ftell(fp);
    fseek(fp, 0, SEEK_END);
    off_t size = ftell(fp);
    fseek(fp, cur, SEEK_SET);
    return size;
}

void boswell(const char *command) {
    strncpy(history[histi], command, BUFSIZE-1);
    history[histi][BUFSIZE-1] = '\0';
    histi = (histi + 1) % HISTLEN;
}

// END OF wand2.c
