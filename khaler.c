/*
 
	khaler.c - main functions for khaler

 */

#include "khaler.h"

// Global constants and variables that might need tweaking
const char delim[] = ":;=\r\n";
const char khal[] = "khal";
const char clear[] = "clear";

// Global variable declarations
char cstr[100];

int currentCal;
char cal[maxCalendars][maxCalName];

char evname[maxname];
char organizerName[maxname];
char organizerEmail[maxemail];
char stime[8];
char etime[8];
char sdate[12];
char edate[12];
char descr[4096];

char attname[maxatts][maxname];
char attemail[maxatts][maxname];
int curatt = 0;

char *icsFile;

int processInput() {

	char selection = getInput();

	for(;;) {

		if(selection == 'a' || selection == 'A') { 
			sprintf(cstr, "%s import -a %s --batch %s", 
				khal, cal[currentCal], icsFile);
			system(cstr);
			return 0;

		} else if(selection == 'i' || selection == 'I') { 
			sprintf(cstr, "%s interactive", khal);
			system(cstr);
			selection = getInput();

		} else if(selection == 's' || selection == 'S') { 
			printCalendars();
			currentCal = getCalInput();
			return printAll();
		}

		else { return 0; }
	}
}

void printEvent() {

	printf(WHT "Event name: " RESET "\t%s\n", evname);
	printf(WHT "Organizer:" RESET "\t%s (%s)\n", organizerName, organizerEmail);
	printf(WHT "Starting at:" RESET "\t%s\t%s\n", sdate, stime);
	printf(WHT "Ending at:" RESET "\t%s\t%s\n\n", edate, etime);
	printf(WHT "Attendee list:" RESET "\n");

	for(int a = 0; a < curatt; a++) {
		if(strlen(attname[a]) == 0) strcpy(attname[a], "Unknown name");
		printf("%s (%s)\n", attname[a], attemail[a]);
	}

	printf(WHT "\nDescription:\n" RESET);
	printf("%s\n", descr);

	printf("\n--\n\n");
}

void printMenu() {

	printf("\n--\n\n");
	printf("a: Add to khal\n");
	printf("s: Select calendar (%s)\n", cal[currentCal]);
	printf("i: Launch ikhal\n");
	printf("q: Quit\n");
}

int printCalendars() {
	
	printf("\nSelect calendar to use:\n");

	for(int a = 0; a < maxCalendars; a++) { 
		if(strlen(cal[a]) != 0) {
			printf("%d: %s | ", a, cal[a]); 
		}
	}
	printf("q: Abort");
	printf("\n");

	return 0;
}

int printAll() {

	system(clear);
	printEvent();

	sprintf(cstr, "%s agenda --days %d %s", khal, showDays, sdate);
	system(cstr);

	printMenu();

	return processInput();
}

int init(int argc) {

	char checkKhalString[100];
	sprintf(checkKhalString, "%s --version > /dev/null", khal);

	if(system(checkKhalString) != 0) {
		printf("khal not found! Make sure it is installed and in your path.\n");
		return 1;
	}

	if(argc != 2) { 
		printf("Usage: khaler <filename.ics>\n");
		return 1;
	}

	int isCal = readKhalConfig();
	if(isCal != 0) { return 1; }

	return 0;
}

void parseBuf(char *bbuf) {

	char nameGrepKey[] = "SUMMARY";
	char attendeeGrepKey[] = "ATTENDEE";
	char organizerGrepKey[] = "ORGANIZER";
	char emailGrepKey[] = "mailto";
	char startGrepKey[] = "DTSTART";
	char endGrepKey[] = "DTEND";
	char descrkey[] = "DESCRIPTION";

	char *token;

	char bbuf2[4096];
	strcpy(bbuf2, bbuf); // Ugly hack perhaps. There should be a better way.

	if((token = strcasestr(bbuf, nameGrepKey))) { 
		token = strstr(token, ":");
		token++;
		strcpy(evname, token);
	}
	
	if((token = strcasestr(bbuf, organizerGrepKey))) { 
		token = strstr(token, "CN=");
		token = strtok(token, delim);
		strcpy(organizerName, strtok(NULL, delim));
		token = strcasestr(bbuf2, emailGrepKey);
		token = strtok(token, delim);
		if(token) strcpy(organizerEmail, strtok(NULL, delim));
	}

	if((token = strcasestr(bbuf, attendeeGrepKey))) { 
		token = strstr(token, "CN=");
		token = strtok(token, delim);
		if(token)strcpy(attname[curatt], strtok(NULL, delim));
		token = strcasestr(bbuf2, emailGrepKey);
		token = strtok(token, delim);
		if(token) strcpy(attemail[curatt], strtok(NULL, delim));
		curatt++;
	}

	if((token = strcasestr(bbuf, descrkey))) { 
		token = strstr(token, ":");
		token++;
		// Ugly hack for MS Exchange
		if(strcasecmp(token, "REMINDER") != 0) strcpy(descr, token);
	}
	
	if((token = strcasestr(bbuf, startGrepKey))) { 
		token = strstr(token, ":");
		token++;
		if(token) {
			strcpy(sdate, formatDate(strtok(token, "T")));
			strcpy(stime, formatTime(strtok(NULL,delim)));
		}
	}

	if((token = strcasestr(bbuf, endGrepKey))) { 
		token = strstr(token, ":");
		token++;
		if(token) {
			strcpy(edate, formatDate(strtok(token, "T")));
			strcpy(etime, formatTime(strtok(NULL,delim)));
		}
	}

}

int main(int argc, char *argv[]) {

	int returnCode;
	if((returnCode = init(argc)) != 0) return returnCode;

	int sbChars = 1024;
	int bbChars = 4096;
	char sbuf[sbChars];
	char bbuf[bbChars];

	icsFile = argv[1];
	FILE* file = fopen(icsFile, "r");

	if(file == NULL) { 
		printf("File %s could not be opened.\n", icsFile);
		return 1; }
	else { 
		while(fgets(sbuf, sbChars, file)){
			strtok(sbuf, "\n");

			// initial whitespace indicates broken line
			if(isspace(sbuf[0])) {
				memmove(sbuf, sbuf+1, strlen(sbuf));
				strcat(bbuf, sbuf);
			} else {
				parseBuf(bbuf);
				strcpy(bbuf, sbuf);
			}
		}
	}

	return printAll();
} 
