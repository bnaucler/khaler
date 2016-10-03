#include "khaler.h"

// Global constants and variables that might need tweaking
const char delim[] = ":;=\r\n";
const char khal[] = "khal";
const char clear[] = "clear";
const int numObjects = 4;
const int showDays = 3;

char currentCal[50] = "work";

// Global variable declarations
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

// Read one char without echo via getch
char getInput() {

	char ch;
	
	for(;;) {
		ch = getch();
		if(ch == 'q' || ch == 'Q' || ch == 'a' || 
			ch == 'A' || ch == 'i' || ch == 'I' ||
			ch == 's' || ch == 'S') { 
			return ch;
		}
	}
}

int processInput() {

	selection = getInput();

	for(;;) {

		if(selection == 'a' || selection == 'A') { 
			sprintf(commandString, "%s new -a %s %s %s-%s %s %s", khal,
				currentCal, startDate, startTime, endDate, endTime, eventName);
			system(commandString);
			return 0;

		} else if(selection == 'i' || selection == 'I') { 
			sprintf(commandString, "%s interactive", khal);
			system(commandString);
			selection = getInput();

		} else if(selection == 's' || selection == 'S') { 
			printf("Enter name of calendar to use: ");
			scanf("%s", currentCal);
			return printAll();
			// selection = getInput();
		}

		else { return 0; }
	}
}


void printEvent() {

	printf("Event name:\t%s\n", eventName);
	printf("Organizer:\t%s - %s\n", organizerName, organizerEmail);
	printf("Starting at:\t%s\t%s\n", startDate, startTime);
	printf("Ending at:\t%s\t%s\n", endDate, endTime);
	printf("\n--\n\n");
}

void printMenu() {

	printf("\n--\n\n");
	printf("Current calendar: %s\n\n", currentCal);
	printf("a: Add to khal\n");
	printf("s: Select calendar\n");
	printf("i: Launch ikhal\n");
	printf("q: Quit\n");
}

int printAll() {

	system(clear);
	printEvent();

	sprintf(commandString, "%s agenda --days %d %s", khal, showDays, startDate);
	system(commandString);

	printMenu();

	return processInput();
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

	char *icsFile = argv[1];
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
