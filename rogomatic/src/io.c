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
 * io.c:
 *
 * This file contains all of the functions which deal with the real world.
 */

# include <curses.h>
# include <ctype.h>
# include <string.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <time.h>
# include <unistd.h>
# include "install.h"
# include "types.h"
# include "globals.h"
# include "termtokens.h"
# include "getroguetoken.h"

# define READ	0

/*
 * Charonscreen returns the current character on the screen (using
 * curses(3)).  This macro is based on the winch(win) macro.
 */
# define charonscreen(Y,X)	(A_CHARTEXT & mvwinch (stdscr, Y, X))

char *month[] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static char screen00 = ' ';

/* Constants */

# define SENDQ 256

/* The command queue */

char  queue[SENDQ];             /* To Rogue */
int   head = 0, tail = 0;

int s_row1 = -100;  /* start scroll regions way out of bounds */
int s_row2 = 100;   /* start scroll regions way out of bounds */

void
scrollup (void)
{
  int r;
  int c;

  newdoors = doorlist;

  for (r = s_row1; r < s_row2; r++) {
    for (c = 0; c < 80; c++) {
      screen[r][c] = screen[r+1][c];
      updatepos (screen[r][c], r, c);
    }
  }

  for (c = 0; c < 80; c++) {
    screen[s_row2][c] = ' ';
    updatepos (screen[s_row2][c], s_row2, c);
  }
}

void
scrolldown (void)
{
  int r;
  int c;

  for (r = s_row2; r > s_row1; r--) {
    for (c = 0; c < 80; c++) {
      screen[r][c] = screen[r-1][c];
      updatepos (screen[r][c], r, c);
    }
  }

  for (c = 0; c < 80; c++) {
    screen[s_row1][c] = ' ';
    updatepos (screen[s_row1][c], s_row1, c);
  }
}

void
printscreen (void)
{
  int i, j;
  debuglog ("-- cursor  [%2d, %2d] [%c] [%3d] -------------------------------------------------\n", row, col, screen[row][col], screen[row][col]);
  debuglog ("             1111111111222222222233333333334444444444555555555566666666667777777777\n");
  debuglog ("   01234567890123456789012345678901234567890123456789012345678901234567890123456789\n");

  for (i=0; i < 24; ++i) {
    debuglog ("%02d", i);

    if (i >= s_row1 && i <= s_row2) {
      debuglog ("*");
    }
    else {
      debuglog (" ");
    }

    for (j = 0; j < 80; ++j) {
      if (i == row && j == col)
        debuglog ("_");
      else
        debuglog ("%c", screen[i][j]);
    }

    debuglog ("\n");
  }

  debuglog ("--------------------------------------------------------------------------------\n");
}

/*
 * Getrogue: Sensory interface.
 *
 * Handle grungy low level terminal I/O. Getrogue reads tokens from the
 * Rogue process and interprets them, making the screen array an image of
 * the rogue level. Getrogue returns when the string waitstr has been read
 * and either the cursor is on the Rogue '@' or some other condition
 * implies that we have synchronized with Rogue.
 */

getrogue (waitstr, onat)
char *waitstr;                          /* String to synchronize with */
int   onat;                             /* 0 ==> Wait for waitstr
                                           1 ==> Cursor on @ sufficient
                                           2 ==> [1] + send ';' when ever
                                           we eat a --More-- message */

{
  int   botprinted = 0, wasmapped = didreadmap, r, c, pending ();
  register int i, j;
  char  ch, *s, *m, *q, *d, *call;
  int *doors;
  static moved = 0;

  newdoors = doorlist;			/* no new doors found yet */
  atrow0 = atrow; atcol0 = atcol;	/* Save our current posistion */
  s = waitstr;				/* FSM to check for the wait msg */
  m = "re--";				/* FSM to check for '--More--' */
  call = "Call it:";			/* FSM to check for 'Call it:' */
  q = "(* for list): ";			/* FSM to check for prompt */
  d = ")______";			/* FSM to check for tombstone grass */

  if (moved)				/* If we moved last time, put any */
    { sleepmonster (); moved = 0; }	/* Old monsters to sleep */

  /* debugging info */
  if debug(D_MESSAGE) {
    at (28,0);
    clrtoeol ();
    at (27,0);
    clrtoeol ();
    printw("getrogue: waitstr ->%s<-  onat %d.",
           waitstr, onat);
    at (row, col);
    refresh ();
  }

  /* While we have not reached the end of the Rogue input, read */
  /* characters from Rogue and figure out what they mean.       */
  while ((*s) ||
         ((!hasted || version != RV36A) && onat && screen[row][col] != '@')) {
    ch = getroguetoken ();

    if debug(D_MESSAGE) {
      at (28,col);
      printw ("%s", unctrl(ch));
      at (row, col);
      refresh ();
    }

    /* If message ends in "(* for list): ", call terpmes */
    if (ch == *q) { if (*++q == 0) terpmes (); }
    else q = "(* for list): ";

    /* Rogomatic now keys off of the grass under the Tombstone to  */
    /* detect that it has been killed. This was done because the   */
    /* "Press return" prompt only happens if there is a score file */
    /* Available on that system. Hopefully the grass is the same   */
    /* in all versions of Rogue!                                   */
    if (ch == *d) { if (0 == *++d) { addch (ch); deadrogue (); return; } }
    else d = ")_______";

    /* If the message has a more, strip it off and call terpmes */
    if (ch == *m) {
      if (*++m == 0) {
        /* More than 50 messages since last command ==> start logging */
        if (++morecount > 50 && !logging) {
          toggleecho ();
          dwait (D_WARNING, "Started logging --More-- loop.");
        }

        /* Send a space (and possibly a semicolon) to clear the message */
        if (onat == 2) sendnow (" ;");
        else           sendnow (" ");

        /* Clear the --More-- of the end of the message */
        for (i = col - 7; i < col; screen[0][i++] = ' ');

        terpmes ();			/* Interpret the message */
      }
    }
    else m = "re--";

    /* If the message is 'Call it:', cancel the request */
    if (ch == *call) {
      if (*++call == 0) {
        /* Send an escape (and possibly a semicolon) to clear the message */
        if (onat == 2) sendnow ("%c;", ESC);
        else           sendnow ("%c", ESC);
      }
    }
    else call = "Call it:";

    /* Check to see whether we have read the synchronization string */
    if (*s) { if (ch == *s) s++; else s = waitstr; }

    /* Now figure out what the token means */
    switch (ch) {
      case BS_TOK:
        col--;
        debuglog ("BS_TOK      [%2d, %2d] [%c]\n", row, col, screen[row][col]);
        break;

      case CB_TOK:

        for (i =0; i < col; i++) {
          updatepos (' ', row, i);
          screen[row][i] = ' ';
        }

        debuglog ("CB_TOK      [%2d, %2d] [%c]\n", row, col, screen[row][col]);
        break;

      case CE_TOK:

        if (row && row < 23)
          for (i = col; i < 80; i++) {
            updatepos (' ', row, i);
            screen[row][i] = ' ';
          }
        else
          for (i = col; i < 80; i++)
            screen[row][i] = ' ';

        if (row) { at (row, col); clrtoeol (); }
        else if (col == 0) screen00 = ' ';

        debuglog ("CE_TOK      [%2d, %2d] [%c]\n", row, col, screen[row][col]);
        break;

      case CH_TOK:
        debuglog ("CH_TOK [%d, %d] [%d, %d]\n",number1, number2, row, col);
        s_row1 = number1-1;
        s_row2 = number2-1;
        debuglog ("CH_TOK scroll region %d - %d\n",s_row1, s_row2);
        break;

      case CL_TOK:
        clearscreen ();
        row = 0;
        col = 0;
        debuglog ("CL_TOK [%d, %d]\n", row, col);
        break;

      case CM_TOK:
        screen00 = screen[0][0];
        row = number1 - 1;
        col = number2 - 1;
        debuglog ("CM_TOK      [%2d, %2d] [%c]\n", row, col, screen[row][col]);
        break;

      case CR_TOK:
        /* Handle missing '--more--' between inventories  MLM 24-Jun-83 */
        /* --more-- doesn't seem too be missing anymore NYM 3/29/08
         * if (row==0 && screen[0][1]==')' && screen[0][col-1] != '-')
         *   terpmes ();
         */
        col = 0;
        debuglog ("CR_TOK      [%2d, %2d] [%c]\n", row, col, screen[row][col]);
        break;

      case ER_TOK:
        break;

      case LF_TOK:
        row++;
        debuglog ("LF_TOK      check for scroll %d > %d\n",row, s_row2);

        if (row > s_row2) {
          debuglog ("LF_TOK      scroll up\n");
          scrollup ();
        }

        debuglog ("LF_TOK      [%2d, %2d] [%c]\n", row, col, screen[row][col]);
        break;

      case ND_TOK:
        row += number1;
        debuglog ("ND_TOK [%2d] [%2d, %2d] [%c]\n", number1, row, col, screen[row][col]);
        break;

      case SE_TOK:
        debuglog ("SE_TOK\n");
        revvideo = 0;
        standend ();
        break;

      case SO_TOK:
        debuglog ("SO_TOK\n");
        revvideo = 1;
        standout ();
        break;

      case TA_TOK:
        col = 8 * (1 + col / 8);
        debuglog ("TA_TOK      [%2d, %2d] [%c]\n", row, col, screen[row][col]);
        break;

      case EOF:

        if (interrupted) return;

        if (!replaying || !logdigested) { playing = 0; return; }

        saynow ("End of game log, type 'Q' to exit.");
        return;
        break;

      case UP_TOK:
        row--;
        debuglog ("UP_TOK      [%2d, %2d] [%c]\n", row, col,screen[row][col]);
        break;

      case HM_TOK:
        col = 0;
        row = 0;
        debuglog ("HM_TOK      [%2d, %2d] [%c]\n", row, col, screen[row][col]);
        break;

      case NU_TOK:
        row -= number1;
        debuglog ("NU_TOK [%2d] [%2d, %2d] [%c]\n", number1, row, col, screen[row][col]);
        break;

      case NR_TOK:
        col += number1;
        debuglog ("NR_TOK [%2d] [%2d, %2d] [%c]\n", number1, row, col, screen[row][col]);
        break;

      case NL_TOK:
        debuglog ("NL_TOK\n");
        col -= number1;
        debuglog ("NL_TOK [%2d] [%2d, %2d] [%c]\n", number1, row, col, screen[row][col]);
        break;

      case SC_TOK:
        debuglog ("SC_TOK      [%2d, %2d]\n", row, col);
        break;

      case RC_TOK:
        debuglog ("RC_TOK      [%2d, %2d]\n", row, col);
        break;

      case SR_TOK:
        debuglog ("SR_TOK      [%2d, %2d]\n", row, col);
        scrolldown ();
        break;

      default:

        if (ch < ' ') {
          saynow ("Unknown character '\\%o'--more--", ch);
          waitforspace ();
        }
        else if (row) {
          at (row, col);

          if (!emacs && !terse) addch (ch);

          if (row == 23) botprinted = 1;
          else           updatepos (ch, row, col);
        }
        else if (col == 0)
          { screen00 = screen[0][0]; }
        else if (col == 1 && ch == 'l' && screen[0][0] == 'I') {
          screen[0][0] = screen00;

          if (screen00 != ' ') terpmes ();

          screen[0][0] = 'I';
        }

        screen[row][col++] = ch;
        debuglog ("OTHER   [%c] [%2d, %2d] [%c]\n", ch, row, (col-1), screen[row][col-1]);
        break;
    }
  }

  if (botprinted) terpbot ();

  if (atrow != atrow0 || atcol != atcol0) {
    updateat ();	/* Changed position, record the move */
    moved = 1;		/* Indicate that we moved */
    wakemonster (8);	/* Wake up adjacent mean monsters */
    currentrectangle();	/* Keep current rectangle up to date.   LGCH */
  }

  if (!usesynch && !pending ()) {
    usesynch = 1;
    lastobj = NONE;
    resetinv();
  }

  if (version < RV53A && checkrange && !pending ())
    { command (T_OTHER, "Iz"); checkrange = 0; }

  /* If mapping status has changed */
  if (wasmapped != didreadmap) {
    dwait (D_CONTROL | D_SEARCH, "wasmapped: %d   didreadmap: %d",
           wasmapped, didreadmap);

    mapinfer ();
  }

  if (didreadmap != Level) {
    doors = doorlist;

    while (doors != newdoors) {
      r = *doors++; c = *doors++;
      dwait (D_INFORM, "new door at %d, %d", r, c);
      inferhall (r, c);
    }
  }

  if (!blinded)
    for (i = atrow-1; i <= atrow+1; i++)         /* For blanks around the  */
      for (j = atcol-1; j <= atcol+1; j++)       /* rogue...               */
        if (seerc(' ',i,j) && onrc(CANGO,i,j)) { /* CANGO+BLANK impossible */
          unsetrc (CANGO | SAFE, i, j);          /* Infer cant go and...   */
          setnewgoal ();		         /* invalidate the map.    */
        }

  at (row, col);

  if (!emacs && !terse) refresh ();

  printscreen ();

}

/*
 * terpbot: Read the Rogue status line and set the various status
 * variables. This routine depends on the value of version to decide what
 * the status line looks like.
 */

terpbot ()
{
  char sstr[30], modeline[256];
  int oldlev = Level, oldgold = Gold, oldhp = Hp, Str18 = 0;
  extern int geneid;
  register int i, oldstr = Str, oldAc = Ac, oldExp = Explev;

  /* Since we use scanf to read this field, it must not be left blank */
  if (screen[23][78] == ' ') screen[23][78] = 'X';

  /* Read the bottom line, there are three versions of the status line */
  if (version < RV52A) {	/* Rogue 3.6, Rogue 4.7? */
    sscanf (screen[23],
            " Level: %d Gold: %d Hp: %d(%d) Str: %s Ac: %d Exp: %d/%d %s",
            &Level, &Gold, &Hp, &Hpmax, sstr, &Ac, &Explev, &Exp, Ms);
    sscanf (sstr, "%d/%d", &Str, &Str18);
    Str = Str * 100 + Str18;

    if (Str > Strmax) Strmax = Str;
  }
  else if (version < RV53A) {	/* Rogue 5.2 (versions A and B) */
    sscanf (screen[23],
            " Level: %d Gold: %d Hp: %d(%d) Str: %d(%d) Ac: %d Exp: %d/%d %s",
            &Level, &Gold, &Hp, &Hpmax, &Str, &Strmax, &Ac, &Explev, &Exp, Ms);

    Str = Str * 100; Strmax = Strmax * 100;
  }
  else {			/* Rogue 5.3 (and beyond???) */
    sscanf (screen[23],
            " Level: %d Gold: %d Hp: %d(%d) Str: %d(%d) Arm: %d Exp: %d/%d %s",
            &Level, &Gold, &Hp, &Hpmax, &Str, &Strmax, &Ac, &Explev, &Exp, Ms);

    Str = Str * 100; Strmax = Strmax * 100; Ac = 10 - Ac;
  }

  /* Monitor changes in some variables */
  if (screen[23][78] == 'X') screen[23][78] = ' ';	/* Restore blank */

  if (oldlev != Level)       newlevel ();

  if (Level > MaxLevel)      MaxLevel = Level;

  if (oldgold < Gold)        deletestuff (atrow, atcol);

  if (oldhp < Hp)            newring = 1;

  lastdamage = max (0, oldhp - Hp);

  /*
   * Insert code here to monitor changes in attributes due to special
   * attacks					MLM October 26, 1983.
   */

  setbonuses ();

  /*
   * If in special output modes, generate output line
   */

  if ((oldlev != Level || oldgold != Gold || oldstr != Str ||
       oldAc != Ac || oldExp != Explev)) {
    /* Stuff the new values into the argument space (for ps command) */
    sprintf (modeline, "Rgm %d: Id%d L%d %d %d(%d) s%d a%d e%d    ",
             rogpid, geneid, Level, Gold, Hp, Hpmax, Str / 100, 10-Ac, Explev);
    modeline[arglen-1] = '\0';
    strncpy (parmstr, modeline,256);

    /* Handle Emacs and Terse mode */
    if (emacs || terse) {
      /* Skip backward over blanks and nulls */
      for (i = 79; screen[23][i] == ' ' || screen[23][i] == '\0'; i--);

      screen[23][++i] = '\0';

      if (emacs) {
        sprintf (modeline, " %s (%%b)", screen[23]);

        if (strlen (modeline) > 72) sprintf (modeline, " %s", screen[23]);

        fprintf (realstdout, "%s", modeline);
        fflush (realstdout);
      }
      else if (terse && oldlev != Level) {
        fprintf (realstdout, "%s\n", screen[23]);
        fflush (realstdout);
      }
    }
  }
}

/*
 * dumpwalls: Dump the current screen map
 */

dumpwalls ()
{
  register int   r, c, S;
  char ch;

  printexplored ();

  for (r = 1; r < 23; r++) {
    for (c = 0; c < 80; c++) {
      S=scrmap[r][c];
      ch = (ARROW&S)                   ? 'a' :
           (TELTRAP&S)                 ? 't' :
           (TRAPDOR&S)                 ? 'v' :
           (GASTRAP&S)                 ? 'g' :
           (BEARTRP&S)                 ? 'b' :
           (DARTRAP&S)                 ? 's' :
           (WATERAP&S)                 ? 'w' :
           (TRAP&S)                    ? '^' :
           (STAIRS&S)                  ? '>' :
           (RUNOK&S)                   ? '%' :
           ((DOOR+BEEN&S)==DOOR+BEEN)  ? 'D' :
           (DOOR&S)                    ? 'd' :
           ((BOUNDARY+BEEN&S)==BOUNDARY+BEEN) ? 'B' :
           ((ROOM+BEEN&S)==ROOM+BEEN)  ? 'R' :
           (BEEN&S)                    ? ':' :
           (HALL&S)                    ? '#' :
           ((BOUNDARY+WALL&S)==BOUNDARY+WALL) ? 'W' :
           (BOUNDARY&S)                ? 'b' :
           (ROOM&S)                    ? 'r' :
           (CANGO&S)                   ? '.' :
           (WALL&S)                    ? 'W' :
           (S)                         ? 'X' : '\0';

      if (ch) mvaddch (r, c, ch);
    }
  }

  at (row, col);
}

/*
 * sendnow: Send a string to the Rogue process.
 */

/* VARARGS1 */
sendnow (f, a1, a2, a3, a4)
char *f;
int a1, a2, a3, a4;
{
  char cmd[128];
  register char *s = cmd;

  memset(cmd, '\0', 128);

  sprintf (cmd, f, a1, a2, a3, a4);

  while (*s) sendcnow (*s++);
}

/*
 * sendcnow: send a character to the Rogue process. This routine also does
 * the logging of characters in echo mode.
 */

sendcnow (c)
char c;
{
  if (replaying)
    return;

  /* i adjust the constants to fit my specific machine:
      - so i can watch at higher levels (otherwise it's too fast) and
      - so that at lower levels i want my fan speed to stay low.

     if you want to run full blast, make sure the USLEEP global
     constant is 0. */

  if ((USLEEP) && (!noterm))
    if (Level > 20) usleep (USLEEP+(Level * 8000));
    else if (Level > 16) usleep (USLEEP+(Level * 4000));
    else if (Level > 12) usleep (USLEEP+(Level * 2000));
    else usleep (USLEEP);

  rogue_log_write_command (c);

  fprintf (trogue, "%c", c);
}

/*
 * send: add a string to the queue of commands to be sent to Rogue. The
 * commands are sent one at a time by the resend routine.
 */

# define bump(p,sizeq) (p)=((p)+1)%sizeq

/* VARARGS1 */
rogo_send (f, a1, a2, a3, a4)
char *f;
int a1, a2, a3, a4;
{
  char cmd[128];
  register char *s = cmd;

  memset (cmd, '\0', 128);
  sprintf (s, f, a1, a2, a3, a4);

  debuglog ("rogo_send (%s)\n",s);

  for (; *s; bump (tail, SENDQ))
    queue[tail] = *(s++);

  /* Appends null, so resend will treat as a unit */
  queue[tail] = '\0';
  bump (tail, SENDQ);
}

/*
 * resend: Send next block of characters from the queue
 */

resend ()
{
  register char *l=lastcmd;		/* Ptr into last command */

  morecount = 0;			/* Clear message count */

  if (head == tail) return (0);		/* Fail if no commands */

  /* Send all queued characters until the next queued NULL */
  while (queue[head])
    { sendcnow (*l++ = queue[head]); bump (head, SENDQ); }

  bump (head, SENDQ);
  *l = '\0';

  return (1);				/* Return success */
}

/*
 * pending: Return true if there is a command in the queue to be sent to
 * Rogue.
 */

pending ()
{
  return (head != tail);
}

/*
 * at: move the cursor. Now just a call to move();
 */

at (r, c)
int   r, c;
{
  move (r, c);
}

/*
 * deadrogue: Called when we have been killed, it reads the tombstone
 * to see how much we had when we died and who killed us. It then
 * calls quitrogue to handle the termination handshaking and log the
 * game.
 */

# define GOLDROW 15
# define KILLROW 17
# define TOMBCOL 19

deadrogue ()
{
  int    mh;
  char  *killer, *killend;

  printw ("\n\nOops...");
  refresh ();

  sscanf (&screen[GOLDROW][TOMBCOL], "%18d", &Gold);

  killer = &screen[KILLROW][TOMBCOL];
  killend = killer+17;

  while (*killer==' ') ++killer;

  while (*killend==' ') *(killend--) = '\0';

  /* Record the death blow if killed by a monster */
  if ((mh = findmonster (killer)) != NONE) {
    addprob (&monhist[mh].theyhit, SUCCESS);
    addstat (&monhist[mh].damage, Hp);
  }

  quitrogue (killer, Gold, DIED);
}

/*
 * quitrogue: we are going to quit. Log the game and send a \n to
 * the Rogue process, then wait for it to die before returning.
 */

quitrogue (reason, gld, terminationtype)
char *reason;                   /* A reason string for the summary line */
int gld;                       /* What is the final score */
int terminationtype;            /* SAVED, FINSISHED, or DIED */
{
  struct tm *localtime(), *ts;
  long clock;
  char  *k, *r;

  /* Save the killer and score */
  for (k=ourkiller, r=reason; *r && *r != ' '; ++k, ++r) *k = *r;

  *k = '\0';
  ourscore = gld;

  /* Dont need to make up any more commands */
  if (!replaying || !logdigested)
    playing = 0;

  /* Now get the current time, so we can date the score */
  clock = time(&clock);
  ts = localtime(&clock);

  /* Build a summary line */
  sprintf (sumline, "%3s %2d, %4d %-8.8s %7d%s%-17.17s %3d %3d ",
           month[ts -> tm_mon], ts -> tm_mday, 1900 + ts -> tm_year,
           getname (), gld, cheat ? "*" : " ", reason, MaxLevel, Hpmax);

  if (Str % 100)
    sprintf (sumline, "%s%2d.%2d", sumline, Str/100, Str%100);
  else
    sprintf (sumline, "%s  %2d ", sumline, Str/100);

  sprintf (sumline, "%s %2d %2d/%-6d  %d",
           sumline, Ac, Explev, Exp, ltm.gamecnt);

  /* Now write the summary line to the log file */
  at (23, 0); clrtoeol (); refresh ();

  /* 22 is index of score in sumline */
  if (!replaying)
    add_score (sumline, versionstr, (terse || emacs || noterm));

  /* Restore interrupt status */
  reset_int ();

  /* Set the termination message based on the termination method */
  if (stlmatch (reason, "total winner"))
    termination = "victorius";
  else if (stlmatch (reason, "user typing quit"))
    termination = "abortivus";
  else if (stlmatch (reason, "gave up"))
    termination = "inops consilii";
  else if (stlmatch (reason, "quit (scoreboard)"))
    termination = "callidus";
  else if (stlmatch (reason, "saved"))
    termination = "suspendus";

  /* Send the requisite handshaking to Rogue */
  if (terminationtype == DIED)
    if (version == RV54A)
      sendnow ("\n\n");
    else
      sendnow ("\n");
  else if (terminationtype == FINISHED)
    sendnow ("Qy\n");
  else
    sendnow ("Syy"); /* Must send two yesses,  R5.2 MLM */

  /* Wait for Rogue to die */
  wait ((int *) NULL);
}

/*
 * waitfor: snarf characters from Rogue until a string is found.
 *          The characters are echoed to the users screen.
 *
 *          The string must not contain a valid prefix of itself
 *          internally.
 *
 * MLM 8/27/82
 */

waitfor (mess)
char *mess;
{
  register char *m = mess;

  while (*m) {
    if (getroguetoken () == *m) m++;
    else m = mess;
  }
}

/*
 * say: Display a messsage on the top line. Restore cursor to Rogue.
 */

/* VARARGS1 */
say (f, a1, a2, a3, a4, a5, a6, a7, a8)
char *f;
int a1, a2, a3, a4, a5, a6, a7, a8;
{
  char buf[BUFSIZ], *b;

  if (!emacs && !terse) {
    memset (buf, '\0', BUFSIZ);
    sprintf (buf, f, a1, a2, a3, a4, a5, a6, a7, a8);
    at (0,0);

    for (b=buf; *b; b++) printw ("%s", unctrl (*b));

    clrtoeol ();
    at (row, col);
  }
}

/*
 * saynow: Display a messsage on the top line. Restore cursor to Rogue,
 *         and refresh the screen.
 */

/* VARARGS1 */
saynow (f, a1, a2, a3, a4, a5, a6, a7, a8)
char *f;
int a1, a2, a3, a4, a5, a6, a7, a8;
{
  if (!emacs && !terse) {
    say (f, a1, a2, a3, a4, a5, a6, a7, a8);
    refresh ();
  }
}

/*
 * waitforspace: Wait for the user to type a space.
 * Be sure to interpret a snapshot command, if given.
 */

waitforspace ()
{
  char ch;

  refresh ();

  if (!noterm)
    while ((ch = fgetc (stdin)) != ' ')
      if (ch == '/') dosnapshot ();

  at (row, col);
}

/*
 * givehelp: Each time a ? is pressed, this routine prints the next
 * help message in a sequence of help messages. Nexthelp is an
 */

char *nexthelp[] = {
  "Rgm commands: t=toggle run mode, e=logging, i=inventory, -=status    [?]",
  "Rgm commands: <ret>=singlestep, `=summary, /=snapshot, R=replay      [?]",
  "Rgm commands: m=long term memory display, G=display gene settings    [?]",
  "Rogue cmds: S=Save, Q=Quit, h j k l H J K L b n u y N B U Y f s < >  [?]",
  "Wizard: d=debug, !=show items, @=show monsters, #=show level flags   [?]",
  "Wizard: ~=version, ^=bowrank, %%=armorrank, $=weaponrank, ==ringrank  [?]",
  "Wizard: (=database, )=cycles, +=possible secret doors, :=chicken     [?]",
  "Wizard: [=weapstat, ]=rustproof armor, r=resetinv, &=object count    [?]",
  "Wizard: *=toggle blind, C=toggle cosmic, M=mazedoor, A=attempt, {=flags",
  NULL
};

char **helpline = nexthelp;

givehelp ()
{
  if (*helpline == NULL) helpline = nexthelp;

  saynow (*helpline++);
}

/*
 * pauserogue: Wait for the user to type a space and then redraw the
 *             screen. Now uses the stored image and passes it to
 *             curses rather than sending a form feed to Rogue. MLM
 */

pauserogue ()
{
  at (23, 0);
  addstr ("--press space to continue--");
  clrtoeol ();
  refresh ();

  waitforspace ();

  redrawscreen ();
}

/*
 * getrogver: Read the output of the Rogue version command
 *            and set version. RV36B = 362 (3.6 with wands)
 *            and RV52A = 521 (5.2). Note that RV36A is
 *            infered when we send a "//" command to identify
 *            wands.
 *
 * Get version from first 2000 chars of a log file	Feb 9, 1985 - mlm
 */

# define VERMSG	"ersion "

getrogver ()
{
  register char *vstr = versionstr, *m = VERMSG;
  register int cnt = 2000, ch;

  if (replaying) {		/* Look for version string in log */
    while (cnt-- > 0 && *m)
      { if (fgetc (logfile) == *m) m++; else m = VERMSG;}

    if (*m == '\0') {		/* Found VERMSG, get version string */
      while ((ch = fgetc (logfile)) != ' ') *(vstr++) = ch;

      *--vstr = '\0';
    }
    else				/* Use default version */
      { sprintf (versionstr, DEFVER); }

    rewind (logfile);			/* Put log file back to start */
  }

  else {				/* Execute the version command */
    sendnow ("v");
    waitfor ("ersion ");

    while ((ch = getroguetoken ()) != ' ') *(vstr++) = ch;

    *--vstr = '\0';
  }

  if (stlmatch (versionstr, "3.6"))		version = RV36B;
  else if (stlmatch (versionstr, "5.2"))	version = RV52A;
  else if (stlmatch (versionstr, "5.3"))	version = RV53A;
  else if (stlmatch (versionstr, "5.4"))	version = RV54A;
  else saynow ("What a strange version of Rogue! ");
}

/*
 * charsavail: How many characters are there at the terminal? If any
 * characters are found, 'noterm' is reset, since there is obviously
 * a terminal around if the user is typing at us.
 */

charsavail ()
{
  long n;
  int retc;

  if (retc = ioctl (READ, FIONREAD, &n)) {
    saynow ("Ioctl returns %d, n=%ld.\n", retc, n);
    n=0;
  }

  if (n > 0) noterm = 0;

  return ((int) n);
}

/*
 * redrawscreen: Make the users screen look like the Rogue screen (screen).
 */

redrawscreen ()
{
  register int i, j;
  char ch;

  clear ();

  for (i = 1; i < 24; i++) for (j = 0; j < 80; j++)
      if ((ch = screen[i][j]) > ' ') mvaddch(i, j, ch);

  at (row, col);

  refresh ();
}

/*
 * toggleecho: toggle the I/O echo feature. If first time, open the
 * roguelog file.
 */

toggleecho ()
{
  if (replaying) return;

  logging = !logging;

  if (logging) {
    if (! rogue_log_open (ROGUELOG)) {
      logging = !logging;
      saynow ("can't open %s", ROGUELOG);
    }
    else {
      saynow ("Logging to file %s", ROGUELOG);

      if (*versionstr) command (T_OTHER, "v");
    }
  }
  else {
    rogue_log_close ();

    if (playing) saynow ("File %s closed", ROGUELOG);
  }

  if (playing)
    { at (row, col); refresh (); }
}

/*
 * clearsendqueue: Throw away queued Rogue commands.
 */

clearsendqueue ()
{
  head = tail;
}

/*
 * startreplay: Open the log file to replay.
 */

startreplay (logf, logfname)
FILE **logf;
char *logfname;
{
  if ((*logf = fopen (logfname, "r")) == NULL) {
    fprintf (stderr, "Can't open '%s'.\n", logfname);
    exit(1);
  }
}

/*
 * putn: Put 'n' copies of character 'c' on file 'f'.
 */

putn (c, f, n)
register char c;
register FILE *f;
register int n;
{
  while (n--)
    putc (c, f);
}

/*
 * printsnap: print a snapshot to file f.
 */

printsnap (f)
FILE *f;
{
  register int i, j, length;
  struct tm *localtime(), *ts;
  char *statusline();
  long clock;

  /* Now get the current time, so we can date the snapshot */
  clock = time(&clock);
  ts = localtime(&clock);

  /* Print snapshot timestamp */
  fprintf (f, "\nSnapshot taken on %s %d, %d at %02d:%02d:%02d:\n\n",
           month[ts -> tm_mon], ts -> tm_mday, 1900 + ts -> tm_year,
           ts -> tm_hour, ts -> tm_min, ts -> tm_sec);

  /* Print the current map */
  putn ('-', f, 79);
  fprintf (f, "\n");

  for (i = 0; i < 24; i++) {
    for (length = 79; length >= 0 && charonscreen(i,length) == ' '; length--);

    for (j=0; j <= length; j++) fprintf (f, "%c", charonscreen(i,j));

    fprintf (f, "\n");
  }

  putn ('-', f, 79);

  /* Print status variables */
  fprintf (f, "\n\n%s\n\n", statusline ());

  /* Print the inventory */

  dumpinv (f);
  fprintf (f, "\n");
  putn ('-', f, 79);
  fprintf (f, "\n");
}

/*
 * dosnapshot: add a snapshot to the SHAPSHOT file.
 */

dosnapshot ()
{
  if ((snapshot = wopen (SNAPSHOT, "a")) == NULL)
    saynow ("Cannot write file %s.", SNAPSHOT);
  else {
    printsnap (snapshot);
    fclose (snapshot);
    saynow ("Snapshot added to %s.", SNAPSHOT);
  }
}

/*
 * clearscreen: Done whenever a {ff} is sent by Rogue.  This code is
 * separate so it can be called from replay(), since there is an implicit
 * formfeed not recorded in the log file.   MLM
 */

clearscreen ()
{
  register int i, j;

  row = col = 0;
  clear ();
  screen00 = ' ';

  for (i = 0; i < 24; i++)
    for (j = 0; j < 80; j++) {
      screen[i][j] = ' ';
      scrmap[i][j] = SCRMINIT;
    }

  initstufflist ();
  mlistlen = 0;  /* initmonsterlist (); temp hack MLM */
}

/*
 * statusline: Write all about our current status into a string.
 * Returns a pointer to a static area.			MLM
 */

char *
statusline ()
{
  static char staticarea[256];
  register char *s=staticarea;

  sprintf (s, "Status: ");

  if (aggravated)		strcat (s, "aggravated, ");

  if (beingheld)		strcat (s, "being held, ");

  if (blinded)			strcat (s, "blind, ");

  if (confused)			strcat (s, "confused, ");

  if (cosmic)			strcat (s, "cosmic, ");

  if (cursedarmor)		strcat (s, "cursed armor, ");

  if (cursedweapon)		strcat (s, "cursed weapon, ");

  if (doublehasted)		strcat (s, "perm hasted, ");

  if (droppedscare)		strcat (s, "dropped scare, ");

  if (diddrop)		strcat (s, "dropped something, ");

  if (floating)			strcat (s, "floating, ");

  if (hasted)			strcat (s, "hasted, ");

  if (protected)		strcat (s, "protected, ");

  if (redhands)			strcat (s, "red hands, ");

  if (Level == didreadmap)	strcat (s, "mapped, ");

  if (*genocided) sprintf (s, "%sgenocided '%s', ", s, genocided);

  sprintf (s, "%s%d food%s, %d missile%s, %d turn%s, (%d,%d %d,%d) bonus",
           s, larder, plural(larder), ammo, plural(ammo), turns,
           plural(turns), gplushit, gplusdam, wplushit, wplusdam);

  return (s);
}
