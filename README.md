# khaler v0.2A
A parser for .ics files which feeds the data into [khal](https://lostpackets.de/khal/).  

## Written by:  
Björn Westerberg Nauclér 2016  
mail@bnaucler.se

## License: 
None whatsoever. Do whatever you want with it,  
and let me know if you find it useful.

## Installation
$ **sudo make all install**  
The executable will be installed in /usr/local/bin/  
(unless otherwise specified with DESTDIR)

## Usage
It's simple enough to use. Takes an .ics file as only argument and presents an interactive menu.  
Make sure to keep your khal config in $HOME/.config/khal/

## .mailcap
If you would like to use khaler with mutt, add the following lines to your .mailcap.  
**text/calendar; khaler %s  
application/ics; khaler %s  
application/octet-stream; khaler %s**

## TODO
* Smarter way to check for khal?
* Attendee listing RSVP delegation
* Include participation roles (req, opt, non)
* Organizer in attendee array?
* Implement popen for reading khal agenda
	- Read calendar colors from khal config?
* Time zone corrections
* Print functions to new .c?
* Reorganize project dir
* Separate parsing and print functions
* Read up on and implement memory allocation methods
* Finding events in raw text (pipe from mutt)?
	- As separate executable perhaps
	- Or enable getopt()
* Check for khal config in multiple locations
	- Break down readkhalconfig.c in multiple functions
* RSVP via mutt pipe (the actual work)
* Limit description length on term size?

## Disclaimer
Quick and dirty. About half done and full of bugs. Have fun!
