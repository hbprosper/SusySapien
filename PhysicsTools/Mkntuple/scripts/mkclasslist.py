#!/usr/bin/env python
#------------------------------------------------------------------------------
# create classes.txt
# Created: 05-Jan-2010 Harrison B. Prosper
# Updated: 08-Aug-2010 HBP minor fix before release
#$Revision: 1.3 $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
#------------------------------------------------------------------------------
cwd = os.path.basename(os.environ['PWD'])
if cwd != "plugins":
	print "\t** must be run from plugins directory"
	sys.exit(0)

# Extract getByLabel strings using a non-greedy search
getfields  = re.compile(r'(?<=")[^ ]*?(?=")')
#------------------------------------------------------------------------------
argv = sys.argv[1:]
argc = len(argv)
if argc == 0:
	print "Usage:\n  mkclasslist.py <root-file> [root-file]\n"
	sys.exit(0)

# Get list of branches

record = ''
for rootfile in argv:
	if not os.path.exists(rootfile):
		print "*** error *** File %s not found" % rootfile
		sys.exit(0)
		
	print "\tlisting branches for file: %s" % rootfile
	cmd = "edmDumpEventContent %s" % rootfile
	record += os.popen(cmd).read()
records = split(record, '\n')

# For now, add GenRunInfoProduct "generator" "" "HLT." to the records by hand.
# It would be more elegant to have something like edmDumpContent <treename>.
records.append('GenRunInfoProduct "generator" "" "HLT."')

# Get list of classes and labels

print "\t\twriting classes.txt ..."
tname = {}
for record in records:
	classname = strip(split(record, '"')[0])
	fields = getfields.findall(record)
	if len(fields) == 0: continue
	if classname in ['double', 'int', 'bool']: continue

	if not tname.has_key(classname): tname[classname] = {}
	label = fields[0]
	if fields[1] != '': label += "_" + fields[1]
	tname[classname][label]=0

keys = tname.keys()
keys.sort()
out = open("classes.txt",'w')
for index, key in enumerate(keys):
	names = tname[key].keys()
	names.sort()	
	record = "%4d %s\n" % (index, key)
	out.write(record)
	for name in names:
		record = "\t%s\n" % name
		out.write(record)
	out.write("\n")
out.close()


