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

int debug = 0;

char evname[maxname];
char location[maxname];
char orgname[maxname];
char orgemail[maxemail];
char ownemail[maxoemail][maxemail];
char descr[bbch];
bool haveagenda = 0;
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
int curoemail = 0;
int respemail = 0;
int toff = 0;

char khal[maxpath] = "khal";
char khalconf[maxpath];
char pager[maxpath];
char tmpdir[maxpath] = "/tmp";

char *icsfile;

int writefile(char *fname, char *text) {

	FILE *file = fopen(fname, "a");

	if(file == NULL) return 1; 
	else {
		fprintf(file, "%s\n", text);
		fclose(file);
		return 0;
	}
}

char *getl(char *ret, size_t len) {

	fgets(ret, len, stdin);
	ret[(strlen(ret) -1)] = '\0';
	if(strlen(ret) > len) ret[len] = '\0';
	return ret;
}

int mansetemail() {

	char cfile[maxpath];
	char prefix[] = "email=";

	sprintf(cfile, "%s/%s", getenv("HOME"), cfilename);

	printf("\nEnter your email address: ");
	getl(ownemail[0], sizeof(ownemail[0]));

	curoemail++;

	printf("\nWrite '%s%s' to %s?\n", prefix, ownemail[0], cfile);
	printf(WHT "y:" RESET " yes " WHT "n:" RESET
			" no " WHT "r:" RESET " re-enter\n");

	char sel = getin("ynr");

	for(;;) {
		if(sel == 'y' || sel == 'Y') {
			char *wrstring = calloc(strlen(prefix) + strlen(ownemail[0]) + 1, sizeof(char));
			sprintf(wrstring, "%s%s", prefix, ownemail[0]);
			writefile(cfile, wrstring);
			free(wrstring);
			return 0;
		}
		else if(sel == 'r' || sel == 'R') mansetemail();
		else return 1;
	}
}

int processInput() {

	char cstr[maxpath];
	char opts[maxopts] = "aisq";

	if(strlen(ownemail[0]) == 0) strcat(opts, "e");
	if(debug) strcat(opts, "r");

	char sel = getin(opts);

	for(;;) {

		if(sel == 'a') {
			sprintf(cstr, "%s import -a %s --batch %s",
				khal, cal[ccal], icsfile);
			system(cstr);
			return 0;

		} else if(sel == 'i') {
			sprintf(cstr, "%s interactive", khal);
			system(cstr);
			haveagenda = 0;
			return printAll();

		} else if(sel == 'e') {
			mansetemail();
			return printAll();

		} else if(sel == 'r') {
			sprintf(cstr, "%s %s", pager, icsfile);
			system(cstr);
			return printAll();

		} else if(sel == 's') {
			printcal();
			ccal = getcalin();
			return printAll();
		}

		else { return 0; }
	}
}

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

int printatts(int respset) {

	int llcount = 0;
	char *tmp = calloc(mbch, sizeof(char));

	if(numatts > 1) {
		sprintf(tmp, WHT "Attendees:" RESET " (%d incl. organizer)\n", numatts);
		llcount += cpr(tmp);

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

			if (respset == 0 && a == respemail)  {
				sprintf(tmp, WHT "%s (%s)" RESET " - %s\n", attname[a], attemail[a], rsvpstr);
				llcount += cpr(tmp);
			}
			else {
				sprintf(tmp, "%s (%s) - %s\n", attname[a], attemail[a], rsvpstr);
				llcount += cpr(tmp);
			}
		}

		if(numatts > curatt + 1) {
			sprintf(tmp, "(%d more)\n", numatts - (curatt + 1));
			llcount += cpr(tmp);
		}

	} else {
		sprintf(tmp, RED "\nNo attendees\n" RESET);
		llcount += cpr(tmp);
	}

	return llcount;
}

int printdescr() {

	int llcount = 0;
	char *tmp = calloc(mbch, sizeof(char));

	if(strlen(descr) > 0) {
		sprintf(tmp, WHT "\nDescription:\n" RESET "%s\n", descr);
		llcount += cpr(tmp);
	}
	else llcount += cpr(RED "\nNo event description\n" RESET);

	llcount += cpr("\n--\n\n");

	return llcount;
}

int printhdr() {

	int llcount = 0;
	char *tmp = calloc(mbch, sizeof(char));

	sprintf(tmp, WHT "Event name: " RESET "\t%s\n", evname);
	llcount += cpr(tmp);

	if(strlen(location) > 0) {
		sprintf(tmp, WHT "Location: " RESET "\t%s\n", location);
		llcount += cpr(tmp);
	}

	if(strlen(orgname) > 0) {
		sprintf(tmp, WHT "Organizer:" RESET "\t%s (%s)\n", orgname, orgemail);
		llcount += cpr(tmp);
	} else {
		sprintf(tmp, WHT "Organizer:" RESET "\t%s\n", orgemail);
		llcount += cpr(tmp);
	}

	if(eyear == syear && emonth == smonth && eday == sday) {
		sprintf(tmp, WHT "Time:" RESET "\t\t%04d-%02d-%02d\t%02d:%02d - %02d:%02d\n\n",
				syear, smonth, sday, (stime / 60), (stime % 60),
				(etime / 60), (etime % 60));
		llcount += cpr(tmp);
	} else {
		sprintf(tmp, WHT "Starting at:" RESET "\t%04d-%02d-%02d\t%02d:%02d\n",
				syear, smonth, sday, (stime / 60), (stime % 60));
		llcount += cpr(tmp);
		sprintf(tmp, WHT "Ending at:" RESET "\t%04d-%02d-%02d\t%02d:%02d\n\n",
				eyear, emonth, eday, (etime / 60), (etime % 60));
		llcount += cpr(tmp);
	}

	return llcount;
}

int printMenu() {

	int llcount = 0;
	char *tmp = calloc(mbch, sizeof(char));

	llcount += cpr("\n\n--\n\n");
	llcount += cpr(WHT "a:" RESET " Add to khal\n");
	if(strlen(ownemail[0]) == 0)
		llcount += cpr(WHT "e:" RESET " Set email address\n");
	sprintf(tmp, WHT "s:" RESET " Select calendar (%s)\n", cal[ccal]);
	llcount += cpr(tmp);
	if(debug) 
		llcount += cpr(WHT "r:" RESET " View raw in pager\n");
	llcount += cpr(WHT "i:" RESET " Launch ikhal\n");
	llcount += cpr(WHT "q:" RESET " Quit\n");

	return llcount;
}

int printcal() {

	int llcount = 0;
	char *tmp = calloc(mbch, sizeof(char));

	llcount += cpr(WHT "\nSelect calendar to use:\n" RESET);

	for(int a = 0; a < maxcal; a++) {
		if(strlen(cal[a]) != 0) {
			sprintf(tmp, WHT "%d:" RESET " %s | ", a, cal[a]);
			llcount += cpr(tmp);
		}
	}
	llcount += cpr(WHT "q:" RESET " Abort");
	llcount += cpr("\n");

	return llcount;
}

int cpr(char *buf) {

	int count = cchar(buf, '\n');
	printf("%s", buf);

	return count;
}

char *readagenda(char *istr) {

	char sbuf[sbch];
	char sbuf2[sbch];
	char bbuf[bbch] = "";
	char *delim = "-";
	char *erstr = calloc(mbch, sizeof(char));

	sprintf(erstr, RED "Could not read khal agenda" RESET);

	FILE *rcmd = popen(istr, "r");

	if(rcmd == NULL) return erstr;
	else { 
		while(fgets(sbuf, sbch, rcmd)) {
			strcpy(sbuf2, sbuf);
			if(atoi(strtok(sbuf2, delim)) == syear) {
				sprintf(sbuf2, WHT "%s" RESET, sbuf);
				strcat(bbuf, sbuf2); 
			}
			else strcat(bbuf, sbuf); 
		}
	}

	haveagenda = 1; 

	return strcpy(bbuf, remtrail(bbuf));
}

int printAll() {

	char agenda[bbch];
	char cstr[maxpath];
	int lcount = 0;

	system(clear);
	lcount += printhdr();
	lcount += printatts(setrespemail());
	lcount += printdescr();

	if(!haveagenda) {
		sprintf(cstr, "%s agenda --days %d %04d-%02d-%02d",
				khal, shdays, syear, smonth, sday);
		strcpy(agenda, readagenda(cstr));
	} 
	lcount += cpr(agenda);

	lcount += printMenu();

	if(debug) {
		lcount += cpr("\n");
		lcount++;
		printf("Term size: %dx%d. This line: %d\n", termcol(), termrow(), lcount);
	}

	return processInput();
}

int init(int argc, char *execname) {

	char cstr[maxpath];

	if(argc != 2) {
		printf("Usage: %s <filename.ics>\n", execname);
		return 1;
	}

	if(readkhalconfig()) { return 1; }
	readconfig();
	if(strlen(ownemail[0]) == 0) readmuttconfig();

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
	if((ret = init(argc, argv[0])) != 0) return ret;

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
