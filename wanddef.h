#ifndef WANDDEF_H
#define WANDDEF_H

#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>

/* WANDDEF.H -- Non-deterministic fantasy story tool header
** Global definitions
** Copyright (c) 1978 by Peter S. Langston - New York, N.Y.
*/

#define H_SCCS  "@(#)wanddef.h\t1.8  last mod 6/9/84 -- (c) psl 1978"

/* Editable parameters */
#define GAMESPATH(X)    "/Users/psl/Games+Toys/" X
#define WANDPATH(X)     GAMESPATH("Wander/" X)

/* Buffer and object sizes */
#define MAXLOCS     1024
#define MAXACTS     64
#define MAXFIELDS   8
#define PATHLENGTH  1024
#define MAXACTWDS   5
#define MAXVARS     128
#define BUFSIZE     4096
#define MAXINPNUMS  2
#define HISTLEN     128

#define FIELDELIM   ' '
#define LINEDELIM   '\n'
#define ESCHAR      '\\'
#define VARCHAR     '%'
#define DOTCHAR     '.'
#define ATCHAR      '@'
#define COMCHAR     ':'

#define BASESTATE   -1
#define FLD1_VAR    0x80
#define FLD2_VAR    0x40
#define TYPEONLY    0x3F
#define NO_WORD     0

#define COM_UNREC   0
#define COM_RECOG   1
#define COM_DONE    2
#define COM_DESC    4
#define COM_NDOBJ   8
#define COM_COMPLETE 16

#define QUIT_SCORE  -1
#define QUIT_QUIET  -2

#define CUR_LOC     100
#define PREV_LOC    101
#define INP_W1      102
#define INP_W2      103
#define INP_W3      104
#define INP_W4      105
#define INP_W5      106
#define INP_WC      107
#define NUM_CARRY   108
#define MAX_CARRY   109
#define NOW_YEAR    110
#define NOW_MONTH   111
#define NOW_DOM     112
#define NOW_DOW     113
#define NOW_HOUR    114
#define NOW_MIN     115
#define NOW_SEC     116
#define NOW_ET      117
#define BREVITY     118
#define LOC_VIEW    119
#define OBJ_VIEW    120
#define INP_N1      121
#define INP_N2      122
#define NUM_MOVES   123
#define NUM_PLACES  124

/* Field types */
#define F_VOID      0
#define FT_OBJ      1
#define FT_NOBJ     2
#define FT_TOOL     3
#define FT_NTOOL    4
#define FT_STATE    5
#define FT_NSTATE   6
#define FT_EVAR     7
#define FT_NVAR     8
#define FT_GVAR     9
#define FT_LVAR     10
#define FT_ODDS     11
#define FT_EBIN     12
#define FT_NBIN     13
#define FT_GBIN     14
#define FT_LBIN     15

#define FR_GOBJ     20
#define FR_LOBJ     21
#define FR_GTOOL    22
#define FR_LTOOL    23
#define FR_SSTATE   24
#define FR_ISTATE   25
#define FR_DSTATE   26
#define FR_SVAR     27
#define FR_IVAR     28
#define FR_DVAR     29
#define FR_MVAR     30
#define FR_QVAR     31
#define FR_CSUB     32
#define FR_WORLD    33
#define FR_SBIN     34
#define FR_IBIN     35
#define FR_DBIN     36

#define FRESTART    0
#define FMAINNEW    1
#define FRESTORE    2
#define FMAINRES    3

struct paramstr {
    int     p_pathlength;
    int     p_histlen;
    int     p_histi;
    int     p_maxlocs;
    int     p_maxwrds;
    int     p_maxvars;
    int     p_maxndx;
    int     p_maxpre;
    int     p_maxpost;
    char   *p_storebuf;
    int     p_sbufsiz;
    int     p_time;
    off_t   p_msize;
    off_t   p_wsize;
};

struct fieldstr {
    char    f_type;
    int     f_fld1;
    int     f_fld2;
};

struct actstr {
    int     a_wrd[MAXACTWDS];
    int     a_rloc;
    char    a_rcont;
    struct  fieldstr a_field[MAXFIELDS];
    FILE   *a_msgfp;
    long    a_msgaddr;
};

struct placestr {
    int     p_loc;
    char    p_state;
    long    p_sdesc;
    long    p_ldesc;
    struct actstr p_acts[MAXACTS];
};

/* Object word flags */
#define W_SING      1
#define W_PLUR      2
#define W_NOART     4
#define W_ASIS      8
#define W_DONLY     16

struct wrdstr {
    char    *w_word;
    char    w_syn;
    char    w_flg;
    int     w_loc;
};

struct ndxstr {
    int  i_loc;
    char i_state;
    long i_addr;
};

/* Externals (globals in wandglb.c) */
extern struct ndxstr    ndx[];
extern struct placestr   place;
extern struct actstr     pre_acts[], post_acts[];
extern struct wrdstr     wrds[], spvars[];
extern char             *thereis[], *aansome[];
extern char              fldels[], vardel[], wrdels[];
extern char              listunused[];
extern char              locfile[], miscfile[], tmonfil[], monfile[PATHLENGTH];
extern char              curname[PATHLENGTH], *stdpath, *defmfile;
extern char              mfbuf[], wfbuf[];
extern char              history[HISTLEN][BUFSIZE];
extern int               histi;
extern int               maxwrds, maxactwds, maxinpwd, maxlocs, maxndx;
extern int               maxacts, maxpreacts, maxpostacts, maxfields, maxvars;
extern int               ldescfreq;
extern char              fieldelim, linedelim;
extern char              eschar, varchar, dotchar, atchar, comchar;
extern int               monitor, monloc, monstate;
extern int               max_carry;
extern char              inwrd[][32];
extern char              locseen[], locstate[];
extern int               var[];
extern FILE             *mfp, *wfp;

// Externs from wand1.c
extern int              objall;
extern FILE            *monfp;
extern char             ungotlin[BUFSIZE];
extern FILE            *fpungot;
extern long             ungotaddr;

/* Function prototypes (ANSI C) */
int     main(int argc, char *argv[]);
void    prloc(void);
char   *getcom(void);
int     carry_out(const char *com);
int     check_act(struct actstr *actp);
void    get_loc(int loc, int state);
void    setup(int argc, char *argv[]);
struct actstr *code_act(struct actstr *actbuf, int maxacts, struct actstr *actp, char *buf, FILE *ifp, long baddr);
int     get_files(const char *name, int flag);

/* Routines in wand2.c */
void    restart(const char *name);
void    takeobj(int obj);
char   *objdesc(const char *pre, const char *art, struct wrdstr *wp, const char *post, char *buf, int bufsize);
char   *deparity(const char *from);
void    bytecopy(const char *from, char *to, int length);
char   *movchars(const char *from, char *to, const char *delims);
int     obj_at(int obj, int loc);
int     oneof(int wrd, const int *w);
int     class(const struct wrdstr *wp);
void    dotpair(int type, char *string, struct fieldstr *fp);
void    atpair(int type, char *string, struct fieldstr *fp);
int     atov(char *string);
char   *store(const char *string);
int     length(const char *string);
int     wdparse(const char *string, int *w, int *nums, int flag);
char   *msglin(FILE *fp, long addr);
void    quit(int n);
void    save(const char *file);
void    restore(const char *file, int flag);
void    monsav(void);
long    getndx(int loc, int state);
char   *msgpara(FILE *fp, long addr);
char   *msgfmt(const char *string);
void    inventory(void);
int     wrdadd(const char *word, int syn, int iloc, int flg);
int     which(const char *word, struct wrdstr *wrds);
int     wfnd(const char *word, struct wrdstr *wrds);
void    ungetlin(FILE *ifp, const char *cbp);
int     getlin(FILE *ifp, char *cbp);
int     getpara(FILE *ifp, char *cbp);
int     atoip(char **ptrptr);
char   *splur(int n);
char   *cpy(char *tp, const char *fp);
char   *cpyn(char *tp, const char *fp, int n);
off_t   fsize(FILE *fp);
void    boswell(const char *command);

#endif // WANDDEF_H
