/*
 
	readhkalconfig.c - file operations for khaler

 */

#include "khaler.h"

char returnString[100];

char *readKhalConfig() {
		
	int maxChars = 1024;

	char buf[maxChars];
	char *token;
	char *khalConfig = malloc(100);
	char defaultCalKey[] = "default_calendar";

	sprintf(khalConfig, "%s/.config/khal/khal.conf", getenv("HOME"));

	FILE* file = fopen(khalConfig, "r");

	if(file == NULL) { 
		printf("File %s could not be opened.\n", khalConfig);
		return NULL; }
	else {
		while(fgets(buf, maxChars, file)){

			if((token = strstr(buf, "[["))){
				if(token) { 
					token = token + 2;
					token = strtok(token, "]]");
					strcat(returnString, token);
					strcat(returnString, ":");
				}
			}

			// Find default and format
			if((token = strstr(buf, defaultCalKey))) {
				int a = strlen(defaultCalKey);
				while(token[a] == ' ' || token[a] == '=')  a++;  
				token = strtok(token, "\n");
				if(token) strcpy(currentCal, token + a); 
			}
		}

		// Stripping the trailing delimiter
		char *lastPointer = returnString;
		lastPointer[strlen(lastPointer) - 1] = 0;

		// Setting first cal to current if no default has been set
		if(strlen(currentCal) == 0) { strcpy(currentCal, strtok(returnString, ":")); }

		return returnString;
	} 
}
