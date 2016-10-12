CC = cc
TARGET = khaler
SOURCE = *.c
DESTDIR = /usr/local/bin
CFLAGS= -Wall -g

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

install:
	mv $(TARGET) $(DESTDIR)
