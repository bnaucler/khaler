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

#define maxCalendars 10			// Anyone with more than ten calendars is crazy
#define maxCalName 20			// Max characters in calendar name
#define showDays 3				// # of days when showing agenda

extern int currentCal;
extern char cal[maxCalendars][maxCalName];

// Forward declarations
char getch();
int printAll();
int printCalendars();
char *formatDate(char *unformDate);
char *formatTime(char *unformTime);
int readKhalConfig();
char getInput();
char getCalInput();

#endif
