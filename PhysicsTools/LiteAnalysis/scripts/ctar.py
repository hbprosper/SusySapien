#!/usr/bin/env python
#----------------------------------------------------------------------------
# File: ctar.py
# Description: add files to a tar file
# Created: 13-Jan-2010, Harrison B. Prosper
#$Revision:$
#----------------------------------------------------------------------------
import sys, os, re
from string import *
from time import *
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

# Now add to tar file

dirlist = dirmap.keys()
dirlist.sort()

argv = sys.argv[1:]
if len(argv) == 0:
	tarfile = "code.tar"
else:
	tarfile = argv[0]

out = open("MANIFEST","w")
out.write("%s\n" % ctime(time()))
first = True
for d in dirlist:
	files = dirmap[d]
	for f in files:
		out.write(f+'\n')
		if first:
			first = False
			os.system("tar cvf %s %s" % (tarfile, f))
		else:
			os.system("tar uvf %s %s" % (tarfile, f))
out.close()

