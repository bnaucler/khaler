#include "khaler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Replaces all instances of char rep with char new
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

	// char *output = malloc(totlen * newlen);
	// char *output = calloc(((totlen / replen) * newlen), sizeof(char));
	char *output = calloc(totlen * newlen, sizeof(char));

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
