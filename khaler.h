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
#include <termios.h>

char getch();
int printAll();
int printCalendars();
char *formatDate(char *unformDate);
char *formatTime(char *unformTime);
char *readKhalConfig();

#endif
