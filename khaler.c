/*

	khaler.c - v0.2
	a parser for .ics files and adding to khal

	WRITTEN BY:
	Björn Westerberg Nauclér 2016
	mail@bnaucler.se

	LICENSE: 
	None. Do whatever you want with it,
	and let me know if you find it useful.

	README:
	It's simple enough to use. Takes an .ics file
	as the only argument and presents an 
	interactive menu. khal needs the following
	configuration under [locale]:
	
	timeformat= %H:%M
	longdateformat= %Y-%m-%d
	(or edit formatDate() and formatTime())

	TODO:
	* Use khal import builtin file parsing
	* Proper parsing of organizer name and email
	* Convert to strstr()
	* Proper makefile
	* Merge formatTime() and formatDate()
	* Selection parsing as function
	* Move all of this crap to README.md
	* Support for multiple calendars

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <termios.h>

// Global constants that might need tweaking
const char timeDelim = ':';
const char dateDelim = '-';
const char delim[] = ":;=\r\n";
const char khal[] = "khal";
const char clear[] = "clear";
const int numObjects = 4;
const int showDays = 3;

// Not my code. Thank you whoever you are.
static struct termios old, new;

char getch() {

  char inputChar;

  tcgetattr(0, &old);
  new = old;
  new.c_lflag &= ~ICANON;
  new.c_lflag &= 0 ? ECHO : ~ECHO;
  tcsetattr(0, TCSANOW, &new);
  inputChar = getchar();
  tcsetattr(0, TCSANOW, &old);

  return inputChar;
}

// Loop and first syntax check
char getInput() {

	char ch;
	
	for(;;) {
		ch = getch();
		if(ch == 'q' || ch == 'Q' || ch == 'a' || 
			ch == 'A' || ch == 'i' || ch == 'I') { 
			return ch;
		}
	}
}

void printMenu() {

	printf("\n--\n");
	printf("a: Add to khal\n");
	printf("i: Launch ikhal\n");
	printf("q: Quit\n");
}

char *formatTime(char *unformTime) {

	char h[2];
	char m[2];
	char *formTime = malloc(sizeof(unformTime));

	for(int a = 0; a < 2; a++) { h[a] = unformTime[a]; }
	for(int a = 0; a < 2; a++) { m[a] = unformTime[a+2]; }

	sprintf(formTime, "%c%c%c%c%c", h[0], h[1], timeDelim, m[0], m[1]);

	return formTime;
}

char *formatDate(char *unformDate) {

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

int main(int argc, char *argv[]) {

	if(argc != 2) { 
		printf("Usage: khaler <filename.ics>\n");
		return 1;
	}

	int maxChars = 1024;
	char buf[maxChars];
	char *token;
	char *stringPointer;
	char commandString[100];
	char selection;

	char nameGrepKey[] = "SUMMARY";
	char organizerGrepKey[] = "ORGANIZER";
	char organizerEmailGrepKey[] = "mailto";
	char startGrepKey[] = "DTSTART";
	char endGrepKey[] = "DTEND";

	char eventName[100];
	char organizerName[100];
	char organizerEmail[100];
	char startTime[8];
	char endTime[8];
	char startDate[12];
	char endDate[12];

	char *icsFile = argv[1];
	FILE* file = fopen(icsFile, "r");

	if(file == NULL) { 
		printf("File %s could not be opened.\n", icsFile);
		return 1; }
	else { 
		while(fgets(buf, maxChars, file)){

			if((stringPointer = strstr(buf, nameGrepKey))) { 
				token = strstr(stringPointer, ":");
				token++;
				strcpy(eventName, token);
			}
			
			if((stringPointer = strstr(buf, organizerGrepKey))) { 
				stringPointer = strstr(stringPointer, "CN=");
				token = strtok(stringPointer, delim);
				strcpy(organizerName, strtok(NULL, delim));
			}

			if((stringPointer = strstr(buf, organizerEmailGrepKey))) { 
				token = strtok(stringPointer, delim);
				strcpy(organizerEmail, strtok(NULL, delim));
			}

			if((stringPointer = strstr(buf, startGrepKey))) { 
				token = strstr(stringPointer, ":");
				token++;
				if(token) {
					strcpy(startDate, formatDate(strtok(token, "T")));
					strcpy(startTime, formatTime(strtok(NULL,delim)));
				}
			}

			if((stringPointer = strstr(buf, endGrepKey))) { 
				token = strstr(stringPointer, ":");
				token++;
				if(token) {
					strcpy(endDate, formatDate(strtok(token, "T")));
					strcpy(endTime, formatTime(strtok(NULL,delim)));
				}
			}
		}
	}
	
	system(clear);

	printf("Event name:\t%s\n", eventName);
	printf("Organizer:\t%s\t%s\n", organizerName, organizerEmail);
	printf("Starting at:\t%s\t%s\n", startDate, startTime);
	printf("Ending at:\t%s\t%s\n", endDate, endTime);

	printf("\n");
	
	sprintf(commandString, "%s agenda --days %d %s", khal, showDays, startDate);
	system(commandString);
	printMenu();
	selection = getInput();

	for(;;) {
		if(selection == 'a' || selection == 'A') { 
			sprintf(commandString, "%s new %s %s-%s %s %s", khal,
				startDate, startTime, endDate, endTime, eventName);
			system(commandString);
			return 0;
		} else if(selection == 'i' || selection == 'I') { 
			sprintf(commandString, "%s interactive", khal);
			system(commandString);
			selection = getInput();
		} else { return 0; }
	}
} 
