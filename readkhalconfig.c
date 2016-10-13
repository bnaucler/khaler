/*

	readhkalconfig.c - file operations for khaler

 */

#include "khaler.h"

int readKhalConfig() {

	int counter = 0;
	bool incal = 0;

	char confpath[100];

	char buf[sbch];
	char *token;
	const char defkey[] = "default_calendar";

	sprintf(confpath, "%s/.config/khal/khal.conf", getenv("HOME"));

	FILE* file = fopen(confpath, "r");

	if(file == NULL) {
		printf("File %s could not be opened.\n", confpath);
		return 1;
	}
	else {
		while(fgets(buf, sbch, file)){

		// Determine if reading from [calendars] section
			if((token = strstr(buf, "["))){
				token++;
				if(token) token = strtok(token, "]");
				if(token) {
					if(strcmp(token, "calendars") == 0) incal = 1;
					else if(!strstr(token, "[")) {
						incal = 0;
					}
				}
			}

			// Read calendar names enclosed in [[]]
			if(((token = strstr(buf, "[[")) && incal)) {
				if(token) {
					token += 2;
					strcpy(cal[counter], strtok(token, "]]"));
					counter++;
				}
			}

			// Find default and format
			if((token = strstr(buf, defkey))) {
				int a = strlen(defkey);
				while(token[a] == ' ' || token[a] == '=') a++;
				token = strtok(token, "\n");
				for(int b = 0; b < maxcal; b++) {
					if(strcmp(token + a, cal[b]) == 0) { ccal = b; }
				}
			}
		}

		if(strlen(cal[0]) == 0) {
			printf("No calendars found in khal configuration file.\n");
			return 1;
		} else return 0;
	}
}
