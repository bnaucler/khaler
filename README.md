# khaler v0.2A 
A parser for .ics files which feeds the data into khal.  

# Written by:  
Björn Westerberg Nauclér 2016  
mail@bnaucler.se

# License: 
None whatsoever. Do whatever you want with it,  
and let me know if you find it useful.

# Usage
It's simple enough to use. Takes an .ics file as the only argument and presents an interactive menu. 

# .mailcap
If you would like to use khaler with mutt, add the following lines to your .mailcap.  
text/calendar; khaler %s  
application/ics; khaler %s  
application/octet-stream; khaler %s

# TODO
* Create proper menu for calendar selection
* Read default calendar from khal config
	- and set to defult
* Proper Makefile(?)
* Proper email parsing
* Attendee listing
* Description listing
* File parsing as function
* Support for ics via stdin
* Check for khal and return 1 if not found
* Check for khal config in multiple locations
* RSVP via mutt pipe
* Sanitize calendar reading from khal config

# Disclaimer
Quick and dirty. Not even half done and probably full of bugs. Enjoy!
