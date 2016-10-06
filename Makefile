CFLAGS=-Wall -g -I

all:
	cc -o khaler khaler.c getch.c datetime.c readkhalconfig.c getinput.c

clean:
	rm -f khaler

install:
	mv khaler /usr/local/bin
