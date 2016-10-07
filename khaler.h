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
#define maxname 100				// Longest name of person or event
#define maxemail 100			// Longest email address
#define maxatts 10				// We don't want to list more than 10 people
#define showDays 3				// # of days when showing agenda

// Text color definitions
#define WHT "\033[1m\033[37m"
#define RESET "\033[0m"

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
