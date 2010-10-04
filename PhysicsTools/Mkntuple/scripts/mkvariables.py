#!/usr/bin/env python
# -----------------------------------------------------------------------------
#  File:        mkvariables.py
#  Description: Make variables.txt from an ntuple, which can then be used to
#               drive mkanalyzer.py
#  Created:     Mon Oct  4, 2010
#  Author:      Harrison B. Prosper
# -----------------------------------------------------------------------------
from ROOT import *
from time import *
from string import *
import os, sys, re
# -----------------------------------------------------------------------------
def usage():
	print '''
mkvariables.py <ntuple-filename> <tree-name> 
	'''
	sys.exit(0)
argv = sys.argv[1:]
argc = len(argv)
if argc < 2: usage()

filename = argv[0]
if not os.path.exists(filename):
	print "\t** file %s not found" % filename
	sys.exit(0)
# -----------------------------------------------------------------------------
from PhysicsTools.Mkntuple.AutoLoader import *
# -----------------------------------------------------------------------------
getvtype = re.compile('(?<=vector[<]).+(?=[>])')
# -----------------------------------------------------------------------------
def main():
	
	treename = argv[1]
	stream = itreestream(filename, treename)
	if not stream.good():
		print "\t** hmmmm...something amiss here!"
		sys.exit(0)

	stream.ls()

	print
	print "==> file: %s" % filename
	print "==> tree: %s" % treename
	print "==> output: variables.txt"

	out = open("variables.txt", "w")
	out.write("%s\n" % ctime(time()))
	out.write("\n")
	
	records = map(split, split(stream.str(),'\n'))
	for x in records:

		# skip junk
		if len(x) == 0: continue
		if x[0] in ["Tree", "Number", ""]: continue

		# look for a leaf counter
		iscounter = x[-1] == "*" # look for a leaf counter
		if iscounter: x = x[:-1]
		
		if len(x) == 4:
			a, branch, c, btype = x
			maxsize = 1
		elif len(x) == 5:
			a, branch, c, btype, maxcount = x
			maxsize = 2 * atoi(maxcount[1:-1])
		else:
			print "\t**hmmm...not sure what to do with:\n\t%s\n\tchoi!" % x
			sys.exit(0)
			
		btype = replace(lower(btype), "_t", "")
		vtype = getvtype.findall(btype)
		if len(vtype) == 1:
			maxsize = 200
			btype = vtype[0]
		
		# Variable name
		if iscounter:
			lc = " *"
		else:
			lc = ""
			
		name = replace(branch, '.', '_')
		record = "%s/%s/%s/%d %s\n" % (btype, branch, name, maxsize, lc)
		out.write(record)
# -----------------------------------------------------------------------------
main()
