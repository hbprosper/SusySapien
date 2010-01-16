#!/usr/bin/env python
#-----------------------------------------------------------------------------
# Filename: mkadapter.py
# Description: make simple adapters for DataFormat classes
# Created:  10-Doc-2009 HBP
#-----------------------------------------------------------------------------
import os, sys, re
from sys    import exit
from time   import *
from string import *
from getopt import getopt, GetoptError
from pprint import PrettyPrinter
from boostlib import nameonly, populateDB
#-----------------------------------------------------------------------------
if not os.environ.has_key('PYTHON_PROJECTS'):
	print 'PYTHON_PROJECTS not defined!\n'
	exit(0)
BASE = os.environ['PYTHON_PROJECTS']
PP = PrettyPrinter()
def usage():
	print '''
Usage:
    mkadapter.py [options] <LIS-file1> [LIS-file2 ...]
				 -e=<editsfile> add methdods contained in edits file
				 '''
	sys.exit(0)

SHORTOPTIONS = 'he:'

TEMPLATE='''#ifndef %(ADAPTERCLASSNAME)s_H
#define %(ADAPTERCLASSNAME)s_H
//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: LiteAnalysis
// Class:       %(adapterclassname)s
// Description: Simple adapter for:
//              class:   %(classname)s
//              header:  %(header)s
//              
// Created:     %(time)s by mkadapter.py  HBP
//-----------------------------------------------------------------------------
#include "%(header)s"
//-----------------------------------------------------------------------------
class %(adapterclassname)s : public %(classname)s
{
 public: 
  %(adapterclassname)s() {}
  %(adapterclassname)s(const %(classname)s& o)
    : %(classname)s(o) {}
  ~%(adapterclassname)s() {}

%(methods)s
};
#endif
'''
retype = re.compile(r'float|double|int|unsigned|bool|\bsize_t\b')
stripnamespace = re.compile(r'\w+::')
stripcolon = re.compile(r':')
striptemplatepars = re.compile(r'\<.*\>')
stripconst = re.compile(r'\bconst ')
striparg   = re.compile(r'\bconst |\w+::|\&')
stripptr   = re.compile(r'\*$')
methodname = re.compile(r"^.*\(.*\)(?= const)")
retype = re.compile(r'float|double|int|unsigned|bool|\bsize_t\b')
simpletype =re.compile(r'void|float|double|int|unsigned|bool|\bsize_t\b|string')
striprtype = re.compile(r'\W+::|\bconst |\*$|&$|Ref$')
striprtypeless = re.compile(r'\bconst |\*$|&$|Ref$')
stripname = re.compile(r'[ ,\<\>:*&]')

DEBUG3 = 0
#----------------------------------------------------------------------------
def expandMethod(filename, methlist):
	records = map(strip,open(filename).readlines())

	# Ok now, get simple methods

	for record in records:
		record = split(record, '|')
		if len(record) != 4: continue
		
		ctype, rtype, name, atype = record

		if DEBUG3 > 1: print "\t\texpandMethod RTYPE( %s, %s )" % (rtype, name)
		
		if rtype == "void": continue
		if atype != "void": continue
		if retype.match(rtype) == None: continue
		
		if DEBUG3 > 1: print "\t\texpandMethod RTYPE( %s, %s )" % (rtype, name)
		
		# We have a simple return type
		methlist.append((rtype, name))
#----------------------------------------------------------------------------
def extraMethods(filename):
	if filename == "": return {}
	if not os.path.exists(filename): return {}
	execfile(filename)
	edits = {}
	for x in dir():
		if x in ["filename", "edits"]: continue
		edits[x] = eval(x)
	return edits
#----------------------------------------------------------------------------
def fixreturnAndarg(rtype, atype):
	# Fix default return values

	if rtype == "bool":
		val = "false"
	elif find(rtype, "size_t" ) > -1:
		val = "999999"
	elif find(rtype, "unsigned") > -1:
		if find(rtype, "short") > -1:
			val = "9999"
		else:
			val = "999999"
	else:
		val ="-999999"

	# Fix argument
	if atype == "void":
		arg = ""
		var = ""
	else:
		# strip away "&"
		if atype[-1] == "&": atype = atype[:-1]

		# check for default value
		t = split(atype,"=")
		if len(t) == 2:
			arg = "%s x=%s" % tuple(t)
		else:
			arg = "%s x" % atype
		var = "x"
	return (val, arg, var)
#----------------------------------------------------------------------------
def makeAdapter(filename, db, editsfile):
	
	if find(filename,'_') > -1: return

	# Evaluate edits file, if specified
	
	edits = extraMethods(editsfile)

	names = {}
	tab = "//              "

	records = map(strip,open(filename).readlines())

	# Extract header and classname
	header = split(filename,'/')[-1]
	t = split(header, '.')
	header    = "%s/%s/interface/%s.h" % tuple(t[:3])
	classname = split(records[0],'|')[0]
	if DEBUG3 > 0:
		print "\theader: %s\t\n\t\tclassname: %s" % (header, classname)

	# Return if this is a template class

	if find(classname, "<") > -1:
		print "\tskipping template class: %s" % classname
		return
	
	# Construct adapter class name etc.
	
	adapterclassname = "a%s" % stripcolon.sub("",classname)
	headerfilename   = striptemplatepars.sub("",adapterclassname)

	if find(headerfilename,'>') > -1: return
	if find(headerfilename,'<') > -1: return

	print "processing: %s" % filename
	
	records = map(lambda x: split(x,'|'), records)

	# Ok now, create methods

	expanded = False

	index = 0
	meths = []
	methods = []
	for record in records:
		if len(record) != 4: continue

		# Take are not to overwrite these variables
		
		ctype, rtype, name, atype = record

		# Keep only simple methods
		
		if name  == "clone": continue
		if rtype == "void": continue
		arg = striparg.sub("", atype)

		if simpletype.match(arg) == None: continue

		if DEBUG3 > 0: print "\tRTYPE( %s ) METHOD( %s ) ARG( %s )" % \
		   (rtype, name, atype)

		if retype.match(rtype) != None:
			
			# We have a simple return type

			val, arg, var = fixreturnAndarg(rtype, atype)
			
			# identify simple return type with * at end of string

			method = "  %s\t%s(%s) const \t{return %s(%s);}*" % \
					 (rtype, name, arg, name, var)
			methods.append(method)
			meths.append(("%s %s" % (name, rtype), index))
			index += 1
		else:
			
			# expand complex return type

			cname = striprtypeless.sub("", rtype)
			if DEBUG3 > 0: print "\t\tCOMPLEX RTYPE( %s )" % cname

			if not db['class'].has_key(cname):
				if DEBUG3 > 0:
					print "\t\t*** class %s NOT found in db" % cname
				continue

			# Get class info

			include, cl = db['class'][cname]
			lisfile = replace(include,'.h','')
			lisfile = replace(lisfile,'/','.')
			lisfile = replace(lisfile,'interface.','')
			lisfile = "lis/%s.%s.lis" % \
					  (lisfile, stripname.sub("_", cl.attrib['name']))

			if not os.path.exists(lisfile):
				if DEBUG3 > 0:
					print "\t\t*** lisfile %s NOT found" % lisfile
				continue

			if DEBUG3 > 0:
				print "\t\t\tLISFILE( %s )" % lisfile
				print "\t\t\t\tRETURN TYPE( %s )" % cname
				
			# lis file exists, so proceed 
			methlist = []
			expandMethod(lisfile, methlist)
			if len(methlist) == 0: continue

			methlist.sort()

			expanded = True

			# write method

			ispointer = rtype[-1] == "*"
			isRef     = rtype[-3:] == "Ref"
			
			for rtype, fname in methlist:

				val, arg, var = fixreturnAndarg(rtype, atype)
				
				method = ""
				if ispointer:
					method += "  %s %s_%s(%s) "\
							  "const \t{return %s(%s) ? " % \
							  (rtype, name, fname, arg, name, var)
					method += "%s(%s)->%s() : %s;}" % (name, var, fname, val)
				elif isRef:
					method += "  %s %s_%s(%s) "\
							  "const \t{return %s(%s).isNonnull() ? " % \
							  (rtype, name, fname, arg, name, var)
					method += "%s(%s)->%s() : %s;}" % (name, var, fname, val)
				else:
					method += "  %s %s_%s(%s) "\
							  "const \t{return %s(%s).%s();}" % \
							  (rtype, name, fname, arg, name, var,fname)

				methods.append(method)
				meths.append(("%s %s %s" % (name, fname, rtype), index))
				index += 1

				##D
				if DEBUG3 > 0:
					print "\tEXPANDEDMETHOD( %s %s_%s )" % (rtype, name, fname)
					
	# sort
	meths.sort()
	simmethods = []
	extmethods = []
	for x, index in meths:
		method = methods[index]
		if not method[-1] == "*":
			extmethods.append(method)
		else:
			method = method[:-1]
			simmethods.append(method)

	methods = extmethods

	# Check whether to add edits

	if edits.has_key(adapterclassname):
		method = '\n  // -------------------------------'
		methods.append(method)
		method =   '  // --- Methods added by "hand" ---'
		methods.append(method)
		method =   '  // -------------------------------'
		methods.append(method)

		meths = edits[adapterclassname]
		for method in meths:
			method = '  %s\n' % strip(method)
			methods.append(method)

	# Write adapter header only if we had to expand methods
	# or methods were added by "hand"

	if expanded:

		print "\t\t\t** expanded methods"

		names['bases'] = "//"		
		names['time'] = ctime(time())
		names['classname'] = classname
		names['header']    = header
		names['adapterclassname'] = adapterclassname
		names['ADAPTERCLASSNAME'] = upper(adapterclassname)
		names['file'] = file
		names['methods'] = joinfields(methods,'\n')

		str = TEMPLATE % names
		os.system("mkdir -p adapters")
		open("adapters/%s.h" % headerfilename, 'w').write(str+'\n')


	# Finally, write a simple summary document for this class
	allmethods = simmethods + extmethods
	methods = map(lambda z: z[0],
				  filter(lambda y: len(y) > 0,
						 map(lambda x: methodname.findall(x), allmethods)))

	methods = map(strip, methods)
	str = '%s\n' % joinfields(methods,'\n')
	os.system("mkdir -p methods")
	open("methods/%s.txt" % headerfilename, 'w').write(str+'\n')
#----------------------------------------------------------------------------
def main():

	# Decode command line
	
	try:
		opts, infiles = getopt(sys.argv[1:], SHORTOPTIONS)
	except GetoptError, m:
		print
		print m
		usage()

	# Decode option, value pairs

	editsfile = ''
	for option, value in opts:

		if option == "-h":
			usage()

		elif option == "-e":
			editsfile = value
			
	if len(infiles) == 0: usage()

	
	for filename in infiles:
		t = split(split(filename, '/')[-1],'.')
		include = "%s/%s/interface/%s.h" % tuple(t[:3])

		db = {}
		populateDB(BASE, include, db)
		makeAdapter(filename, db, editsfile)
#----------------------------------------------------------------------------
main()
