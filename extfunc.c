/*

	extfunc.c - extended functions for khaler

 */

#include "khaler.h"

// Print to tbuf
void cpr(char *buf) {

	strcat(tbuf, buf);
}

// Print multiple arguments to tbuf
int bpr(int bufsize, const char *format, ...) {

	char tmp[bufsize];
	va_list args;

	va_start(args, format);
	int ret = vsnprintf(tmp, bufsize, format, args);
	va_end(args);

	strcat(tbuf, tmp);

	return ret;
}

// Choosing email address to respond from
int setrespemail() {

	for(int a = 0; a < curatt; a++) {
		for(int b = 0; b < curoemail; b++) {
			if(strcmp(attemail[a], ownemail[b]) == 0) {
				respemail = b;
				return 0;
			}
		}
	}

	return 1;
}

// Append buf to file
int writefile(char *fname, char *text) {

	FILE *file = fopen(fname, "a");

	if(file == NULL) return 1;
	else {
		fprintf(file, "%s\n", text);
		fclose(file);
		return 0;
	}
}

// Read line without risk of buffer overflow
char *getl(char *ret, size_t len) {

	fgets(ret, len, stdin);
	ret[(strlen(ret) -1)] = '\0';
	if(strlen(ret) > len) ret[len] = '\0';
	return ret;
}

// Verify duplicates in attendee array
int dupecheck() {

	int count = 0;

	for(int a = 0; a <= numatts; a++) {
		if(strcmp(attemail[a], orgemail) == 0) {
			for(int b = a; b <= maxatts; b++){
				strcpy(attname[b], attname[(b+1)]);
				strcpy(attemail[b], attemail[(b+1)]);
			}
			numatts--;
			count++;
		}
	}

	return count;
}

// Returns a temporary filename with full path
char *gettmpfname(char *ret, int randlen, int maxlen) {

	int count = 0;
	char vars[4][15];
	int maxvars = sizeof(vars) / sizeof(vars[0]);

	strcpy(vars[0], "TMP");
	strcpy(vars[1], "TEMP");
	strcpy(vars[2], "TMPDIR");
	strcpy(vars[3], "TEMPDIR");

	do {
		if(getenv(vars[count]) != NULL) {
			snprintf(ret, maxlen, "%s", getenv(vars[count]));
			break;
		}
	} while(count++ < maxvars);

	if(strlen(ret) == 0) snprintf(ret, sizeof(deftmpdir), "%s", deftmpdir);
	if(ret[strlen(ret) - 1] != '/') ret[strlen(ret)] = '/';

	strcat(ret, randstr(randlen));
	strcat(ret, ".ics");

	return ret;
}
