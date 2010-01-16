#!/usr/bin/env python
#-----------------------------------------------------------------------------
# tag files listed in given file, whose default name is MANIFEST
#$Revision:$ HBP
#-----------------------------------------------------------------------------
import os, sys, re
from string import *
from getopt import getopt, GetoptError
#-----------------------------------------------------------------------------
skipdir = re.compile(r'dataformats/lis|dataformats/txt|src/DataFormats')
skipext = re.compile(r'\.([vV][1-9]+|so|log|old|gif|root|pyc|d|#|~)$')
skipfile= re.compile(r'CVS|MANIFEST|\.cpp.h|\.header.h|a.txt|'\
					  '\.Bayespack.out|log|XmlMap.py.*')
SHORTOPTIONS = 'ha'
def usage():
	print """
Usage:
    cvstag.py [options] <tagname> [MANIFEST]

	options
	     -h     print this
	     -a     tag all files

                default - skip files in:
				dataformats/lis
				dataformats/txt
				src/DataFormats
	"""
	sys.exit(0)
#-----------------------------------------------------------------------------
# Decode command line
	
try:
	opts, other = getopt(sys.argv, SHORTOPTIONS)
except GetoptError, m:
	print
	print m
	usage()

# Decode option, value pairs

skip = True
for option, value in opts:
	if option == "-h":
		usage()
		
	elif option == "-a":
		skip = False
			
other = other[1:]
if len(other) == 0: usage()

tagname = other[0]
if len(other) > 1:
	manifest = other[1]
else:
	manifest = "MANIFEST"
if not os.path.exists(manifest):
	print "file %s not found" % manifest
	sys.exit(0)
	
filenames = map(strip, open(manifest))
for filename in filenames:
	if filename == ".": continue
	if skipfile.search(filename) != None: continue
	if skipext.search(filename)  != None: continue
	
	if skip:
		if skipdir.search(filename) != None: continue
	
	command = "cvs tag %s %s" % (tagname, filename)
	print command
	os.system(command)
	
