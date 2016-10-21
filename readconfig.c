/*

	readconfig.c - config file operations for khaler

 */

#include "khaler.h"

// khaler config
#define cmailkey		"email"
#define pagerkey		"pager"
#define debugkey		"debug"
#define ckpathkey		"kpath"
#define ckcpathkey		"kconfigpath"
#define tmpdirkey		"tempdir"
#define csendstrkey		"sendstring"

// khal config
#define calkey			"calendars"
#define defkey			"default_calendar"

// mutt config
#define omkey			"set from"

char *readconfobj(int cklen, char *token) {

	while(isspace(token[cklen]) || token[cklen] == '=') cklen++;
	token += cklen;
	token = strtok(token, "\n");
	token = remchar(token, '\"');

	return token;
}

int readconfig() {

	char buf[sbch];
	char *token = calloc(sbch, sizeof(char));
	char cfile[maxpath];
	const char comstr = '#';

	sprintf(cfile, "%s/%s", getenv("HOME"), cfilename);

	FILE* file = fopen(cfile, "r");

	if(file == NULL) return 1;
	else {
		while(fgets(buf, sbch, file)){

			if(buf[0] == comstr) continue;

			if((token = strstr(buf, cmailkey))) {
				int cklen = strlen(cmailkey);
				strcpy(ownemail[curoemail], readconfobj(cklen, token));
				curoemail++;
			}

			if((token = strstr(buf, ckpathkey))) {
				int cklen = strlen(ckpathkey);
				strcpy(khal, readconfobj(cklen, token));
			}

			if((token = strstr(buf, pagerkey))) {
				int cklen = strlen(pagerkey);
				strcpy(pager, readconfobj(cklen, token));
			}

			if((token = strstr(buf, tmpdirkey))) {
				int cklen = strlen(tmpdirkey);
				strcpy(tmpdir, readconfobj(cklen, token));
			}

			if((token = strstr(buf, debugkey))) {
				int cklen = strlen(debugkey);
				debug =  atoi(readconfobj(cklen, token));
			}

			if((token = strstr(buf, ckcpathkey))) {
				int cklen = strlen(ckcpathkey);
				strcpy(khalconf, readconfobj(cklen, token));
			}
		}
	}

	fclose(file);
	free(token);
	return 0;
}

int readkhalconfig() {

	int counter = 0;
	bool incal = 0;

	char buf[sbch];
	char *token = calloc(sbch, sizeof(char));

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
			free(token);
			return 1;
		} else {
			free(token);
			return 0;
		}
	}
}

int readmuttconfig() {

	char confpath[100];

	char buf[sbch];
	char *token = calloc(sbch, sizeof(char));

	sprintf(confpath, "%s/.muttrc", getenv("HOME"));

	FILE* file = fopen(confpath, "r");

	if(file == NULL) return 1;
	else {
		while(fgets(buf, sbch, file)){

			if((token = strstr(buf, omkey))) {
				int cklen = strlen(omkey);
				strcpy(ownemail[0], readconfobj(cklen, token));
				curoemail++;
				return 0;
			}
		}
	}

	free(token);
	return 1;
}
