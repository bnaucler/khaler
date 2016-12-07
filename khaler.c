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

char tbuf[tbch];
char evname[maxname];
char location[maxname];
char orgname[maxname];
char orgemail[maxemail];
char ownemail[maxoemail][maxemail];
char descr[bbch];
char agenda[bbch];
bool needagenda = 1;
int sttime = 0;
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

typedef enum {ACCEPTED, DECLINED, TENTATIVE} rsvp;

char khalconf[maxpath];
char khal[maxpath] = "khal";
char pager[maxpath] = "less";
char editor[maxpath] = "vi";
char tmpdir[maxpath] = "/tmp";
char sendstr[sbch] = "/usr/local/bin/mutt -s RSVP $recepient -a $file";

char icsfile[maxpath];

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
		if(sel == 'y') {
			char *wrstring = calloc(strlen(prefix) + strlen(ownemail[0]) + 1, sizeof(char));
			sprintf(wrstring, "%s%s", prefix, ownemail[0]);
			writefile(cfile, wrstring);
			free(wrstring);
			return 0;
		}
		else if(sel == 'r') mansetemail();
		else return 1;
	}
}

char *mkrsvpstr(char *input, int status) {

	if(status == ACCEPTED) strcpy(input, repstr(input, statusn, statusa));
	if(status == DECLINED) strcpy(input, repstr(input, statusn, statusd));
	if(status == TENTATIVE) strcpy(input, repstr(input, statusn, statust));

	return input;
}

char *mksendstr(char *ret, char *fname) {

	strcpy(ret, sendstr);
	strcpy(ret, repstr(ret, "$recipient", orgemail));
	strcpy(ret, repstr(ret, "$file", fname));

	return ret;
}

int mkrsvp(int status) {

	char *dfname = calloc(maxpath, sizeof(char));
	char sbuf[sbch];
	char *sbuf2 = calloc(sbch, sizeof(char));
	char *fstr = calloc(maxpath, sizeof(char));

	int olen = 0, oolen = 0, remz = 0;

	strcpy(dfname, gettmpfname(sbuf2, rflen, maxpath));

	FILE* sfile = fopen(icsfile, "r");
	FILE* dfile = fopen(dfname, "a");

	if(sfile == NULL || dfile == NULL) return 1;
	else {
		while(fgets(sbuf, sbch, sfile)){

			if(strcasestr(sbuf, "METHOD:REQUEST")) strcpy(sbuf, "METHOD:REPLY\n");
			else if(strcasestr(sbuf, "BEGIN:VTIMEZONE")) remz = 1;
			else if(strcasestr(sbuf, "END:VTIMEZONE")) {
				remz = 0;
				continue;
			}

			if (!remz) {
				if (!isspace(sbuf[0]) && olen) {

					if(strcasestr(sbuf2, ownemail[respemail])) {
						strcpy(sbuf2, remchar(sbuf2, '\n'));
						strcpy(fstr, mkrsvpstr(sbuf2, status));
						if(strlen(fstr) > (icsmaxlen + 1)) {
							memmove(fstr + (oolen + 2), fstr + oolen, strlen(fstr) - oolen);
							fstr[oolen] = '\n';
							fstr[oolen+1] = ' ';
						}
						strcat(fstr, "\n");
						fprintf(dfile, "%s", fstr);
					} else {
						// if(strlen(sbuf2) > (icsmaxlen + 1)) {
						// 	memmove(sbuf2 + (olen + 1), sbuf2 + olen, strlen(sbuf2) - olen);
						// 	sbuf2[olen] = ' ';
						// }
						// fprintf(dfile, "%s", sbuf2);
					}
					strcpy(sbuf2, sbuf);
					oolen = olen;
					olen = 0;
				}

				if(strcasestr(sbuf, attkey)) {
					strcpy(sbuf2, sbuf);
					olen = strlen(sbuf);
					if(!oolen) oolen = olen;
				} else if(isspace(sbuf[0]) && olen) {
					memmove(sbuf, sbuf+1, strlen(sbuf));
					strcat(sbuf2, sbuf);
				}

				if (!olen) fprintf(dfile, "%s", sbuf);
			}
		}
	}

	fclose(dfile);
	free(dfname);
	mksendstr(sbuf2, dfname);
	if(debug) {
		printf("DEBUG temp file: %s\n", dfname);
		printf("DEBUG sendstr: %s\n", sbuf2);
		printf("DEBUG own email: %s\n", ownemail[respemail]);
		printf("DEBUG # atts: %d\n", numatts);
	}
	// system(sbuf2);
	free(fstr);
	free(sbuf2);
	// remove(dfname); // include error handling
	// sleep(500);
	return 0;
}

int procinput() {

	char cstr[maxpath];
	char opts[maxopts] = "adtisq";

	if(strlen(ownemail[0]) == 0) strcat(opts, "e");
	if(debug) strcat(opts, "v");

	char sel = getin(opts);

	for(;;) {

		if(sel == 'a') {
			mkrsvp(ACCEPTED);
			sprintf(cstr, "%s import -a %s --batch %s",
				khal, cal[ccal], icsfile);
			system(cstr);
			return 0;

		} else if(sel == 'd') {
			mkrsvp(DECLINED);
			return 0;

		} else if(sel == 't') {
			mkrsvp(TENTATIVE);
			sprintf(cstr, "%s import -a %s --batch %s",
				khal, cal[ccal], icsfile);
			system(cstr);
			return 0;

		} else if(sel == 'i') {
			sprintf(cstr, "%s interactive", khal);
			system(cstr);
			needagenda = 1;
			return printall();

		} else if(sel == 'e') {
			mansetemail();
			return printall();

		} else if(sel == 'v') {
			sprintf(cstr, "%s %s", pager, icsfile);
			system(cstr);
			return printall();

		} else if(sel == 's') {
			printcal();
			ccal = getcalin();
			return printall();
		}

		else { return 0; }
	}
}

void printatts(int respset) {

	if(numatts > 1) {
		bpr(mbch, WHT "Attendees:" RESET " (%d incl. organizer)\n", numatts);

		for(int a = 0; a < curatt; a++) {

			char rsvpstr[40];

			if(strlen(attname[a]) == 0 && strlen(attemail[a]) == 0) continue;
			else if(strlen(attname[a]) == 0) {
				if(strcmp(attemail[a], ownemail[respemail]) == 0)
					strcpy(attname[a], "You");
				else strcpy(attname[a], "Unknown name");
			}
			else if(strlen(attemail[a]) == 0) strcpy(attemail[a], "Unknown email");

			if(attrsvp[a] == 1) sprintf(rsvpstr, YELLOW "No response" RESET);
			else if(attrsvp[a] == 2) sprintf(rsvpstr, GREEN "Accepted" RESET);
			else if(attrsvp[a] == 3) sprintf(rsvpstr, YELLOW "Tentative" RESET);
			else if(attrsvp[a] == 4) sprintf(rsvpstr, RED "Declined" RESET);
			else strcpy(rsvpstr, "Unknown");

			if (respset == 0 && strcmp(attemail[a], ownemail[respemail]) == 0)
				bpr(mbch, WHT "%s (%s)" RESET " - %s\n", attname[a], attemail[a], rsvpstr);
			else bpr(mbch, "%s (%s) - %s\n", attname[a], attemail[a], rsvpstr);
		}

		if(numatts > curatt + 1) bpr(mbch, "(%d more)\n", numatts - (curatt + 1));

	} else bpr(mbch, RED "\nNo attendees\n" RESET);
}

void printdescr() {

	if(strlen(descr) > 0) bpr(bbch, WHT "\nDescription:\n" RESET "%s\n", descr);
	else bpr(mbch, RED "\nNo event description\n" RESET);

	cpr("\n--\n\n");
}

void printhdr() {

	bpr(mbch, WHT "Event name: " RESET "\t%s\n", evname);

	if(strlen(location) > 0) bpr(mbch, WHT "Location: " RESET "\t%s\n", location);

	if(strlen(orgname) > 0)
		bpr(mbch, WHT "Organizer:" RESET "\t%s (%s)\n", orgname, orgemail);
	else bpr(mbch, WHT "Organizer:" RESET "\t%s\n", orgemail);

	if(eyear == syear && emonth == smonth && eday == sday)
		bpr(mbch, WHT "Time:" RESET "\t\t%04d-%02d-%02d\t%02d:%02d - %02d:%02d\n\n",
				syear, smonth, sday, (sttime / 60), (sttime % 60),
				(etime / 60), (etime % 60));
	else {
		bpr(mbch, WHT "Starting at:" RESET "\t%04d-%02d-%02d\t%02d:%02d\n",
				syear, smonth, sday, (sttime / 60), (sttime % 60));
		bpr(mbch, WHT "Ending at:" RESET "\t%04d-%02d-%02d\t%02d:%02d\n\n",
				eyear, emonth, eday, (etime / 60), (etime % 60));
	}
}

void printMenu() {

	cpr("\n--\n\n");
	cpr(WHT "a:" RESET " Accept\n");
	cpr(WHT "d:" RESET " Decline\n");
	cpr(WHT "t:" RESET " Tentative\n");
	if(strlen(ownemail[0]) == 0) cpr(WHT "e:" RESET " Set email address\n");
	bpr(mbch, WHT "s:" RESET " Select calendar (%s)\n", cal[ccal]);
	if(debug) cpr(WHT "v:" RESET " View raw in pager\n");

	cpr(WHT "i:" RESET " Launch ikhal\n");
	cpr(WHT "q:" RESET " Quit\n");
}

void printcal() {

	memset(tbuf, 0, tbch);

	bpr(mbch, WHT "\nSelect calendar to use:\n" RESET);

	for(int a = 0; a < maxcal; a++) {
		if(strlen(cal[a]) != 0) bpr(mbch, WHT "%d:" RESET " %s | ", a, cal[a]);
	}
	cpr(WHT "q:" RESET " Abort\n");

	printf("%s", tbuf);
}

int getagenda() {

	char cstr[maxpath];
	char sbuf[sbch];
	char sbuf2[sbch];
	char erstr[mbch];
	char *ddelim = "-";

	memset(agenda, 0, bbch);

	sprintf(cstr, "%s agenda --days %d %04d-%02d-%02d",
			khal, shdays, syear, smonth, sday);

	sprintf(erstr, RED "Could not read khal agenda" RESET);

	FILE *rcmd = popen(cstr, "r");

	if(rcmd == NULL) {
		strcpy(agenda, erstr);
		return 1;
	}
	else {
		while(fgets(sbuf, sbch, rcmd)) {
			strcpy(sbuf2, sbuf);
			if(atoi(strtok(sbuf2, ddelim)) == syear) {
				sprintf(sbuf2, WHT "%s" RESET, sbuf);
				strcat(agenda, sbuf2);
			}
			else strcat(agenda, sbuf);
		}
	}

	pclose(rcmd);
	remtrail(agenda);

	return 0;
}

int printall() {

	memset(tbuf, 0, tbch);

	if(!debug) system(clear);
	printhdr();
	printatts(setrespemail());
	printdescr();

	if(needagenda) needagenda = getagenda();
	cpr(agenda);
	printMenu();

	int lcount = cchar(tbuf, '\n');
	printf("%s", tbuf);

	if(debug) {
		cpr("\n");
		printf("Term size: %dx%d. This line: %d\n", termcol(), termrow(), ++lcount);
	}

	return procinput();
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

int main(int argc, char *argv[]) {

	int ret;
	if((ret = init(argc, argv[0])) != 0) return ret;

	char sbuf[sbch];
	char bbuf[bbch];

	strcpy(icsfile, argv[1]);
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

		sttime += (toff/60);
		etime += (toff/60);

		dupecheck();
	}

	fclose(file);
	return printall();
}
