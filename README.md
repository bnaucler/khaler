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
`email=(your email address, can be repeated up to 10x)`  
`kpath=(full path to khal)`  
`kconfigpath=(full path to your khal.conf)`  
`tempdir=(full path to a writeable directory for temp files)`  
`sendstring=(default: /usr/local/bin/mutt -s RSVP $recepient -a $file)`  
`debug=(set to nonzero number for debugging functionality)`  
Normally this is not needed. khaler can read your email address from .muttrc, your tempdir from environmental variables and find khal as well as your khal.conf unless they are located in odd places. Functions available only with debug set to nonzero are completely unsupported.

## Usage
It's pretty simple to use. Takes an .ics file as only argument and presents an interactive menu. Let me know if something is not self-explanatory.

## .mailcap
If you would like to use khaler with mutt, add the following lines to your .mailcap.  
`text/calendar; khaler %s`  
`application/ics; khaler %s`  
`application/octet-stream; khaler %s`

## TODO
* Create die function?
* Sanitize readconfig input
* Support for recurring events
* Attendee listing RSVP delegation
* Increased number of variables for send string
* Change procinput to switch statement
* Include participation roles (req, opt, non)
* Time zone corrections
	- Convert to time.h
	- Day, month, year wrap
	- Month length, leap years etc
* Print functions to new .c?
* Sanitize parsebuf (only match beginning of str)
* Reduce number of global variables
* Fix highlighting bug when inviting yourself
* Limit header lines on terminal size
* Listing of only email address if attendee name is unknown
* Getenv $PAGER
* Show in khal if accepted tentatively?
* Merge consdate and conedate
* Reorganize project dir
* Config object for standard message / signature file?
* Verify temp file does not already exist
* Separate parsing and print functions
* Finding events in raw text (pipe from mutt)?
	- Read from stdin if no file name specified
	- Determine if ics stream or raw text
* RSVP via mutt pipe (the actual work)
	- debug mode enabling raw edit
	- determine minimal data needed
	- add METHOD & ...?
* Limit description length on term size
* Limit # of attendees listed dep on term size

## Disclaimer
Quick and dirty. About half done and full of bugs. Have fun!

## Test data
.. is very welcome. Send me your weirdest .ics files.

## License:
MIT  
(do whatever you want)
