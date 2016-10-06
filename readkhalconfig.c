/*
 
	readhkalconfig.c - file operations for khaler

 */

	
#include "khaler.h"

int readKhalConfig() {
		
	int maxChars = 1024;
	int calCounter = 0;
	char khalConfig[100];

	char buf[maxChars];
	char *token;
	const char defaultCalKey[] = "default_calendar";

	sprintf(khalConfig, "%s/.config/khal/khal.conf", getenv("HOME"));

	FILE* file = fopen(khalConfig, "r");

	if(file == NULL) { 
		printf("File %s could not be opened.\n", khalConfig);
		return 1; 
	}
	else {
		while(fgets(buf, maxChars, file)){

			// Read calendar names enclosed in [[]]
			if((token = strstr(buf, "[["))){
				if(token) { 
					token = token + 2;
					strcpy(cal[calCounter], strtok(token, "]]"));
					calCounter++;
				}
			}

			// Find default and format
			if((token = strstr(buf, defaultCalKey))) {
				int a = strlen(defaultCalKey);
				while(token[a] == ' ' || token[a] == '=') a++;  
				token = strtok(token, "\n");
				for(int b = 0; b < maxCalendars; b++) {
					if(strcmp(token + a, cal[b]) == 0) { currentCal = b; }
				}
			}
		}

		if(strlen(cal[0]) == 0) {
			printf("No calendars found in khal configuration file.\n");
			return 1;
		} else return 0;
	} 
}
