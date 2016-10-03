# khaler v0.2 
A parser for .ics files which feeds the data into khal.  

# Written by:  
Björn Westerberg Nauclér 2016  
mail@bnaucler.se

# License: 
None whatsoever. Do whatever you want with it,  
and let me know if you find it useful.

# Usage
It's simple enough to use. Takes an .ics file as the only argument and presents an interactive menu. 

# khal config
Currently the following lines are needed in your khal configuration, under [locale]:  
timeformat= %H:%M  
longdateformat= %Y-%m-%d  
(or edit datetime.c)

# .mailcap
It you would like to use khaler with mutt, add the following lines to your .mailcap.  
text/calendar; khaler %s  
application/ics; khaler %s  
application/octet-stream; khaler %s

# TODO
* Read existing calendars from khal config
	- create menu for selection
* Use khal import builtin file parsing
	- and include description
* Proper Makefile
* Selection parsing as function
* Move all of this crap to README.md
* Support for multiple calendars
* Support for ics via stdin

# Disclaimer
Quick and dirty. Not even half done and probably full of bugs. Enjoy!
