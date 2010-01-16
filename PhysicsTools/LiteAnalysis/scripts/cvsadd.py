#!/usr/bin/env python
#----------------------------------------------------------------------------
# File: cvsadd.py
# Description: Recursively add files to cvs
# Created: 13-Jun-2006, Harrison B. Prosper
#$Revision: 1.2 $
#----------------------------------------------------------------------------
import sys, os, re
from string import *
#----------------------------------------------------------------------------
skip = re.compile(r'\.([vV][1-9]+|so|log|old|gif|root|pyc|d|#|~)$')
skipfile = re.compile(r'(\.cpp.h|\.header.h|a.txt|'\
					  '\.Bayespack.out|log)$')

recs = map(strip, os.popen("find .").readlines())
dirmap = {}
for record in recs:

	if skip.search(record) != None: continue
	
	if os.path.isdir(record):
		d = record
		if d == ".": continue
		f = os.path.basename(d)
		if f in ["copy", "CVS", "tmp", "logs"]: continue
		if not dirmap.has_key(d):
			dirmap[d] = []
			#print "DIR( %s )" % d
	else:
		d = os.path.dirname(record)
		if not dirmap.has_key(d): continue
		if skipfile.search(record): continue
		
		dirmap[d].append(record)
		#print "  FILE( %s )" % record

dirlist = dirmap.keys()
dirlist.sort()
for d in dirlist:
	files = dirmap[d]
	command = "cvs add %s" % d
	print command
	os.system(command)
	
	for f in files:
		command = "cvs add %s" % f
		print "\t%s" % command
		os.system(command)
		
	
