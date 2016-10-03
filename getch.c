/*
 
	getch.c - enables reading of one character at the time, without echo

	Found the base for this at some random place on the internet.
	Thank you whoever wrote it.

*/

#include "khaler.h"

static struct termios old, new;

char getch() {

  char inputChar;

  tcgetattr(0, &old);
  new = old;
  new.c_lflag &= ~ICANON;
  new.c_lflag &= 0 ? ECHO : ~ECHO;
  tcsetattr(0, TCSANOW, &new);
  inputChar = getchar();
  tcsetattr(0, TCSANOW, &old);

  return inputChar;
}
