# khaler v0.3A
A parser for .ics files which feeds the data into [khal](https://lostpackets.de/khal/).  

## Written by  
Björn Westerberg Nauclér 2016  
mail@bnaucler.se

Compiled and tested on Mac OS Sierra and FreeBSD.

## Installation
`$ sudo make all install`  
The executable will be installed in /usr/local/bin/  
(unless otherwise specified with DESTDIR)

## Configuration
You can create a .khaler file in your $HOME, containing:  
`email=(your email address, can be repeated up to 10x)  
kpath=(full path to khal)  
kconfigpath=(full path to your khal.conf)`  
Normally this is not needed. khaler can read your email address from .muttrc, find khal and your khal.conf unless they are located in odd places.

## Usage
It's pretty simple to use. Takes an .ics file as only argument and presents an interactive menu. Let me know if something is not self-explanatory.

## .mailcap
If you would like to use khaler with mutt, add the following lines to your .mailcap.  
`text/calendar; khaler %s  
application/ics; khaler %s  
application/octet-stream; khaler %s`

## TODO
* Create die function?
* Support for recurring events
* Email client send string with variables
	- and set a mutt default
* Attendee listing RSVP delegation
* Include participation roles (req, opt, non)
* Organizer in attendee array?
* Implement popen for reading khal agenda
	- Read calendar colors from khal config?
* Time zone corrections
	- Day, month, year wrap
	- Month length, leap years etc
	- There should be a library for this
* Print functions to new .c?
* Reduce number of global variables
* Merge consdate and conedate
* Merge read(X)conf files
* Merge get(X)input functions
* Reorganize project dir
* Separate parsing and print functions
* Finding events in raw text (pipe from mutt)?
	- As separate executable perhaps
	- Or enable getopt()
* RSVP via mutt pipe (the actual work)
	- create temp file with randstr
	- change PARTSTAT
	- determine minimal data needed
	- pipe to determined send string
* Limit description length on term size?

## Disclaimer
Quick and dirty. About half done and full of bugs. Have fun!

## License:
MIT  
(do whatever you want)
