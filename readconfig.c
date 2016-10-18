/*

	readconfig.c - file operations for khaler

 */

#include "khaler.h"

#define cmailkey		"email"
#define ckpathkey		"kpath"
#define ckcpathkey		"kconfigpath"
#define csendstrkey		"sendstring"

int readconfig() {

	char confpath[100];

	char buf[sbch];
	char *token;
	const char cfile[] = ".khaler";
	const char comstr = '#';

	sprintf(confpath, "%s/%s", getenv("HOME"), cfile);

	FILE* file = fopen(confpath, "r");

	if(file == NULL) return 1;
	else {
		while(fgets(buf, sbch, file)){

			if(buf[0] == comstr) continue;

			if((token = strstr(buf, cmailkey))) {
				int a = strlen(cmailkey);
				while(isspace(token[a]) || token[a] == '=') a++;
				token += a;
				token = strtok(token, "\n");
				token = remchar(token, '\"');
				strcpy(ownemail, token);
			}

			if((token = strstr(buf, ckpathkey))) {
				int a = strlen(ckpathkey);
				while(isspace(token[a]) || token[a] == '=') a++;
				token += a;
				token = strtok(token, "\n");
				token = remchar(token, '\"');
				strcpy(khal, token);
			}

			if((token = strstr(buf, ckcpathkey))) {
				int a = strlen(ckcpathkey);
				while(isspace(token[a]) || token[a] == '=') a++;
				token += a;
				token = strtok(token, "\n");
				token = remchar(token, '\"');
				strcpy(khalconf, token);
			}

		}
	}

	return 0;
}
