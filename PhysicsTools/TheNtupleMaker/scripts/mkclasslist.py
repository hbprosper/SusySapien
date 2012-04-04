#!/usr/bin/env python
#------------------------------------------------------------------------------
# create classlist.txt
# Created: 05-Jan-2010 Harrison B. Prosper
# Updated: 08-Aug-2010 HBP minor fix before release
#          25-Aug-2010 HBP add a few more classes (by hand)
#          31-Mar-2012 HBP use directories defined by classmap.py
#                      simplify classes.txt format to one class per line
#$Id: mkclasslist.py,v 1.18 2012/04/04 01:32:41 prosper Exp $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from PhysicsTools.TheNtupleMaker.classmap import ClassToHeaderMap
from PhysicsTools.TheNtupleMaker.Lib import cmsswProject
#------------------------------------------------------------------------------
PACKAGE, SUBPACKAGE, LOCALBASE, BASE, VERSION = cmsswProject()
cwd = os.path.basename(os.environ['PWD'])
if PACKAGE == None:
	print "\n\t** Must be run from package directory"
	sys.exit(0)

# Extract getByLabel strings using a non-greedy search
getclass1  = re.compile(r'(?<=edm::Wrapper\<).+(?=\>")')
getclass2  = re.compile(r'(?<=edm::Wrapper\<).+(?=\> ")')
getfields = re.compile(r'(?<=")[^ ]*?(?=")')
isvector  = re.compile(r'(?<=std::vector\<).+(?=\>)')
isAvector = re.compile(r'^(?<=edm::AssociationVector\<).+(?=\>)$')

# classes to exclude
skipme    = re.compile('edmNew|'\
					   'edm::Ref|'\
					   '[*]|'\
					   'Collection|'\
					   'RefVector|'
					   'PtrVector|'
					   'AssociationMap|'\
					   'ValueMap|'\
					   'RangeMap|'\
					   'OwnVector|'\
					   'DetSet|'\
					   'Lazy')
#------------------------------------------------------------------------------
argv = sys.argv[1:]
argc = len(argv)

# Get list of directories

values = ClassToHeaderMap.values()
values.sort()
records = {}
for value in values:
	t = split(value, "/interface/")
	records[t[0]] = 0
records = records.keys()
records.sort()

# Get list of classes

wclasses = []
for record in records:
	xmlfile = "%s/%s/src/classes_def.xml" % (LOCALBASE, record)
	if not os.path.exists(xmlfile):
		xmlfile = "%s/%s/src/classes_def.xml" % (BASE, record)
		if not os.path.exists(xmlfile): continue

	recs = os.popen('grep "edm::Wrapper" %s' % xmlfile).readlines()
	for rec in recs:

		# fix string before getting class
		rec = strip(rec)
		rec = replace(rec, "&lt;", "<")
		rec = replace(rec, "&gt;", ">")

		classname = getclass1.findall(rec)
		if len(classname) == 0:
			classname = getclass2.findall(rec)
			if len(classname) == 0: continue
		classname = strip(classname[0])
		#print classname
		wclasses.append(classname)

# -------------------------
# Add some classes by hand
# -------------------------
wclasses.append("std::vector<reco::CaloCluster>")
wclasses.append("std::vector<reco::VertexCompositeCandidate>")
wclasses.sort()
#------------------------------------------------------------------------------
# Get list of classes

print "\t==> writing plugins/classlist.txt ..."
tname = {}
for classname in wclasses:

	#Skip a bunch of complicated stuff, for now...
	if skipme.findall(classname) != []: continue
	#print classname
	# Find singletons and collections

	t = isvector.findall(classname)
	if len(t) > 0:
		key = strip(t[0])
		tname[key] = classname
	else:
		if not tname.has_key(classname):
			tname[classname] = classname

# write out classes
keys = tname.keys()
keys.sort()
records = []
for index, key in enumerate(keys):
	records.append("%s\n" % tname[key])
records.sort()
print "\t==> number of classes: %d" % len(records) 
open("plugins/classlist.txt",'w').writelines(records)

