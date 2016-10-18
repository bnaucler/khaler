/*

	getinput.c - input reading functions for khaler

 */

#include "khaler.h"

// Read one char without echo via getch
char getInput() {

	char ch;

	for(;;) {
		ch = getch();
		if(ch == 'q' || ch == 'Q' || ch == 'a' ||
			ch == 'A' || ch == 'i' || ch == 'I' ||
			ch == 's' || ch == 'S') {
			return ch;
		}
	}
}

// Read one char without echo via getch and convert to int
char getCalInput() {

	char ch;

	for(;;) {
		ch = getch();
		if(isdigit(ch) != 0) {
			int sel = ch - '0'; // Hack, but C std
			if(strlen(cal[sel]) != 0) return sel;
		} else if (ch == 'q' || ch == 'Q') return ccal;
	}
}
