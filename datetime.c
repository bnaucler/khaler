/*

	datetime.c - parsing functions for khaler

*/

#include "khaler.h"

int toffset() {

	time_t t = time(NULL);
	struct tm lt = {0};

	localtime_r(&t, &lt);

	return lt.tm_gmtoff;
}

int isdls() {

	time_t t = time(NULL);
	struct tm lt = {0};

	localtime_r(&t, &lt);

	return lt.tm_isdst;
}

void settzkeys(int dls) {

	if(dls > 0) {
		strcpy(tzin, "BEGIN:DAYLIGHT");
		strcpy(tzout, "END:DAYLIGHT");
	}
	else if(dls == 0) {
		strcpy(tzin, "BEGIN:STANDARD");
		strcpy(tzout, "END:STANDARD");
	}
}

int contime(char *uftime) {

	return ((atoi(uftime) / 100) % 100) + ((atoi(uftime) / 10000) * 60);
}

void consdate(char *ufdate) {

	int raw = atoi(ufdate);

	syear = raw / 10000;
	smonth = (raw % 10000) / 100;
	sday = raw % 100;
}

void conedate(char *ufdate) {

	int raw = atoi(ufdate);

	eyear = raw / 10000;
	emonth = (raw % 10000) / 100;
	eday = raw % 100;
}
