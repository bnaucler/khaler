/*

	datetime.c - parsing functions for khaler

*/

#include "khaler.h"

char *formatTime(char *unformTime) {

	const char timeDelim = ':';

	char h[2];
	char m[2];
	char *formTime = malloc(sizeof(unformTime));

	for(int a = 0; a < 2; a++) { h[a] = unformTime[a]; }
	for(int a = 0; a < 2; a++) { m[a] = unformTime[a+2]; }

	sprintf(formTime, "%c%c%c%c%c", h[0], h[1], timeDelim, m[0], m[1]);

	return formTime;
}

char *formatDate(char *unformDate) {

	const char dateDelim = '-';

	char y[4];
	char m[2];
	char d[2];
	char *formDate = malloc(sizeof(unformDate) + 2); // Add 2 for delimiters

	for(int a = 0; a < 4; a++) { y[a] = unformDate[a]; }
	for(int a = 0; a < 2; a++) { m[a] = unformDate[a+4]; }
	for(int a = 0; a < 2; a++) { d[a] = unformDate[a+6]; }

	sprintf(formDate, "%c%c%c%c%c%c%c%c%c%c",
			y[0], y[1], y[2], y[3], dateDelim,
			m[0], m[1], dateDelim, d[0], d[1]);

	return formDate;
}
