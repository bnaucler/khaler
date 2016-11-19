/*

	readconfig.c - config file operations for khaler

 */

#include "khaler.h"

// khaler config
#define cmailkey		"email"
#define pagerkey		"pager"
#define editorkey		"editor"
#define debugkey		"debug"
#define ckpathkey		"kpath"
#define ckcpathkey		"kconfigpath"
#define tmpdirkey		"tempdir"
#define sendstrkey		"sendstring"

// khal config
#define calkey			"calendars"
#define defkey			"default_calendar"

// mutt config
#define omkey			"set from"

char *readconfobj(char *token, char *key) {

	int gklen = strlen(key);

	while(isspace(token[gklen]) || token[gklen] == '=') gklen++;
	token += gklen;
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
				strcpy(ownemail[curoemail], readconfobj(token, cmailkey));
				curoemail++;
			}

			if((token = strstr(buf, ckpathkey)))
				strcpy(khal, readconfobj(token, ckpathkey));

			if((token = strstr(buf, pagerkey)))
				strcpy(pager, readconfobj(token, pagerkey));

			if((token = strstr(buf, editorkey)))
				strcpy(editor, readconfobj(token, editorkey));

			if((token = strstr(buf, tmpdirkey)))
				strcpy(tmpdir, readconfobj(token, tmpdirkey));

			if((token = strstr(buf, debugkey)))
				debug = atoi(readconfobj(token, debugkey));

			if((token = strstr(buf, ckcpathkey)))
				strcpy(khalconf, readconfobj(token, ckcpathkey));

			if((token = strstr(buf, sendstrkey)))
				strcpy(sendstr, readconfobj(token, sendstrkey));
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
				strcpy(token, readconfobj(token, defkey));
				for(int b = 0; b < maxcal; b++) {
					if(strcmp(token, cal[b]) == 0) { ccal = b; }
				}
			}
		}

		if(strlen(cal[0]) == 0) {
			printf("No calendars found in khal configuration file.\n");
			free(token);
			fclose(file);
			return 1;
		} else {
			free(token);
			fclose(file);
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
				strcpy(ownemail[0], readconfobj(token, omkey));
				curoemail++;
				fclose(file);
				return 0;
			}
		}
	}

	fclose(file);
	free(token);
	return 1;
}
