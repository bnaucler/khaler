/*

	khaler.c - main functions for khaler

 */

#include "khaler.h"

// Global constants that might need tweaking
const char clear[] = "clear";
const char which[] = "which"; // 'whereis' on some systems

// Global variable declarations
char cal[maxcal][maxcalname];
int ccal = 0;

char evname[maxname];
char location[maxname];
char orgname[maxname];
char orgemail[maxemail];
char ownemail[maxemail];
char descr[bbch];
int stime = 0;
int etime = 0;
int syear = 0;
int smonth = 0;
int sday = 0;
int eyear = 0;
int emonth = 0;
int eday = 0;

char attname[maxatts][maxname];
char attemail[maxatts][maxname];
int attrsvp[maxatts];

int numatts = 0, curatt = 0;
int toff = 0;

char khal[maxpath] = "khal";
char khalconf[maxpath];

char *icsfile;

int processInput() {

	char sel = getInput();
	char cstr[maxpath];

	for(;;) {

		if(sel == 'a' || sel == 'A') {
			sprintf(cstr, "%s import -a %s --batch %s",
				khal, cal[ccal], icsfile);
			system(cstr);
			return 0;

		} else if(sel == 'i' || sel == 'I') {
			sprintf(cstr, "%s interactive", khal);
			system(cstr);
			return printAll();

		} else if(sel == 's' || sel == 'S') {
			printcal();
			ccal = getCalInput();
			return printAll();
		}

		else { return 0; }
	}
}

void printatts() {

	if(numatts > 1) {
		printf(WHT "Attendees:" RESET " (%d incl. organizer)\n", numatts);

		for(int a = 0; a < curatt; a++) {

			char rsvpstr[40];
			bool self = 0;

			if(strlen(attname[a]) == 0 && strlen(attemail[a]) == 0) continue;
			else if(strlen(attname[a]) == 0) strcpy(attname[a], "Unknown name");
			else if(strlen(attemail[a]) == 0) strcpy(attemail[a], "Unknown email");

			if(strcmp(attemail[a], ownemail) == 0) self = 1;

			if(attrsvp[a] == 1) sprintf(rsvpstr, YELLOW "No response" RESET);
			else if(attrsvp[a] == 2) sprintf(rsvpstr, GREEN "Accepted" RESET);
			else if(attrsvp[a] == 3) sprintf(rsvpstr, YELLOW "Tentative" RESET);
			else if(attrsvp[a] == 4) sprintf(rsvpstr, RED "Declined" RESET);
			else strcpy(rsvpstr, "Unknown");

			if (self) printf(WHT "%s (%s)" RESET " - %s\n", attname[a], attemail[a], rsvpstr);
			else printf("%s (%s) - %s\n", attname[a], attemail[a], rsvpstr);
		}

		if(numatts > curatt + 1) printf("(%d more)\n", numatts - (curatt + 1));

	} else printf(RED "\nNo attendees\n" RESET);
}

void printdescr() {

	if(strlen(descr) > 0) printf(WHT "\nDescription:\n" RESET "%s\n", descr);
	else printf(RED "\nNo event description\n" RESET);

	printf("\n--\n\n");
}

void printhdr() {

	bool sameday = 0;

	if(eyear == syear && emonth == smonth && eday == sday) sameday = 1;

	printf(WHT "Event name: " RESET "\t%s\n", evname);

	if(strlen(location) > 0) printf(WHT "Location: " RESET "\t%s\n", location);
	printf(WHT "Organizer:" RESET "\t%s (%s)\n", orgname, orgemail);

	if(sameday) {
		printf(WHT "Time:" RESET "\t\t%04d-%02d-%02d\t%02d:%02d - %02d:%02d\n\n",
				syear, smonth, sday, (stime / 60), (stime % 60),
				(etime / 60), (etime % 60));
	} else {
		printf(WHT "Starting at:" RESET "\t%04d-%02d-%02d\t%02d:%02d\n",
				syear, smonth, sday, (stime / 60), (stime % 60));
		printf(WHT "Ending at:" RESET "\t%04d-%02d-%02d\t%02d:%02d\n\n",
				eyear, emonth, eday, (etime / 60), (etime % 60));
	}

}

void printMenu() {

	printf("\n--\n\n");
	printf(WHT "a:" RESET " Add to khal\n");
	printf(WHT "s:" RESET " Select calendar (%s)\n", cal[ccal]);
	printf(WHT "i:" RESET " Launch ikhal\n");
	printf(WHT "q:" RESET " Quit\n");
}

int printcal() {

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

	char cstr[maxpath];

	system(clear);
	printhdr();
	printatts();
	printdescr();

	sprintf(cstr, "%s agenda --days %d %04d-%02d-%02d",
			khal, shdays, syear, smonth, sday);
	system(cstr);

	printMenu();

	return processInput();
}

int init(int argc) {

	char cstr[maxpath];

	if(argc != 2) {
		printf("Usage: khaler <filename.ics>\n");
		return 1;
	}

	if(readKhalConfig()) { return 1; }
	readconfig();
	if(strlen(ownemail) == 0) readmuttconfig();

	// In case absolute path has been provided in config file
	if(strcmp(khal, "khal") == 0) sprintf(cstr, "%s khal > /dev/null", which);
	else sprintf(cstr, "test -x %s", khal);

	if(system(cstr) != 0) {
		printf("khal not found! Make sure it is installed and in your path.\n");
		return 1;
	}
	toff = toffset();
	settzkeys(isdls());

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

	icsfile = argv[1];
	FILE* file = fopen(icsfile, "r");

	if(file == NULL) {
		printf("File %s could not be opened.\n", icsfile);
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

		stime += (toff/60);
		etime += (toff/60);

		dupecheck();
	}

	return printAll();
}
