/*
 
	khaler.c - main functions for khaler

 */

#include "khaler.h"

// Global constants and variables that might need tweaking
const char delim[] = ":;=\r\n";
const char khal[] = "khal";
const char clear[] = "clear";

// Global variable declarations
char commandString[100];

int currentCal;
char cal[maxCalendars][maxCalName];

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

char *icsFile;

int processInput() {

	char selection = getInput();

	for(;;) {

		if(selection == 'a' || selection == 'A') { 
			sprintf(commandString, "%s import -a %s --batch %s", 
				khal, cal[currentCal], icsFile);
			system(commandString);
			return 0;

		} else if(selection == 'i' || selection == 'I') { 
			sprintf(commandString, "%s interactive", khal);
			system(commandString);
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

	printf("Event name:\t%s\n", eventName);
	printf("Organizer:\t%s (%s)\n", organizerName, organizerEmail);
	printf("Starting at:\t%s\t%s\n", startDate, startTime);
	printf("Ending at:\t%s\t%s\n", endDate, endTime);
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

	sprintf(commandString, "%s agenda --days %d %s", khal, showDays, startDate);
	system(commandString);

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

int main(int argc, char *argv[]) {

	int returnCode;
	if((returnCode = init(argc)) != 0) return returnCode;

	int maxChars = 1024;
	char buf[maxChars];
	char *token;
	char *stringPointer;

	icsFile = argv[1];
	FILE* file = fopen(icsFile, "r");

	if(file == NULL) { 
		printf("File %s could not be opened.\n", icsFile);
		return 1; }
	else { 
		while(fgets(buf, maxChars, file)){

			if((stringPointer = strcasestr(buf, nameGrepKey))) { 
				token = strstr(stringPointer, ":");
				token++;
				strcpy(eventName, token);
			}
			
			if((stringPointer = strcasestr(buf, organizerGrepKey))) { 
				stringPointer = strstr(stringPointer, "CN=");
				token = strtok(stringPointer, delim);
				strcpy(organizerName, strtok(NULL, delim));
			}

			if((stringPointer = strcasestr(buf, organizerEmailGrepKey))) { 
				token = strtok(stringPointer, delim);
				strcpy(organizerEmail, strtok(NULL, delim));
			}

			if((stringPointer = strcasestr(buf, startGrepKey))) { 
				token = strstr(stringPointer, ":");
				token++;
				if(token) {
					strcpy(startDate, formatDate(strtok(token, "T")));
					strcpy(startTime, formatTime(strtok(NULL,delim)));
				}
			}

			if((stringPointer = strcasestr(buf, endGrepKey))) { 
				token = strstr(stringPointer, ":");
				token++;
				if(token) {
					strcpy(endDate, formatDate(strtok(token, "T")));
					strcpy(endTime, formatTime(strtok(NULL,delim)));
				}
			}
		}
	}
	
	return printAll();
} 
