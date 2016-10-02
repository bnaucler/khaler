CFLAGS=-Wall -g

all:
	cc khaler.c -o khaler

clean:
	rm -f khaler

install:
	mv khaler /usr/local/bin
