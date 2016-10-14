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
int ccal = 0;

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
int attrsvp[maxatts];

int numatts = 0, curatt = 0;

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
	// printf(WHT "rows:" RESET "\t%d\n", termrow());
	// printf(WHT "cols:" RESET "\t%d\n", termcol());

	if(numatts > 1) {
		printf(WHT "Attendees:" RESET " (%d incl. organizer)\n", numatts);

		for(int a = 0; a < curatt; a++) {

			char rsvpstr[40];

			if(strlen(attname[a]) == 0 && strlen(attemail[a]) == 0) continue;
			else if(strlen(attname[a]) == 0) strcpy(attname[a], "Unknown name");
			else if(strlen(attemail[a]) == 0) strcpy(attemail[a], "Unknown email");

			if(attrsvp[a] == 1) sprintf(rsvpstr, YELLOW "No response" RESET);
			else if(attrsvp[a] == 2) sprintf(rsvpstr, GREEN "Accepted" RESET);
			else if(attrsvp[a] == 3) sprintf(rsvpstr, YELLOW "Tentative" RESET);
			else if(attrsvp[a] == 4) sprintf(rsvpstr, RED "Declined" RESET);
			else strcpy(rsvpstr, "Unknown");

			printf("%s (%s) - %s\n", attname[a], attemail[a], rsvpstr);
		}

		if(numatts > curatt + 1) printf("(%d more)\n", numatts - (curatt + 1));

	} else printf(RED "\nNo attendees\n" RESET);

	if(strlen(descr) > 0) printf(WHT "\nDescription:\n" RESET "%s\n", descr);
	else printf(RED "\nNo event description\n" RESET);

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
