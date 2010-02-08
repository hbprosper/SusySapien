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
from boostlib import nameonly, readMethods
from classmap import ClassToHeaderMap
#-----------------------------------------------------------------------------
if not os.environ.has_key('CMSSW_BASE'):
	print 'CMSSW_BASE not defined!\n'
	exit(0)
BASE = "%s/src/PhysicsTools/LiteAnalysis" % os.environ['CMSSW_BASE']
PP = PrettyPrinter()
def usage():
	print '''
Usage:
    mkadapter.py <TXT-file1> [TXT-file2 ...]
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
// Created:     %(time)s by mkadapter.py
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

BLACK = "\x1b[0;30;48m" # Ctrl[attribute;foreground;backgroundm
RED   = "\x1b[0;31;48m" # \x1b 0,1,...   30+color   40+color

retype = re.compile(r'float|double|int|unsigned|bool|\bsize_t\b')
stripnamespace = re.compile(r'\w+::')
stripcolon = re.compile(r':')
striptemplatepars = re.compile(r'\<.*\>')
stripconst = re.compile(r'\bconst ')
striparg   = re.compile(r'\bconst |\w+::|\&')
stripptr   = re.compile(r'\*$')
methodname = re.compile(r"^.*\(.*\)(?= const)")
retype = re.compile(r'float|double|int|unsigned|bool|\bsize_t\b')
simpletype=re.compile(r'void|float|double|int|unsigned|bool|\bsize_t\b|string')
striprtype = re.compile(r'\W+::|\bconst |\*$|&$|Ref$')
striprtypeless = re.compile(r'\bconst |\*$|&$|Ref$')
stripname = re.compile(r'[ ,\<\>:*&]')
skipmethod= re.compile(r'clone')

DEBUG3 = 0
#----------------------------------------------------------------------------
def expandMethod(filename, methlist):

	header, classname, basenames, methodlist = readMethods(filename)

	# Ok now, get simple methods

	for rtype, name, atype, record in methodlist:

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
		
		t = split(atype,",")
		arg = '%s x0' % t[0]
		var = 'x0'
		for i in xrange(1,len(t)):
			arg += ", %s x%d" % (t[i], i)
			var += ", x%d" % i
	return (val, arg, var)
#----------------------------------------------------------------------------

def makeAdapter(filename, editsfile):
	
	if find(filename,'_') > -1: return
	
	# Evaluate edits file, if specified
	
	edits = extraMethods(editsfile)

	names = {}
	tab = "//              "

	header, classname, basenames, methodlist = readMethods(filename)
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
	
	# Ok now, process methods

	expanded = False

	index = 0
	meths = []
	methods = []
	for rtype, name, atype, record in methodlist:

		# Take are not to overwrite these variables

		if DEBUG3 > 0: print "\n\tRTYPE( %s ) METHOD( %s ) ARG( %s )" % \
		   (rtype, name, atype)

		# Skip some methods
		
		if skipmethod.match(name) != None: continue
		
		if rtype == "void": continue
		arg = striparg.sub("", atype)

		if simpletype.match(arg) == None: continue

		if DEBUG3 > 0:
			print "\t\tKEEP"

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

			# Get header for this class from ClassToHeaderMap

			if not ClassToHeaderMap.has_key(cname):
				if DEBUG3 > 0:
					print "\t\t** header for class %s NOT found" % cname
				continue
			
			headerfile = ClassToHeaderMap[cname]
			filestem = replace(headerfile, 'interface/', '')
			filestem = split(filestem, '.h')[0]
			filestem = replace(filestem, '/', '.')
			cname  = split(cname,'::').pop()
			txtfilename = "txt/%s.%s.txt" % (filestem, cname)

			if not os.path.exists(txtfilename):
				if DEBUG3 > 0:
					print "\t\t*** file %s NOT found" % txtfilename
				continue

			if DEBUG3 > 0:
				print "\t\t\tTXTFILE( %s )" % txtfilename
				print "\t\t\t\tRETURN TYPE( %s )" % cname
				
			# txt file exists, so proceed 
			methlist = []
			expandMethod(txtfilename, methlist)
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

		print RED+"\t\t\t** adapter created" + BLACK

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
		makeAdapter(filename, editsfile)
#----------------------------------------------------------------------------
main()
