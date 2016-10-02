/*

	khaler.c - v0.1
	a parser for .ics files and adding to khal

	Written by:
	Björn Westerberg Nauclér 2016
	mail@bnaucler.se

	License: 
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

	Also, check location of khal executable
	before compiling.

	TODO:
	* Reading time & date format from khal config
	* Input syntax check
	* Better parsing of organizer name
	* Proper makefile
	* Merge formatTime() and formatDate()
	* Selection parsing as function

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <termios.h>

// Variables that might need tweaking
char timeDelim = ':';
char dateDelim = '-';
const char delim[] = ":;\r\n";
const char khal[] = "/usr/local/bin/khal";
const int numObjects = 4;

// Yes there is probably a smarter way to do this.
typedef struct {
	char name[30];
	char grepKey[20];
	char content[200];
	char date[12];			// khal parsing possible without separating
	char time[8];			// date and time. Done for posterity.
	int depth;
	bool timeObject;
} icsObject;

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
			ch == 'A' || ch == 'c' || ch == 'C') { 
			return ch;
		}
	}
}

void printObject(icsObject *currentObject) {

	if(currentObject->timeObject) {
		printf("%s: %s %s\n", currentObject->name, 
			currentObject->date, currentObject->time);
	} else { 
		printf("%s: %s\n", currentObject->name, currentObject->content); 
	}
}

void printMenu() {

	printf("\n--\n");
	printf("a: Add to khal\n");
	printf("c: Check khal\n");
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

int grepFor(char *inputToken, icsObject *currentObject) {

	if(strcmp(inputToken, currentObject->grepKey) == 0) {
		for(int a = 0; a < currentObject->depth; a++) { 
			inputToken = strtok(NULL, delim);
		}
		if(inputToken != NULL) { 
			if(currentObject->timeObject) { 
				strcpy(currentObject->date, formatDate(strtok(inputToken, "T")));
				strcpy(currentObject->time, formatTime(strtok(NULL,delim)));
			} else { strcpy(currentObject->content, inputToken); }
		}
	} 
	return 0;
}

int main(int argc, char *argv[]) {

	int maxChars = 1024;
	char buf[maxChars];
	char *token;
	char khalAddString[100];
	char selection;

	icsObject object[numObjects];
	icsObject *objectPointer = malloc(sizeof(icsObject));

	strcpy(object[0].name, "Event name");
	strcpy(object[0].grepKey, "SUMMARY");
	object[0].depth = 1;
	
	strcpy(object[1].name, "Organizer");
	strcpy(object[1].grepKey, "ORGANIZER");
	object[1].depth = 2;

	strcpy(object[2].name, "Start time");
	strcpy(object[2].grepKey, "DTSTART");
	object[2].depth = 2;
	object[2].timeObject = 1;

	strcpy(object[3].name, "End time");
	strcpy(object[3].grepKey, "DTEND");
	object[3].depth = 2;
	object[3].timeObject = 1;

	char *icsFile = argv[1];
	FILE* file = fopen(icsFile, "r");

	if(file == NULL) { 
		printf("File %s could not be opened.\n", icsFile);
		return 1; }
	else { 
		while(fgets(buf, maxChars, file)){
			token = strtok(buf, delim);
			if(token) { // To avoid segfault on empty lines
				for(int a = 0; a < numObjects; a++) {
					if(strcasecmp(token, object[a].grepKey) == 0) { 
						objectPointer = &object[a];
						grepFor(buf, objectPointer); 
					}
				}
			}
		}
	}
	
	for(int a = 0; a < numObjects; a++) {
		objectPointer = &object[a];
		printObject(objectPointer);
	}
	
	printMenu();
	selection = getInput();

	if(selection == 'a' || selection == 'A') { 
		sprintf(khalAddString, "%s new %s %s-%s %s %s", khal,
			object[2].date, object[2].time,
			object[3].date, object[3].time,
			object[0].content);
		system(khalAddString);
		return 0;
	} else if(selection == 'c' || selection == 'C') { 
		system(khal);
		return 0;
	} else { return 0; }
} 
