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

#define maxcal			10			// Anyone with more than ten calendars is crazy
#define maxcalname		20			// Max characters in calendar name
#define maxname			100			// Longest name of person or event
#define maxemail		100			// Longest email address
#define maxatts			10			// We don't want to list more than 10 people
#define shdays			3			// # of days when showing agenda
#define sbch			1024		// Size of small file buffer
#define bbch			4096		// Size of big file buffer
#define tlen			8			// Length of time string
#define dlen			12			// Length of date string

// Text color definitions
#define WHT				"\033[1m\033[37m"
#define RED				"\x1B[2m\033[31m"
#define GREEN			"\x1B[2m\033[32m"
#define YELLOW			"\x1B[2m\033[33m"
#define RESET			"\033[0m"

// Global variables
extern int ccal;
extern char cal[maxcal][maxcalname];

extern char evname[maxname];
extern char location[maxname];
extern char orgname[maxname];
extern char orgemail[maxemail];
extern char descr[bbch];
extern char stime[tlen];
extern char etime[tlen];
extern char sdate[dlen];
extern char edate[dlen];

extern char attname[maxatts][maxname];
extern char attemail[maxatts][maxname];
extern int attrsvp[maxatts];
extern int numatts;
extern int curatt;

// Forward declarations
char getch();
int termcol();
int termrow();

int printAll();
int printCalendars();
int readKhalConfig();

void parseBuf(char *bbuf);

char getInput();
char getCalInput();

char *formatDate(char *unformDate);
char *formatTime(char *unformTime);

int cchar(char *buf, char ch);
char *remchar(char *input, char ch);
char *remstr(char *input, char *rem);
char *repchar(char input[], char rch, char nch);
char *repstr(char input[], char ostr[], char nstr[]);
char *breakline(char *ostr, int blen);
char *remtrail(char *buf);

#endif
