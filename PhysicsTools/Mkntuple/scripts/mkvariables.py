#!/usr/bin/env python
# -----------------------------------------------------------------------------
#  File:        mkvariables.py
#
#  Description: Scan a simple ntuple and create the file variables.txt that
#               describes its branches and leaves. The file variables.txt has
#               the following fields:
#              
#               type / branch[.leaf] / variable-name / maximum count
#
#               This file can then be used by mkanalyzer.py to create a
#               reasonably comprehensive (first) version of a an analyzer.
#
#  Created:     Mon Oct  4, 2010
#  Author:      Harrison B. Prosper
#  Email:       harry@hep.fsu.edu, Harrison.Prosper@cern.ch
#  Fixes:       15-Nov-2010 HBP make sure buffers have a count of at least 1
#               22-Nov-2010 HBP allow multiple trees
# $Revision: 1.4 $
# -----------------------------------------------------------------------------
from ROOT import *
from time import *
from string import *
import os, sys, re
# -----------------------------------------------------------------------------
def usage():
	print '''
mkvariables.py <ntuple-filename> <tree-name> [<tree-name2...]
	'''
	sys.exit(0)
	
# get command line arguments
argv = sys.argv[1:]
argc = len(argv)
if argc < 2: usage()

# get ntuple file name
filename = argv[0]
if not os.path.exists(filename):
	print "\t** file %s not found" % filename
	sys.exit(0)
# -----------------------------------------------------------------------------
# load treestream module
try:
	from PhysicsTools.Mkntuple.AutoLoader import *
except:
	try:
		gROOT.ProcessLine(".L treestream.cc+")
	except:
		print "\t** treestream.cc and treestream.h not found."
		print "\t** copy them to working directory and try again"
		sys.exit(0)
# -----------------------------------------------------------------------------
# extract vector type from vector<type>
getvtype = re.compile('(?<=vector[<]).+(?=[>])')
# -----------------------------------------------------------------------------
def main():

	# 2nd argument is the TTree name
	treename = joinfields(argv[1:], ' ') # Can have more than one tree
	stream = itreestream(filename, treename)
	if not stream.good():
		print "\t** hmmmm...something amiss here!"
		sys.exit(0)
	# list branches and leaves
	stream.ls()

	# write out variables.txt after scanning ntuple listing
	print
	print "==> file: %s" % filename

	tname = split(treename)
	for name in tname:
		print "==> tree: %s" % name
	print "==> output: variables.txt"

	out = open("variables.txt", "w")
	out.write("tree: %s\t%s\n" % (tname[0], ctime(time())))
	for name in tname[1:]:
		out.write("tree: %s\n" % name)
	out.write("\n")

	# get ntuple listing
	records = map(split, split(stream.str(),'\n'))
	for x in records:

		# skip junk
		if len(x) == 0: continue
		if x[0] in ["Tree", "Number", ""]: continue

		# Fields:
		# .. branch / type [maximum count [*]]
		
		# look for variables flagged as leaf counters
		iscounter = x[-1] == "*" # look for a leaf counter
		if iscounter: x = x[:-1] # remove "*" from the end

		if len(x) == 4:
			a, branch, c, btype = x
			maxcount = 1
		elif len(x) == 5:
			a, branch, c, btype, maxcount = x
			maxcount = 1 + 2 * atoi(maxcount[1:-1]) # double up...just in case!
		else:
			print "\t**hmmm...not sure what to do with:\n\t%s\n\tchoi!" % x
			sys.exit(0)

		# get branch type in C++ form (not, e.g.,  Double_t)
		btype = replace(lower(btype), "_t", "")
		vtype = getvtype.findall(btype)
		if len(vtype) == 1:
			btype = vtype[0] # vector type
			maxcount = 100   # default maximum count for vectors

		# fix a few types
		if btype[:-2] in ["32", "64"]:
			btype = btype[:-2]
			
		# If this is leaf counter, add " *" to end of record
		if iscounter:
			lc = " *"
		else:
			lc = ""

		# make a name for yourself
		name = replace(branch, '.', '_')
		# write out info for current branch/leaf
		record = "%s/%s/%s/%d %s\n" % (btype, branch, name, maxcount, lc)
		out.write(record)
# -----------------------------------------------------------------------------
main()
