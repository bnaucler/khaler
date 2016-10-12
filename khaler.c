/*

	khaler.c - main functions for khaler

 */

#include "khaler.h"

// Global constants and variables that might need tweaking
const char khal[] = "khal";
const char clear[] = "clear";
const char which[] = "which"; // 'whereis' on some systems

// Global variable declarations
char cstr[100];

char cal[maxcal][maxcalname];
int ccal;

char evname[maxname];
char location[maxname];
char orgname[maxname];
char orgemail[maxemail];
char descr[bbch];
char stime[tlen];
char etime[tlen];
char sdate[dlen];
char edate[dlen];

char attname[maxatts][maxname];
char attemail[maxatts][maxname];
int numatts = 0;
int curatt = 0;

char *icsFile;

int processInput() {

	char sel = getInput();

	for(;;) {

		if(sel == 'a' || sel == 'A') {
			sprintf(cstr, "%s import -a %s --batch %s",
				khal, cal[ccal], icsFile);
			system(cstr);
			return 0;

		} else if(sel == 'i' || sel == 'I') {
			sprintf(cstr, "%s interactive", khal);
			system(cstr);
			return printAll();

		} else if(sel == 's' || sel == 'S') {
			printCalendars();
			ccal = getCalInput();
			return printAll();
		}

		else { return 0; }
	}
}

void printEvent() {

	printf(WHT "Event name: " RESET "\t%s\n", evname);
	if(strlen(location) > 0) printf(WHT "Location: " RESET "\t%s\n", location);
	printf(WHT "Organizer:" RESET "\t%s (%s)\n", orgname, orgemail);
	printf(WHT "Starting at:" RESET "\t%s\t%s\n", sdate, stime);
	printf(WHT "Ending at:" RESET "\t%s\t%s\n\n", edate, etime);

	if(numatts > 1) {
		printf(WHT "Attendees:" RESET " (%d incl. organizer)\n", numatts);

		for(int a = 0; a < curatt; a++) {
			if(strlen(attname[a]) == 0 && strlen(attemail[a]) == 0) continue;
			else if(strlen(attname[a]) == 0) strcpy(attname[a], "Unknown name");
			else if(strlen(attemail[a]) == 0) strcpy(attemail[a], "Unknown email");
			printf("%s (%s)\n", attname[a], attemail[a]);
		}

		if(numatts > curatt + 1) printf("(%d more)\n", numatts - (curatt + 1));

	} else printf(RED "\nNo attendees\n" RESET);

	if(strlen(descr) > 0) {
		printf(WHT "\nDescription:\n" RESET);
		printf("%s\n", descr);
	} else {
		printf(RED "\nNo event description\n" RESET);
		}

	printf("\n--\n\n");
}

void printMenu() {

	printf("\n--\n\n");
	printf(WHT "a:" RESET " Add to khal\n");
	printf(WHT "s:" RESET " Select calendar (%s)\n", cal[ccal]);
	printf(WHT "i:" RESET " Launch ikhal\n");
	printf(WHT "q:" RESET " Quit\n");
}

int printCalendars() {

	printf(WHT "\nSelect calendar to use:\n" RESET);

	for(int a = 0; a < maxcal; a++) {
		if(strlen(cal[a]) != 0) {
			printf(WHT "%d:" RESET " %s | ", a, cal[a]);
		}
	}
	printf(WHT "q:" RESET " Abort");
	printf("\n");

	return 0;
}

int printAll() {

	system(clear);
	printEvent();

	sprintf(cstr, "%s agenda --days %d %s", khal, shdays, sdate);
	system(cstr);

	printMenu();

	return processInput();
}

int init(int argc) {

	// In case absolute path has been provided
	if(strcmp(khal, "khal") == 0) sprintf(cstr, "%s khal > /dev/null", which); // faster
	else sprintf(cstr, "%s --version > /dev/null", khal); // always accurate

	if(system(cstr) != 0) {
		printf("khal not found! Make sure it is installed and in your path.\n");
		return 1;
	}

	if(argc != 2) {
		printf("Usage: khaler <filename.ics>\n");
		return 1;
	}

	if(readKhalConfig()) { return 1; }

	return 0;
}

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

void parseBuf(char *bbuf) {

	const char delim[] = ":;=\r\n";

	char namekey[] = "SUMMARY";
	char lockey[] = "LOCATION";
	char attkey[] = "ATTENDEE";
	char orgkey[] = "ORGANIZER";
	char emailkey[] = "mailto";
	char startkey[] = "DTSTART";
	char endkey[] = "DTEND";
	char descrkey[] = "DESCRIPTION";

	char *token;

	char bbuf2[bbch];
	strcpy(bbuf2, bbuf); // Ugly hack perhaps. There should be a better way.

	if((token = strcasestr(bbuf, namekey))) {
		token = strstr(token, ":");
		token++;
		strcpy(evname, token);
	}

	if((token = strcasestr(bbuf, lockey))) {
		token = strstr(token, ":");
		token++;
		if(token) strcpy(location, token);
	}

	if((token = strcasestr(bbuf, orgkey))) {
		if(strlen(orgname) == 0 || strlen(orgemail) == 0) {
			token = strstr(token, "CN=");
			token = strtok(token, delim);
			if(token) {
				numatts++;
				strcpy(orgname, strtok(NULL, delim));
				strcpy(orgname, remchar(orgname, '\"'));
			}
			token = strcasestr(bbuf2, emailkey);
			token = strtok(token, delim);
			if(token) strcpy(orgemail, strtok(NULL, delim));
		}
	}

	if((token = strcasestr(bbuf, attkey))) {
		if(!strstr(bbuf2, "X-LOTUS") && strstr(bbuf2, ":")) {
			numatts++;
			if(curatt < numatts) {
				token = strstr(token, "CN=");
				token = strtok(token, delim);
				if(token) {
					strcpy(attname[curatt], strtok(NULL, delim));
					strcpy(attname[curatt], remchar(attname[curatt], '\"'));
				}
				token = strcasestr(bbuf2, emailkey);
				token = strtok(token, delim);
				if(token) strcpy(attemail[curatt], strtok(NULL, delim));
				curatt++;
			}
		}
	}

	if((token = strcasestr(bbuf, descrkey))) {
		token = strstr(token, ":");
		token++;
		if(strcasecmp(token, "REMINDER") != 0) {
			strcpy(descr, repstr(token, "\\n", "\n"));
			strcpy(descr, remchar(descr, '\\'));
		}
	}

	if((token = strcasestr(bbuf, startkey))) {
		token = strstr(token, ":");
		token++;
		if(token) {
			strcpy(sdate, formatDate(strtok(token, "T")));
			strcpy(stime, formatTime(strtok(NULL,delim)));
		}
	}

	if((token = strcasestr(bbuf, endkey))) {
		token = strstr(token, ":");
		token++;
		if(token) {
			strcpy(edate, formatDate(strtok(token, "T")));
			strcpy(etime, formatTime(strtok(NULL,delim)));
		}
	}
}

int main(int argc, char *argv[]) {

	int ret;
	if((ret = init(argc)) != 0) return ret;

	char sbuf[sbch];
	char bbuf[bbch];

	icsFile = argv[1];
	FILE* file = fopen(icsFile, "r");

	if(file == NULL) {
		printf("File %s could not be opened.\n", icsFile);
		return 1; }
	else {
		while(fgets(sbuf, sbch, file)){
			strtok(sbuf, "\n");

			// initial whitespace indicates broken line
			if(isspace(sbuf[0])) {
				memmove(sbuf, sbuf+1, strlen(sbuf));
				strcat(bbuf, sbuf);
			} else {
				parseBuf(bbuf);
				strcpy(bbuf, sbuf);
			}
		}
		dupecheck();
	}

	return printAll();
}
