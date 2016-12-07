/*

	khaler - v0.3
	a parser for .ics files and adding to khal

	See README.md for more information.

*/

#ifndef KHALER_HEAD
#define KHALER_HEAD
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <string.h>
#include <ctype.h>
#include <termios.h>
#include <time.h>

#define icsmaxlen		75			// According to RFC
#define rflen			10			// File name length of response file
#define maxcal			10			// Anyone with more than ten calendars is crazy
#define maxcalname		20			// Max characters in calendar name
#define maxname			100			// Longest name of person or event
#define maxoemail		10			// Max number of own email addresses to store
#define maxemail		100			// Longest email address
#define maxpath			200			// Longest file path
#define maxopts			20			// Maximal number of selection options
#define maxatts			10			// We don't want to list more than 10 people
#define shdays			3			// # of days when showing agenda
#define mbch			256			// Size of micro buffer
#define sbch			1024		// Size of small buffer
#define bbch			4096		// Size of big buffer
#define tbch			8192		// Size of terminal buffer
#define klen			20			// Max size of variable grep key

#define emailkey		"mailto"
#define namekey			"SUMMARY"
#define lockey			"LOCATION"
#define attkey			"ATTENDEE"
#define orgkey			"ORGANIZER"
#define startkey		"DTSTART"
#define endkey			"DTEND"
#define zonekey			"TZOFFSETTO"
#define descrkey		"DESCRIPTION"
#define rsvpkey			"PARTSTAT"
#define evin			"BEGIN:VEVENT"
#define evout			"END:VEVENT"

#define statusn			"NEEDS-ACTION"
#define statusa			"ACCEPTED"
#define statusd			"DECLINED"
#define statust			"TENTATIVE"

#define cfilename		".khaler"
#define deftmpdir		"/tmp"

#define delim			":;=\r\n"	// General set of delimiters

// Text color definitions
#define WHT				"\033[1m\033[37m"
#define RED				"\x1B[1m\033[31m"
#define GREEN			"\x1B[1m\033[32m"
#define YELLOW			"\x1B[1m\033[33m"
#define RESET			"\033[0m"

// Global variables
extern char khal[];
extern char khalconf[];
extern char pager[maxpath];
extern char editor[maxpath];
extern char tmpdir[maxpath];
extern char sendstr[sbch];

extern int ccal;
extern char cal[maxcal][maxcalname];

extern char tbuf[tbch];
extern char evname[maxname];
extern char location[maxname];
extern char orgname[maxname];
extern char orgemail[maxemail];
extern char ownemail[maxoemail][maxemail];
extern char descr[bbch];
extern int sttime;
extern int etime;
extern int syear;
extern int smonth;
extern int sday;
extern int eyear;
extern int emonth;
extern int eday;

extern char attname[maxatts][maxname];
extern char attemail[maxatts][maxname];
extern int attrsvp[maxatts];
extern int numatts;
extern int curatt;
extern int curoemail;

extern char tzin[klen];
extern char tzout[klen];
extern bool intz;
extern int toff;
extern int respemail;

extern int debug;

// Forward declarations
int termcol();
int termrow();

int printall();
void printcal();

int readconfig();
int readkhalconfig();
int readmuttconfig();

void parseBuf(char *bbuf);

void cpr(char *buf);
int bpr(int bufsize, const char *format, ...);
int setrespemail();
int writefile(char *fname, char *text);
char *getl(char *ret, size_t len);
int dupecheck();
char *gettmpfname(char *ret, int randlen, int maxlen);

char getin();
char getcalin();

int toffset();
int isdls();
void settzkeys(int dls);
int contime(char *uftime);
void consdate(char *ufdate);
void conedate(char *ufdate);

int cchar(char *buf, char ch);
char *remchar(char *input, char ch);
char *remstr(char *input, char *rem);
char *repchar(char input[], char rch, char nch);
char *repstr(char input[], char ostr[], char nstr[]);
char *breakline(char *ostr, int blen);
char *remtrail(char *buf);
char *randstr(const int len);

#endif
