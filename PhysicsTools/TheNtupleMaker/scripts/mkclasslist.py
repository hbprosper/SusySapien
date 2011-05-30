#!/usr/bin/env python
#------------------------------------------------------------------------------
# create classes.txt
# Created: 05-Jan-2010 Harrison B. Prosper
# Updated: 08-Aug-2010 HBP minor fix before release
#          25-Aug-2010 HBP add a few more classes (by hand)
#$Id: mkclasslist.py,v 1.16 2011/05/07 18:39:14 prosper Exp $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
#------------------------------------------------------------------------------
cwd = os.path.basename(os.environ['PWD'])
if cwd != "plugins":
	print "\t** must be run from plugins directory"
	sys.exit(0)

# Extract getByLabel strings using a non-greedy search
getfields = re.compile(r'(?<=")[^ ]*?(?=")')
isvector  = re.compile(r'^(?<=std::)?vector<')
isAvector = re.compile(r'edm::AssociationVector<')
skipme    = re.compile('Collection|edmNew|'\
					   'AssociationMap|ValueMap|RangeMap|OwnVector')
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

# For now, add some stuff by hand **** FIXME LATER
# It would be more elegant to have something like edmDumpContent <treename>.

records.append('GenRunInfoProduct "generator" "" "HLT."')
records.append('edm::Event "info" "" "RECO."')

# Get list of classes and labels

print "\t\twriting classes.txt ..."
tname = {}
for record in records:
	classname = strip(split(record, '"')[0])
	fields = getfields.findall(record)
	if len(fields) == 0: continue

	#Skip a bunch of complicated stuff, for now...
	if skipme.findall(classname) != []: continue
	
	# Determine whether or not this is a singleton
	if isvector.match(classname) != None:
		tname["C %s" % classname] = "C"
	elif isAvector.match(classname) != None:
		tname["C %s" % classname] = "C"
	else:
		tname["S %s" % classname] = "S"

keys = tname.keys()
keys.sort()
out = open("classes.txt",'w')
for index, key in enumerate(keys):
	record = "%s\n" % key
	out.write(record)
out.close()


