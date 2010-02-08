#!/usr/bin/env python
#------------------------------------------------------------------------------
# create classes.txt, adapters.txt, headers.txt
# Created: 05-Jan-2010 Harrison B. Prosper
#$Revision: 1.2 $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
#------------------------------------------------------------------------------
cwd = os.path.basename(os.environ['PWD'])
if cwd != "plugins":
	print "\t** must be run from plugins directory"
	sys.exit(0)
	
# Extract branches ending in
getbranch    = re.compile(r'(?<=\_).+\. / .+$',re.M)

# Extract class name from branch name
getclass  = re.compile(r'(?<=Wrapper\<).+(?=\>)')

# Extract getByLabel string from branch name
getlabel  = re.compile(r'.+(?=\_\_)|.+(?=\_)')

# Strip away namespace, vector< etc.
stripname = re.compile(r'::|vector\<|\>| ')

def isVector(name):
	return find(name, "vector<") > -1
#------------------------------------------------------------------------------
argv = sys.argv[1:]
argc = len(argv)
if argc == 0:
	print "Usage:\n  mkclasslist.py <root-file> [root-file]\n"
	sys.exit(0)

# Get list of branches

records = []
for rootfile in argv:
	print "listing branches for %s ..." % rootfile
	cmd = "rlist.py %s Events" % rootfile
	record = os.popen(cmd).read()
	recs = getbranch.findall(record)
	if len(recs) == 0:
		print "Unable to list branches in " % rootfile
		sys.exit(0)
	records += recs

# Remove duplicates
d = {}
for x in records: d[x] = 0
records = d.keys()
records.sort()

open("branches.txt","w").writelines(joinfields(records, '\n'))

# Get list of classes and labels

print "writing classes.txt ..."
tname = {}
for record in records:
	label = getlabel.findall(record)[0]
	cname = getclass.findall(record)[0]
	if not tname.has_key(cname): tname[cname] = []
	if label[-1] == "_": label = label[:-1]
	tname[cname].append(label)

keys = tname.keys()
keys.sort()
out = open("classes.txt",'w')
for index, key in enumerate(keys):
	names = tname[key]
	record = "%4d %s\n" % (index, key)
	out.write(record)
	for name in names:
		record = "\t%s\n" % name
		out.write(record)
	out.write("\n")
out.close()


