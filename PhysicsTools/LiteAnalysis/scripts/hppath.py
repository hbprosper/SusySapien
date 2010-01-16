#!/usr/bin/env python
#----------------------------------------------------------------------------
# File: hppath.py
# Description: For each specified header, make a txt file containing its path
# Created: 26-Oct-2005, Harrison B. Prosper
#$Revision: 1.2 $
#----------------------------------------------------------------------------
import sys
from string import *
from boostlib import hidepath
#----------------------------------------------------------------------------
# Command line
#----------------------------------------------------------------------------
argv = sys.argv[1:]
argc = len(argv)
if argc < 1:
    print "\nUsage:\n\thppath <headers>\n"
    sys.exit(1)
files = argv
for file in files:
    file = hidepath(file)
    name = split(split(file,'/').pop(),'.')[0]
    open("%s.txt" % name,'w').write('%s\n' % file)
    print name
    
