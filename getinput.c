/*

	getinput.c - input reading functions for khaler

 */

#include "khaler.h"

// Not sure who wrote this. Thanks whoever you are
char getch() {

	static struct termios old, new;
	char ichar;

	tcgetattr(0, &old);
	new = old;
	new.c_lflag &= ~ICANON;
	new.c_lflag &= 0 ? ECHO : ~ECHO;
	tcsetattr(0, TCSANOW, &new);
	ichar = getchar();
	tcsetattr(0, TCSANOW, &old);

	return ichar;
}

// Read one char without echo via getch
char getin(char *valid) {

	char ch;
	int len = strlen(valid);

	for(;;) {
		ch = tolower(getch());
		for(int a = 0; a < len; a++) { if(ch == valid[a]) return ch; }
	}
}

// Read one char without echo via getch and convert to int
char getcalin() {

	char ch;

	for(;;) {
		ch = getch();
		if(isdigit(ch) != 0) {
			int sel = ch - '0'; // Hack, but C std
			if(strlen(cal[sel]) != 0) return sel;
		} else if (ch == 'q' || ch == 'Q') return ccal;
	}
}
