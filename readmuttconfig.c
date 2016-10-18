/*

	readmuttconfig.c - file operations for khaler

 */

#include "khaler.h"

int readmuttconfig() {

	char confpath[100];

	char buf[sbch];
	char *token;
	const char omkey[] = "set from";

	sprintf(confpath, "%s/.muttrc", getenv("HOME"));

	FILE* file = fopen(confpath, "r");

	if(file == NULL) return 1;
	else {
		while(fgets(buf, sbch, file)){

			if((token = strstr(buf, omkey))) {
				int a = strlen(omkey);
				while(isspace(token[a]) || token[a] == '=') a++;
				token += a;
				token = strtok(token, "\n");
				token = remchar(token, '\"');
				strcpy(ownemail, token);
				return 0;
			}
		}
	}

	return 1;
}
