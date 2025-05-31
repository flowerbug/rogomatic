/*
 * Rog-O-Matic
 * Automatically exploring the dungeons of doom.
 *
 * Copyright (C) 2008 by Anthony Molinaro
 * Copyright (C) 1985 by Appel, Jacobson, Hamey, and Mauldin.
 *
 * This file is part of Rog-O-Matic.
 *
 * Rog-O-Matic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rog-O-Matic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rog-O-Matic.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * types.h:
 *
 * Miscellaneous Macros and Constants
 */

/* Global Preprocessor constants */

# define ill         ";'"
# define status		   "Str:"
# define MAXATTEMPTS (10)
# define ROGQUIT     (-2)
# define DIED        (1)
# define FINISHED    (0)
# define SAVED       (2)
# define MAXINV      (26)
# define NONE        (-1)
# define MAXSTUFF    (100)
# define MAXMONST    (40)
# define SAVEARROWS  (30)
# define NAMSIZ      (64)
# define MAXLEV      (30)
# define MAXMON      (128)
# define SUCCESS     (1)
# define FAILURE     (0)

# define USLEEP   (14000)  /* set to 0 for full blast.  not using the "-w"
                              option also skips these delays.

                              i'm using 14000 here because that slows
                              things down enough that i can watch.

                              it also it keeps the fan from spinning up
                              (saving battery life, reducing energy use
                               in general or reducing fan noise).

                              at higher levels it goes even slower because
                              it should be more interesting (or at least
                              in theory...)

                              for your specific machine you might have to
                              adjust this up or down to suit your own
                              preferences.

                              if you want to adjust the specific values
                              see io.c (yay source code! :) ) */

/*
 * Magic numbers for Invisible stalker strategies
 */

# define INVDAM		(16)
# define INVHIT		(1000)
# define INVPRES	(INVHIT-100)
# define INVLURK	(INVPRES-200)

/*
 * Attribute bits for the screen map: If you change this list, be sure
 * to change the flag names at the end of debug.c.   MLM
 */

# define SCRMINIT (00000000000)
# define BEEN     (00000000001)
# define CANGO    (00000000002)
# define DOOR     (00000000004)
# define HALL     (00000000010)
# define PSD      (00000000020)
# define ROOM     (00000000040)
# define SAFE     (00000000100)
# define SEEN     (00000000200)
# define SEENPOS  (7)
# define DEADEND  (00000000400)
# define STUFF    (00000001000)
# define TRAP     (00000002000)
# define ARROW    (00000004000)
# define TRAPDOR  (00000010000)
# define TELTRAP  (00000020000)
# define GASTRAP  (00000040000)
# define BEARTRP  (00000100000)
# define DARTRAP  (00000200000)
# define WATERAP  (00000400000)
# define MONSTER  (00001000000)
# define WALL     (00002000000)
# define USELESS  (00004000000)
# define SCAREM   (00010000000)
# define STAIRS   (00020000000)
# define RUNOK    (00040000000)
# define BOUNDARY (00100000000)
# define SLEEPER  (00200000000)
# define EVERCLR  (00400000000)
# define CHOKE    (01000000000)

# define TOPW     (0)
# define BOTW     (1)
# define LEFTW    (2)
# define RIGHTW   (3)
# define NOTW     (-1)
# define DOORW    (-2)
# define CORNERW  (-3)

# define ROGINFINITY (10000)

# define NOTAMOVE (-1)

# define FORCE    (1)
# define OPTIONAL (0)

# define NEAR     (0)
# define HERE     (1)

# define NOPRINT  (0)
# define DOPRINT  (1)

# define NOTRUNNING (0)
# define RUNNING    (1)

# define ANY        (0)
# define RUSTPROOF  (1)

/* Types of moves planned by makemove */
# define REUSE    (0)
# define REEVAL   (1)

# define EXPLORE     (1)
# define EXPLORERUN  (2)
# define RUNTODOOR   (3)
# define RUNAWAY     (4)
# define SECRETDOOR  (5)
# define FINDSAFE    (6)
# define GOTOMOVE    (7)
# define ATTACKSLEEP (8)
# define ARCHERYMOVE (9)
# define UNPIN       (10)
# define UNPINEXP    (11)
# define EXPLOREROOM (12)
# define FINDROOM    (13)
# define RESTMOVE    (14)
# define DOWNMOVE    (15)
# define RUNDOWN     (16)
# define NUMMOVES    (17)

/* Version numbers */
# define RV36A    (361)	/* OLDROG: Rogue 3.6 w/o wands */
# define RV36B    (362)	/* CURROG: Rogue 3.6 with wands */
# define RV52A    (521)	/* NEWROG: Rogue 5.2 */
# define RV52B    (522)	/* Rogue 5.2 with maze rooms */
# define RV53A    (531)	/* Rogue 5.3 new monsters */
# define RV54A    (544)	/* Rogue 5.4.4 mystery trap */

/* Ways to spend time */

# define T_OTHER     (0)
# define T_HANDLING  (1)
# define T_FIGHTING  (2)
# define T_RESTING   (3)
# define T_MOVING    (4)
# define T_EXPLORING (5)
# define T_RUNNING   (6)
# define T_GROPING   (7)
# define T_SEARCHING (8)
# define T_DOORSRCH  (9)
# define T_LISTLEN   (10)

/* Bit value for debugging types (for debugging function dwait and
   screen message debugging).  If D_MESSAGE is set the screen must
   be at least 31x80 information displayed to fit.  */

# define D_FATAL   (00001)
# define D_ERROR   (00002)
# define D_WARNING (00004)
# define D_INFORM  (00010)
# define D_SEARCH  (00020)
# define D_BATTLE  (00040)
# define D_MESSAGE (00100)
# define D_PACK    (00200)
# define D_CONTROL (00400)
# define D_SCREEN  (01000)
# define D_MONSTER (02000)
# define D_SAY     (04000)
# define D_ALL     (01777)
# define D_NORMAL  (D_FATAL | D_ERROR)

# define debugon(mask)  (debugging|=(mask))
# define debugoff(mask) (debugging&=(~(mask)))
# define debug(mask)    (debugging&(mask))

/* Parameters for genetic learning, knobs */
# define K_SRCH  (0)
# define K_DOOR  (1)
# define K_REST  (2)
# define K_ARCH  (3)
# define K_EXPER (4)
# define K_RUN   (5)
# define K_WAKE  (6)
# define K_FOOD  (7)
# define MAXKNOB (8)

/* Monster attributes */
# define AWAKE  (1)
# define ASLEEP (2)
# define ALL    (9)
# define HELD   (10)

/* Constants for handling inventory */
# define UNKNOWN  -99

/* Pack item attributes  DR UTexas 25 Jan 84 */
# define KNOWN     (0000000001)
# define CURSED    (0000000002)
# define ENCHANTED (0000000004)
# define PROTECTED (0000000010)
# define UNCURSED  (0000000020)
# define INUSE     (0000000040)
# define WORTHLESS (0000000100)

/* Miscellaneous macros */

# define LETTER(i) ((char)((i)+'a'))
# define DIGIT(c) ((int)((c)-'a'))
# define OBJECT(c) DIGIT(c)
# define ISDIGIT(c) ((c) >= '0' && (c) <= '9')
# define plural(n) ((n)==1 ? "" : "s")
# define ordinal(n) ((((n)/10)%10==1) ? "th": \
			      ((n)%10==1) ? "st": \
			      ((n)%10==2) ? "nd": \
			      ((n)%10==3) ? "rd":"th")
# define ctrl(c) ((c)&037)
# define ESC ctrl('[')
# define NEWLINE ctrl('J')
# define abs(a) ((a) >= 0 ? (a) : -(a))
# define max(a,b) (((a) > (b)) ? (a) : (b))
# define min(a,b) (((a) < (b)) ? (a) : (b))
# define between(v,a,b) ((v) < (a) ? (a) : (v) > (b) ? (b) : (v))
# define percent(v,p) (((v)*(p))/100)
# define SKIPARG while (*++(*argv)); --(*argv)
# define SKIPDIG(s) while (ISDIGIT(*(s))) (s)++
# define SKIPCHAR(c,s) while (*(s)==(c)) (s)++
# define SKIPTO(c,s) \
	{ while (*(s) && *(s)!=(c)) (s)++; if (*(s)==(c)) (s)++; }

/* Utility Macros */

/* onrc - tell if row and col have the proper attributes */
# define onrc(type,r,c) ((type)&scrmap[r][c])

/* on - tell if current position has correct attributes */
# define on(type) onrc(type,atrow,atcol)

/* seerc - is this character at row,col */
# define seerc(ch,r,c) ((ch)==screen[r][c])

/* see - is this character at current position */
# define see(ch) seerc(atrow,atcol)

/* setrc - set attribute at <r,c> */
# define setrc(type,r,c) scrmap[r][c]|=(type)

/* set - set attribute at current position */
# define set(type) setrc(type,atrow,atcol)

/* unsetrc - unset attribute at <r,c> */
# define unsetrc(type,r,c) scrmap[r][c]&= ~(type)

/* unset - unset attribute at current position */
# define unset(type) unsetrc(type,atrow,atcol)

/* Direc - give the vector from an xy difference */
# define direc(r,c) (r>0?(c>0?7:(c<0?5:6)):(r<0?(c>0?1:(c<0?3:2)):(c>0?0:4)))

/* atdrow - gives row of adjacent square given direction */
# define atdrow(dir) (atrow+deltr[(dir)])

/* atdcol - gives col of adjacent square given direction */
# define atdcol(dir) (atcol+deltc[(dir)])

/* Define a more mnemonic string comparison */
# define streq(s1,s2) (strcmp ((s1),(s2)) == 0)

/* Monster value macros */
# define maxhitchar(m) (cosmic ? Level*3/2+6 : monatt[(m)-'A'].maxdam)
# define avghitchar(m) (cosmic ? Level*2/3+4 : monatt[(m)-'A'].expdam)
# define maxhit(m) maxhitchar(mlist[m].chr)
# define avghit(m) avghitchar(mlist[m].chr)     /* times 10 */

/* Item knowledge macros   DR UTexas 25 Jan 84 */

/* itemis - test pack item for traits */
# define itemis(obj,trait) ((obj>=0) ? ((trait)&inven[obj].traits) : 0)

/* remember - set traits for pack item */
# define remember(obj,trait) ((obj>=0) ? (inven[obj].traits|=(trait)) : 0)

/* forget - clear traits for pack item */
# define forget(obj,trait) ((obj>=0) ? (inven[obj].traits&= ~(trait)) : 0)

/* The types of objects */
typedef enum { strange, food, potion, Scroll, wand, ring, hitter,
               thrower, missile, armor, amulet, gold, none
             } stuff;

typedef struct { int   fail, win; } probability;

typedef struct {
  int   count;
  double sum, sumsq, low, high;
} statistic;

typedef struct {
  int   scol, srow;
  stuff what;
} stuffrec;

typedef struct {
  int   mcol, mrow;
  char  chr;
  int   q;
} monrec;

typedef struct { int   expdam, maxdam, mtokill; } attrec;

typedef struct { int gamecnt, gamesum, inittime, timeswritten; } lrnrec;

typedef struct {
  char m_name[NAMSIZ];
  probability wehit, theyhit, arrowhit;
  statistic htokill, damage, atokill;
} ltmrec;

typedef struct {
  stuff type;
  int   count, phit, pdam, charges, traits;
  char  *str;
} invrec;

typedef struct {
  int activity[T_LISTLEN];
  int timestamp;
} timerec;

extern char *knob_name[MAXKNOB];
extern char genelog[100];
extern char genepool[100];

/*
 * global function declarations
 */

/* arms.c */
extern int havearmor (int k, int print, int rustproof);
extern int armorclass (int i);
extern int haveweapon (int k, int print);
extern int weaponclass (int i);
extern int havering (int k, int print);
extern int ringclass (int i);
extern int havebow (int k, int print);
extern int bowclass (int i);
extern int havemissile (void);
extern int havearrow (void);
extern void setbonuses(void);

/* command.c */
extern void move1 (int d);
extern void fmove (int d);
extern void rmove (int count, int d, int mode);
extern void mmove (int d, int mode);
extern void command (int tmode, char *f, ...);
extern char functionchar (char *cmd);
extern int replaycommand (void);
extern void showcommand (char *cmd);

/* config.c */
extern const char *getRgmDir (void);
extern const char *getLockFile (void);

/* database.c */
extern int findentry (char *string);
extern char *findentry_getfakename (char *string, stuff item_type);
extern char *findentry_getrealname (char *string, stuff item_type);
extern void addobj (char *codename, int pack_index, stuff item_type);
extern void useobj (char *string);
extern void infername (char *codename, char *name, stuff item_type);
extern int used (char *codename);
extern int know (char *name);
extern void dumpdatabase (void);

/* debug.c */
extern int dwait(int msgtype, char *f, ...);
extern void promptforflags (void);
extern void timehistory (FILE *f, char sep);
extern void toggledebug (void);

/* debuglog.c */
extern void debuglog_open (const char *log);
extern void debuglog_close (void);
extern void debuglog (const char *fmt, ...);

/* explore.c */
extern int genericinit (void);
extern int gotowards (int r, int c, int running);
extern int sleepvalue (int r, int c, int depth __attribute__ ((__unused__)),
		       int *val, int *avd, int *cont __attribute__ ((__unused__)));
extern int setpsd (int print);
extern int downvalue (int r, int c, int depth __attribute__ ((__unused__)),
		      int *val, int *avd, int *cont __attribute__ ((__unused__)));
extern int expruninit (void);
extern int exprunvalue (int r, int c, int depth, int *val, int *avd, int *cont);
extern int expunpininit (void);
extern int expunpinvalue (int r, int c, int depth, int *val, int *avd, int *cont);
extern int runinit (void);
extern int runvalue (int r, int c, int depth, int *val, int *avd, int *cont);
extern int unpininit (void);
extern int rundoorinit (void);
extern int rundoorvalue (int r, int c, int depth , int *val, int *avd, int *cont);
extern int secret (void);
extern int findroom (void);
extern int exploreroom (void);
extern int doorexplore(void);
extern int findsafe(void);
extern int archmonster (int m, int trns);
extern void unrest (void);
extern int movetorest (void);

/* findscore.c */
extern int findscore (char *rogue, char *roguename);

/* getroguetoken.c */
extern int rogue_log_open (const char *filename);
extern void rogue_log_close (void);
extern void rogue_log_write_command (char c);
extern void open_frogue_debuglog (const char *file);
extern void close_frogue_debuglog (void);
extern void open_frogue_fd (int frogue_fd);
extern char getroguetoken (void);
extern void getoldcommand (char *s);

/* io.c */
extern void getrogue (char *waitstr, int onat);
extern void terpbot (void);
extern void dumpwalls (void);
extern void sendnow (char *f, ...);
extern void rogo_send (char *f, ...);
extern int resend (void);
extern void at (int r, int c);
extern void quitrogue (char *reason, int gld, int terminationtype);
extern void waitfor (char *mess);
extern void say (char *f, ...);
extern void saynow (char *f, ...);
extern void givehelp (void);
extern void pauserogue (void);
extern void getrogver (void);
extern int charsavail (void);
extern void redrawscreen (void);
extern void toggleecho (void);
extern void clearsendqueue (void);
extern void startreplay (FILE **logf, char *logfname);
extern void printsnap (FILE *f);
extern void dosnapshot (void);
extern void clearscreen (void);
extern char *statusline (void);

/* learn.c */
extern void initpool (int k, int m);
extern void analyzepool (int full);
extern void setknobs (int *newid, int *knb, int *best, int *avg);
extern void evalknobs (int gid, int score, int level);
extern FILE *rogo_openlog (char *genelog);
extern void rogo_closelog (void);
extern int readgenes (char *genepool);
extern void writegenes (char *genepool);

/* ltm.c */
extern void mapcharacter (char ch, char *str);
extern int addmonhist (char *monster);
extern int findmonster (char *monster);
extern void saveltm (int score);
extern void restoreltm (void);
extern void dumpmonstertable (void);
extern void analyzeltm (void);

/* mess.c */
extern void terpmes (void);
extern void infer (char *objname, stuff item_type);
extern void killed (char *monster);
extern void summary (FILE *f, char sep);
extern void versiondep (void);

/* monsters.c */
extern char *monname (char m);
extern void addmonster (char ch, int r, int c, int quiescence);
extern void deletemonster (int r, int c);
extern void dumpmonster (void);
extern void sleepmonster (void);
extern void holdmonsters (void);
extern void wakemonster (int dir);
extern int seemonster (char *monster);
extern int seeawakemonster (char *monster);
extern int monsternum (char *monster);
extern void newmonsterlevel (void);
extern int isholder (char *monster);

/* pack.c */
extern char *itemstr (int i);
extern void dumpinv (FILE *f);
extern void removeinv (int pos);
extern void deleteinv (int pos);
extern void resetinv(void);
extern void doresetinv (void);
extern int inventory (char *msgstart, char *msgend);

/* rand.c */
extern void rogo_srand (int seed);
extern int rogo_rand (void);
extern int rogo_randint (int max);

/* replay.c */
extern void positionreplay (void);

/* rooms.c */
extern void newlevel (void);
extern void markmissingrooms (void);
extern int whichroom (int r, int c);
extern void nametrap (int traptype, int standingonit);
extern void findstairs (int notr, int notc);
extern int downright (int *drow, int *dcol);
extern int lightroom (void);
extern int darkroom (void);
extern void currentrectangle (void);
extern void updateat (void);
extern void updatepos (char ch, int row, int col);
extern void mapinfer(void);
extern void markexplored (int row, int col);
extern int isexplored (int row, int col);
extern int haveexplored (int n);
extern void printexplored (void);
extern void inferhall (int r, int c);
extern int canbedoor (int deadr, int deadc);
extern int mazedoor (int row, int col);
extern int nextto (int type, int r, int c);
extern int nexttowall (int r, int c);
extern void dumpmazedoor (void);
extern void foundnew (void);

/* search.c */
extern int makemove (int movetype, int (*evalinit)(void), int (*evaluate)(int, int, int, int*, int*, int*), int reevaluate);
extern int findmove (int movetype, int (*evalinit)(void), int (*evaluate)(int, int, int, int*, int*, int*), int reevaluate);
extern int followmap (int movetype);
extern void cancelmove (int movetype);
extern void setnewgoal (void);

/* scorefile.c */
extern void add_score (char *new_line, char *vers, int ntrm);
extern void dumpscore (char *vers);

/* strategy.c */
extern int strategize (void);

/* stats.c */
extern void clearprob (probability *p);
extern void addprob (probability *p, int success);
extern double prob (probability *p);
extern void parseprob (char *buf, probability *p);
extern void writeprob (FILE *f, probability *p);
extern void clearstat (statistic *s);
extern void addstat (statistic *s, int datum);
extern double mean (statistic *s);
extern double stdev (statistic *s);
extern void parsestat (char *buf, statistic *s);
extern void writestat (FILE *f, statistic *s);

/* survival.c */
extern int markcycles (int print);
extern int runaway (void);
extern int canrun (void);
extern int unpin (void);
extern int backtodoor (int dist);

/* tactics.c */
extern int handlearmor (void);
extern int handleweapon (void);
extern int quaffpotion (void);
extern int readscroll (void);
extern int handlering (void);
extern int findring (char *name);
extern int grope (int turns);
extern int findarrow (void);
extern int checkcango (int dir, int turns);
extern int godownstairs (int running);
extern int plunge (void);
extern int goupstairs (int running);
extern int restup (void);
extern int gotowardsgoal (void);
extern int gotocorner (void);
extern int light (void);
extern int shootindark (void);
extern int dinnertime (void);
extern int trywand (void);
extern int eat (void);

/* things.c */
extern int wear (int obj);
extern int takeoff (void);
extern int wield (int obj);
extern int drop (int obj);
extern int quaff (int obj);
extern int reads (int obj);
extern int point (int obj, int dir);
extern int throw (int obj, int dir);
extern int puton (int obj);
extern int removering (int obj);
extern void initstufflist (void);
extern void addstuff (char ch, int row, int col);
extern void deletestuff (int row, int col);
extern void dumpstuff (void);
extern void display (char *s);
extern int prepareident (int obj, int iscroll);
extern int pickident (void);
extern int unknown (stuff otype);
extern int unidentified (stuff otype);
extern int haveother (stuff otype, int other);
extern int have (stuff otype);
extern int havenamed (stuff otype, char *name);
extern int havewand (char *name);
extern int wearing (char *name);
extern int havemult (stuff otype, char *name, int count);
extern int haveminus (void);
extern int haveuseless (void);
extern int willrust (int obj);
extern int wielding (stuff otype);
extern int hungry (void);
extern int weak (void);
extern int fainting (void);
extern int havefood (int n);

/* titlepage.c */
extern void halftimeshow (int level);

/* utility.c */
extern int rogo_baudrate (void);
extern char *getname (void);
extern FILE *wopen(char *fname, char *mode);
extern int fexists (char *fn);
extern int filelength (char *f);
extern void critical (void);
extern void uncritical (void);
extern void reset_int (void);
extern void int_exit (void (*exitproc)(int));
extern int lock_file (const char *lokfil, int maxtime);
extern void unlock_file (const char *lokfil);
extern void quit (int code, char *fmt, ...);
extern int stlmatch (char *big, char *small);

/* worth.c */
extern int worth (int obj);
extern int useless (int i);
