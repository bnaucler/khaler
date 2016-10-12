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
#define RED				"\x1B[1m\033[31m"
#define RESET			"\033[0m"

extern int ccal;
extern char cal[maxcal][maxcalname];

// Forward declarations
char getch();
int printAll();
int printCalendars();
char *formatDate(char *unformDate);
char *formatTime(char *unformTime);
int readKhalConfig();
char getInput();
char getCalInput();
char *remchar(char *input, char ch);
char *remstr(char *input, char *rem);
char *repchar(char input[], char rch, char nch);
char *repstr(char input[], char ostr[], char nstr[]);

#endif
