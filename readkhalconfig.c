/*

	readkhalconfig.c - file operations for khaler

 */

#include "khaler.h"

int readKhalConfig() {

	int counter = 0;
	bool incal = 0;

	char buf[sbch];
	char *token = calloc(sbch, sizeof(char));
	const char calkey[] = "calendars";
	const char defkey[] = "default_calendar";

	sprintf(khalconf, "%s/.config/khal/khal.conf", getenv("HOME"));

	FILE* file = fopen(khalconf, "r");

	if(file == NULL) {
		printf("File %s could not be opened.\n", khalconf);
		return 1;
	}
	else {
		while(fgets(buf, sbch, file)){

		// Determine if reading from [calendars] section
			if((token = strstr(buf, "["))){
				token++;
				if(token) token = strtok(token, "]");
				if(token) {
					if(strcmp(token, calkey) == 0) incal = 1;
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
				int cklen = strlen(defkey);
				strcpy(token, readconfobj(cklen, token));
				for(int b = 0; b < maxcal; b++) {
					if(strcmp(token, cal[b]) == 0) { ccal = b; }
				}
			}
		}

		if(strlen(cal[0]) == 0) {
			printf("No calendars found in khal configuration file.\n");
			return 1;
		} else return 0;
	}
}
