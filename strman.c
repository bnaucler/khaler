#include "khaler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Count occurences of ch in buf and returns result
int cchar(char *buf, char ch) {

	int count = 0;
	int len = strlen(buf);

	for(int a = 0; a < len; a++) { if(buf[a] == ch) count++; }

	return count;

}

// Remove all occurances of char from string
char *remchar(char *input, char ch) {

	char *output = malloc(strlen(input));
	int a = 0, b = 0;

	for(a = 0; a < strlen(input); a++) {
		if(input[a] != ch) output[b++] = input[a];
	}

	return output;
}

// Remove all occurances of substring from string
char *remstr(char *input, char *rem) {

	char *output = malloc(sizeof(input));
	int remlen = strlen(rem);
	int a = 0, b = 0, c = 0;

	while(input[a]) {
		if(input[(a+b+c)] == rem[b]) {
			if(b == remlen - 1) {
				c += remlen;
				output[a] = input[(a+c)];
				a++;
				b=0;
			}
			else b++;
		} else {
			b=0;
			output[a] = input[(a+c)];
			a++;
		}
	}
	return output;
}

// Replaces all instances of char rch with char nch
char *repchar(char input[], char rch, char nch) {

	size_t len = strlen(input);
	char *output = malloc(len);

	for(int a = 0; a < len; a++) {
		if(input[a] == rch) output[a] = nch;
		else output[a] = input[a];
	}

	return output;
}

// Replaces all instances of string rep with string new
char *repstr(char input[], char ostr[], char nstr[]) {

	size_t totlen = strlen(input);
	size_t replen = strlen(ostr);
	size_t newlen = strlen(nstr);
	int a = 0, b = 0, c = 0;

	char *output = calloc((totlen / replen) * newlen, sizeof(char));

	while(input[a]) {
		if(input[(a+b)] == ostr[b]) {
			if(b == replen - 1) {
				strcat(output, nstr);
				a += replen;
				c += newlen;
				b=0;
			}
			else b++;
		} else {
			b=0;
			output[c] = input[a];
			a++;
			c++;
		}
	}

	output[c] = '\0';
	return output;
}

// Line break ostr at last space char - max unbroken line is blen
char *breakline(char *ostr, int blen) {

	size_t len = strlen(ostr);
	char *nstr = calloc((len + 1), sizeof(char));
	int last = 0;
	int prev = 0;
	int b = 0;

	for(int a = 0; a < len; a++) {
		if(isspace(ostr[a])) last = a;
		if(b > blen) {
			if(prev == last && last != 0) {
				strcpy(nstr, "ERROR");
				break;
			}
			nstr[last] = '\n';
			prev = last;
			a = last;
			b = 0;
		} else if (ostr[a] == '\n') {
			nstr[a] = ostr[a];
			b = 0;
		} else nstr[a] = ostr[a];

		b++;
	}

	nstr[(len+1)] = '\0';

	return nstr;
}

// Removes line breaks at end of buf
char *remtrail(char *buf) {

	size_t len = strlen(buf);

	char *buf2 = calloc(len, sizeof(char));

	strcpy(buf2, buf);

	for(int a = len - 1; a > 0; a--) {
		if(buf[a] == '\n') buf2[a] = '\0';
		else break;
	}

	return buf2;
}
