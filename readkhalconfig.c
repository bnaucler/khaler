#include "khaler.h"

char returnString[100];

char *readKhalConfig() {
		
	int maxChars = 1024;

	char buf[maxChars];
	char *token;
	char *khalConfig = malloc(100);

	sprintf(khalConfig, "%s/.config/khal/khal.conf", getenv("HOME"));

	FILE* file = fopen(khalConfig, "r");

	if(file == NULL) { 
		printf("File %s could not be opened.\n", khalConfig);
		return NULL; }
	else {
		while(fgets(buf, maxChars, file)){
			token = strstr(buf, "[[");
			if(token) { 
				token = token + 2;
				token = strtok(token, "]]");
				strcat(returnString, token);
				strcat(returnString, ":");
			}
		}

		// Stripping the trailing delimiter
		char *lastPointer = returnString;
		lastPointer[strlen(lastPointer) - 1] = 0;

		return returnString;
	} 

}
