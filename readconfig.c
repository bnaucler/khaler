/*

	readconfig.c - file operations for khaler

 */

#include "khaler.h"

#define cmailkey		"email"
#define ckpathkey		"kpath"
#define ckcpathkey		"kconfigpath"
#define csendstrkey		"sendstring"

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
