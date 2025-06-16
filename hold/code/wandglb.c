#include "wanddef.h"

/* WANDGLB -- Non-deterministic fantasy story tool
** Global storage allocations
*/

char *whatglb = "@(#)wandglb.c\t2.11  last mod 5/29/82 -- (c) psl 1978";
char *glb_h   = H_SCCS;

#define MAXWRDS     768
#define MAXINDEX    768
#define MAXPREACTS  32
#define MAXPOSTACTS 100

struct ndxstr ndx[MAXINDEX];
struct paramstr param;
struct placestr place;
struct actstr pre_acts[MAXPREACTS];
struct actstr post_acts[MAXPOSTACTS];

struct wrdstr wrds[MAXWRDS] = {
    { "\b\b\b\b", 0, 0, MAXWRDS }, // listunused
    { "drop",      0, 0, 0 },
    { "inventory", 0, 0, 0 },
    { "quit",      0, 0, 0 },
    { "save",      0, 0, 0 },
    { "take",      0, 0, 0 },
    { "pick",      1, 0, 0 },
    { "restore",   0, 0, 0 },
    { "look",      0, 0, 0 },
    { "initialize",0, 0, 0 },
    { "history",   0, 0, 0 },
    { "north",     0, 0, 0 },
    { "n",         1, 0, 0 },
    { "south",     0, 0, 0 },
    { "s",         1, 0, 0 },
    { "east",      0, 0, 0 },
    { "e",         1, 0, 0 },
    { "west",      0, 0, 0 },
    { "w",         1, 0, 0 },
    { "up",        0, 0, 0 },
    { "u",         1, 0, 0 },
    { "down",      0, 0, 0 },
    { "d",         1, 0, 0 },
    { "northeast", 0, 0, 0 },
    { "ne",        1, 0, 0 },
    { "southeast", 0, 0, 0 },
    { "se",        1, 0, 0 },
    { "southwest", 0, 0, 0 },
    { "sw",        1, 0, 0 },
    { "northwest", 0, 0, 0 },
    { "nw",        1, 0, 0 },
    { "~snoop",    0, 0, 0 },
    { "~goto",     0, 0, 0 },
    { "~vars",     0, 0, 0 },
    { "~version",  0, 0, 0 },
    { "*",         0, 0, 0 },
    { "N1",        0, 0, 0 },
    { "N2",        0, 0, 0 },
    { "all",       0, 0, 0 },
    { NULL,        0, 0, 0 }
};

struct wrdstr spvars[] = {
    { "CUR_LOC",    0, 0, CUR_LOC },
    { "PREV_LOC",   0, 0, PREV_LOC },
    { "INP_W1",     0, 0, INP_W1 },
    { "INP_W2",     0, 0, INP_W2 },
    { "INP_W3",     0, 0, INP_W3 },
    { "INP_W4",     0, 0, INP_W4 },
    { "INP_W5",     0, 0, INP_W5 },
    { "INP_WC",     0, 0, INP_WC },
    { "NUM_CARRY",  0, 0, NUM_CARRY },
    { "MAX_CARRY",  0, 0, MAX_CARRY },
    { "NOW_YEAR",   0, 0, NOW_YEAR },
    { "NOW_MONTH",  0, 0, NOW_MONTH },
    { "NOW_DOM",    0, 0, NOW_DOM },
    { "NOW_DOW",    0, 0, NOW_DOW },
    { "NOW_HOUR",   0, 0, NOW_HOUR },
    { "NOW_MIN",    0, 0, NOW_MIN },
    { "NOW_SEC",    0, 0, NOW_SEC },
    { "NOW_ET",     0, 0, NOW_ET },
    { "BREVITY",    0, 0, BREVITY },
    { "LOC_VIEW",   0, 0, LOC_VIEW },
    { "OBJ_VIEW",   0, 0, OBJ_VIEW },
    { "INP_N1",     0, 0, INP_N1 },
    { "INP_N2",     0, 0, INP_N2 },
    { "NUM_MOVES",  0, 0, NUM_MOVES },
    { "NUM_PLACES", 0, 0, NUM_PLACES },
    { NULL,         0, 0, 0 }
};

char *thereis[] = { " ", "There is ", "There is ", "There are " };
char *aansome[] = { " ", "a ", "an ", "some " };

char fldels[] = { FIELDELIM, LINEDELIM, 0 };
char vardel[] = { VARCHAR, 0 };
char wrdels[] = { ' ', ' ' | 0x80, ',', '.', ';', '!', '?', 0 };

char listunused[] = "\b\b\b\b";

char locfile[PATHLENGTH];
char miscfile[PATHLENGTH];
char tmonfil[PATHLENGTH];
char monfile[PATHLENGTH];

char *stdpath = WANDPATH("/");
char curname[PATHLENGTH] = "a3";
char *defmfile = WANDPATH("wand.mon");

char mfbuf[BUFSIZE];
char wfbuf[BUFSIZE];

char history[HISTLEN][BUFSIZE];
int histi = 0;

int maxwrds     = MAXWRDS;
int maxlocs     = MAXLOCS;
int maxndx      = MAXINDEX;
int maxacts     = MAXACTS;
int maxpreacts  = MAXPREACTS;
int maxpostacts = MAXPOSTACTS;
int maxfields   = MAXFIELDS;
int maxvars     = MAXVARS;
int ldescfreq   = 5;

char fieldelim   = FIELDELIM;
char linedelim   = LINEDELIM;
char eschar      = ESCHAR;
char varchar     = VARCHAR;
char dotchar     = DOTCHAR;
char atchar      = ATCHAR;
char comchar     = COMCHAR;

int monitor      = -1;
int monloc       = 0;
int monstate     = 0;
int max_carry    = 8;

char inwrd[MAXACTWDS][32];
char locseen[MAXLOCS];
char locstate[MAXLOCS];
int var[MAXVARS];
FILE *mfp = NULL;
FILE *wfp = NULL;