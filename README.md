# khaler.c

A parser for .ics files which feeds the data into khal.  
See header of khaler.c for more information.

Knocked this together to add .ics files to khal from mutt. If you want do to the same, add the following lines to your .mailcap once khaler has been installed:  
text/calendar; khaler %s  
application/ics; khaler %s  
application/octet-stream; khaler %s

Quick and dirty. Not even half done and probably full of bugs. Enjoy!
