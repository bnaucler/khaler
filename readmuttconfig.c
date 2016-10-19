/*

	readmuttconfig.c - file operations for khaler

 */

#include "khaler.h"

#define omkey "set from"

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
