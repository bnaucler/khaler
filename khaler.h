/*

	khaler - v0.2
	a parser for .ics files and adding to khal

	See README.md for more information.

*/

#ifndef KHALER_HEAD
#define KHALER_HEAD

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <string.h>
#include <ctype.h>
#include <termios.h>
#include <time.h>

#define maxcal			10			// Anyone with more than ten calendars is crazy
#define maxcalname		20			// Max characters in calendar name
#define maxname			100			// Longest name of person or event
#define maxemail		100			// Longest email address
#define maxpath			200			// Longest file path
#define maxatts			10			// We don't want to list more than 10 people
#define shdays			3			// # of days when showing agenda
#define sbch			1024		// Size of small file buffer
#define bbch			4096		// Size of big file buffer
#define klen			20			// Max size of variable grep key

// Text color definitions
#define WHT				"\033[1m\033[37m"
#define RED				"\x1B[2m\033[31m"
#define GREEN			"\x1B[2m\033[32m"
#define YELLOW			"\x1B[2m\033[33m"
#define RESET			"\033[0m"

// Global variables
extern char khal[];
extern char khalconf[];

extern int ccal;
extern char cal[maxcal][maxcalname];

extern char evname[maxname];
extern char location[maxname];
extern char orgname[maxname];
extern char orgemail[maxemail];
extern char ownemail[maxemail];
extern char descr[bbch];
extern int stime;
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

extern char tzin[klen];
extern char tzout[klen];
extern bool intz;
extern int toff;

// Forward declarations
char getch();
int termcol();
int termrow();

int printAll();
int printcal();
int readconfig();
int readKhalConfig();
int readmuttconfig();

void parseBuf(char *bbuf);

char getInput();
char getCalInput();

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

#endif
