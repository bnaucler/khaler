/*

	parsebuf.c - buffer parsing functions for khaler

 */

#include "khaler.h"

#define emailkey		"mailto"
#define namekey			"SUMMARY"
#define lockey			"LOCATION"
#define attkey			"ATTENDEE"
#define orgkey			"ORGANIZER"
#define startkey		"DTSTART"
#define endkey			"DTEND"
#define zonekey			"TZOFFSETTO"
#define descrkey		"DESCRIPTION"
#define rsvpkey			"PARTSTAT"
#define evin			"BEGIN:VEVENT"
#define evout			"END:VEVENT"

#define delim			":;=\r\n"

char tzin[klen];
char tzout[klen];
bool intz = 0;
bool inev = 0;

void settoff(char *token) {

	token += (strlen(zonekey) + 2);
	toff -= ((atoi(token) % 100) + ((atoi(token) / 100) * 60)) * 60;
}

void setname(char *token) {

	token = strstr(token, ":");
	strcpy(evname, ++token);
}

void setloc(char *token) {

	token = strstr(token, ":");
	if(token) strcpy(location, ++token);
}

void setorg(char *token, char *bbuf2) {

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

void setatt(char *token, char *bbuf2) {

	if(!strstr(bbuf2, "X-LOTUS") && strstr(bbuf2, ":") && !strstr(bbuf2, "CHAIR")) {
		numatts++;
		if(curatt < numatts && numatts < maxatts) {
			token = strstr(token, "CN=");
			token = strtok(token, delim);

			if(token) {
				strcpy(attname[curatt], strtok(NULL, delim));
				strcpy(attname[curatt], remchar(attname[curatt], '\"'));
			}

			token = strcasestr(bbuf2, emailkey);
			token = strtok(token, delim);
			if(token) strcpy(attemail[curatt], strtok(NULL, delim));

			token = strcasestr(bbuf2, rsvpkey);
			token += (strlen(rsvpkey) + 1);
			if(token) token = strtok(token, delim);
			if(token) {
				if(strcmp(token, "NEEDS-ACTION") == 0) attrsvp[curatt] = 1;
				else if(strcmp(token, "ACCEPTED") == 0) attrsvp[curatt] = 2;
				else if(strcmp(token, "TENTATIVE") == 0) attrsvp[curatt] = 3;
				else if(strcmp(token, "DECLINED") == 0) attrsvp[curatt] = 4;
				else attrsvp[curatt] = 0;
			}
			curatt++;
		}
	}
}

void setdescr(char *token) {

	token = strstr(token, ":");
	if(strcasecmp(token, ":REMINDER") != 0) {
		strcpy(descr, repstr(++token, "\\n", "\n"));
		strcpy(descr, remchar(descr, '\\'));
		strcpy(descr, breakline(descr, termcol()));
		strcpy(descr, remtrail(descr));
	}
}

void setstart(char *token) {

	token = strstr(token, ":");
	if(token) {
		consdate(strtok(++token, "T"));
		stime = contime(strtok(NULL,delim));
	}
}

void setend(char *token) {

	token = strstr(token, ":");
	if(token) {
		conedate(strtok(++token, "T"));
		etime = contime(strtok(NULL,delim));
	}
}

void parseBuf(char *bbuf) {

	char *token = calloc(bbch, sizeof(char));

	if(strcasestr(bbuf, tzin)) intz = 1;
	if(strcasestr(bbuf, tzout)) intz = 0;

	if(strcasestr(bbuf, evin)) inev = 1;
	if(strcasestr(bbuf, evout)) inev = 0;

	if((token = strcasestr(bbuf, zonekey)) && intz) settoff(token);

	if(inev) {
		char *bbuf2 = calloc(bbch, sizeof(char));
		strcpy(bbuf2, bbuf);

		if((token = strcasestr(bbuf, namekey))) setname(token);
		else if((token = strcasestr(bbuf, lockey))) setloc(token);
		else if((token = strcasestr(bbuf, orgkey))) setorg(token, bbuf2);
		else if((token = strcasestr(bbuf, attkey))) setatt(token, bbuf2);
		else if((token = strcasestr(bbuf, descrkey))) setdescr(token);
		else if((token = strcasestr(bbuf, startkey))) setstart(token);
		else if((token = strcasestr(bbuf, endkey))) setend(token);

		free(bbuf2);
	}
}
