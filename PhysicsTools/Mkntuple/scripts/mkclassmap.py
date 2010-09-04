#!/usr/bin/env python
#---------------------------------------------------------------------------
# File:        mkclassmap.py
# Description: Create a map of classnames to headers
# Created:     26-Aug-2010 Harrison B. Prosper
#$Revision: 1.2 $
#---------------------------------------------------------------------------
import os, sys, re
from ROOT import *
from string import *
from time import *
from glob import glob
from getopt     import getopt, GetoptError
#---------------------------------------------------------------------------
# Constants
#---------------------------------------------------------------------------
# Check for a CMSSW release
if not os.environ.has_key("CMSSW_RELEASE_BASE"):
	print "\t*** Please setup a CMSSW release\n"
	sys,exit(0)

if not os.environ.has_key("CMSSW_BASE"):
	print "\t*** Please setup a CMSSW release\n"
	sys,exit(0)
	
MAXCONSTRUCT=5000

shortOptions = "Hu:"

USAGE='''
Usage:
  mkclassmap.py [options] [sub-package1 [sub-package2...]]

  options
  -u<sub-package-path>   Update classmap by scanning given sub-package-path

  example:
      mkclassmap.py -u$CMSSW_BASE/src/MyArea/MyAnalysis

  default sub-packages:
                DataFormats/* 
				SimDataFormats/*
				FWCore/Framework
				FWCore/Utilities
				FWCore/Common
				PhysicsTools/Mkntuple
'''
def usage():
	print USAGE
	sys.exit(0)

PWD = os.path.realpath(os.environ['PWD'])
BASE      = "%s/src" % os.environ["CMSSW_RELEASE_BASE"]
LOCALBASE = "%s/src" % os.environ["CMSSW_BASE"]
VERSION   = os.environ["CMSSW_VERSION"]
#------------------------------------------------------------------------------
# Determine project directory
project = split(replace(PWD, LOCALBASE + "/", ""), '/')
if len(project) < 2:
	print "\t**Please run mkclassmap.py in your subpackage directory"
	sys.exit(0)

PACKAGE, SUBPACKAGE = project[:2]
PROJECTBASE = "%s/%s/%s"   % (LOCALBASE, PACKAGE, SUBPACKAGE)
PYTHONDIR   = "%s/python"   % PROJECTBASE  
#------------------------------------------------------------------------------
# Decode command line
argv = sys.argv[1:]
try:
	opts, pkgs = getopt(argv, shortOptions)
except GetoptError, m:
	print
	print m
	usage()

Update = False
PKGBASE = "%s/" % BASE # default: scan $CMSSW_RELEASE_BASE
CLASSMAPFILE = '%s/classmap.py' % PYTHONDIR
subpkgs = []

for option, value in opts:
	if option == "-H":
		usage()
		
	elif option == "-u":
		Update = True
		subpkgs = split(value)
		if os.path.exists(CLASSMAPFILE):
			execfile(CLASSMAPFILE)
			PKGBASE = ""
		else:
			Update = False
			PKGBASE = LOCALBASE
			subpkgs = ["%s/%s", (PACKAGE, SUBPACKAGE)]
			
if len(subpkgs) > 0:
	SUBPACKAGELIST = subpkgs
else:
	SUBPACKAGELIST = ["DataFormats/*",
					  "SimDataFormats/*",
					  "FWCore/Framework",
					  "FWCore/Common",
					  "FWCore/Utilities"
					  ]
#------------------------------------------------------------------------------
DEBUG=0
if DEBUG > 0:
	OUTDEBUG = open("debug.h","w")
else:
	OUTDEBUG = None
	
PLACEHOLDERS =['namespace',
			   'endnamespace',
			   'class',
			   'endclass',
			   'structclass',
			   'endstructclass',
			   'inlineclass',
			   'inlinestructclass']
WEIRD        = "<|<@&@>|>"
# Load needed libraries
#from PhysicsTools.LiteAnalysis.AutoLoader import *
#gSystem.Load("libPhysicsToolsMkntuple")
#---------------------------------------------------------------------------
# Exceptions
#---------------------------------------------------------------------------
class ImConfused(Exception):
	def __init__(self, msg):
		self.msg = msg

	def __str__(self):
		return `self.value`
#===========================================================================
# Heuristic regular expressions
#
# Notes:
# 1. Use named groups (?P<groupname>....) so that we can retrieve matched
#    strings by group name.
#
# 2. Use lookaheads (?=...), (?!...) and lookbehinds (?<=...), (?<!...)
#    to simplify regexes and make matches more robust. Note: lookaheads and
#    lookbehinds as zero width matches so they do not consume characters.
#
# 3. Occasionally, we use raw strings r'....' to make the escaping of
#    special characters less of a chore.
#===========================================================================
beginnamespace= '(?P<namespace>' \
			   '\\bnamespace\\b\s*(\w+)?\s*[^\{]*(?=\{)\{)'

endnamespace  = '(?P<endnamespace>\}(?!;))'

templatecmd  = '(?P<template>template'\
			   '\s*(<.*>|([^\{\n]+\n)+)\s*)'
istemplate   = re.compile(templatecmd)

specializedtemplatecmd  = '(?P<template>template\s*<\s*>\s*)'
isspecializedtemplate   = re.compile(specializedtemplatecmd)

begintemplate= '(?P<item>^template([^\{]+)+\s*(?=\{)\{)'

beginclass   = '(?P<class>' \
			   '^[ \t]*%s?' \
			   '(class|struct)[ \t]+[^;\(\{]+(?=\{)\{)' % templatecmd

endclass     = '(?P<endclass>\};)'

struct       = '(?P<struct>^[ \t]*struct'   \
			   '(\s+\w+)?\s*'   \
			   '\{[^\}]+\}'     \
			   '(\s*\w+)?[ \t]*;)'
#===========================================================================
# Compiled regular expressions
#===========================================================================

# subsystems to ignore

skipsubsystem = re.compile('Alignment|'\
						   'Geometry|'\
						   'Histograms|'\
						   'Provenance|'\
						   'Road|'\
						   'StdDict|'\
						   'Streamer|'\
						   'TestObj|'\
						   'VZero|'\
						   'Wrapped')

skipheader = re.compile('(classes|Fwd|print).h$')

leadingblanks= re.compile('(?P<leadingblanks>(^[ \t]*\n)+)',re.M)

# Return class preambles (that is, classs <name>.... {)

beginclasses = re.compile(beginclass,re.M)

# Return template preambles

begintemplates = re.compile(begintemplate,re.M)

# Return namespace preambles (that is, namespace <name> {)

namespaces = re.compile(beginnamespace,re.M)

classtitle = re.compile('%s?'\
						'\s*(class|struct)\s+' \
						'(?P<classtitle>[a-zA-Z]\w*[^\{]*\{)' % \
						templatecmd, re.M)

classtype  = re.compile('(?P<classtype>\\b(class|struct)\\b)',re.M)

# Return string containing base classes

basenames = re.compile('(?P<basenames>' \
					   '\s*(?<!:):(?!:)\s*(public|private|protected)?[^\{]+)')

findInlineComments      = re.compile('[ \t]*//[^\n]*|[ \t]*/[*].*(?=\*/)\*/')
getclassname = re.compile(r"\bclass[ \t]+(?P<name>\w+)[ \t]*;",re.M)
#===========================================================================
# Functions
#===========================================================================
def fatal(s):
	print "** Error ** %s" %s
	print "\tgoodbye!"
	sys.exit(1)

def debug(code,records,hasTitle=1):
	if DEBUG >= code:
		if hasTitle:
			OUTDEBUG.write("\n\t\t<==== %s ====>\n\n" % rstrip(records[0]))
			if len(records) > 1:
				records = records[1:]
			else:
				records = []        
		for record in records:
			OUTDEBUG.write(rstrip(record)+"\n")
	
def stripBlanklines(record):
    """
    Strip away leading and trailng blank lines.
    """
    m = leadingblanks.match(record) # Important: Use match not search!
    if m:
        record = record[m.end():]
    return rstrip(record)
#---------------------------------------------------------------------------
# Use (homegrown) CPP to clean-up header before parsing
#---------------------------------------------------------------------------
cpp_namespace = '^ *namespace +[a-zA-Z]+\s*{'
cpp_tclassname = '^ *template +<[^>]+>\s*class +\w+\s*\w*\s*[^{;]+{'
cpp_classname = '^ *class +\w+\s*\w*\s*[^{;]+{'

cpp_tstructname = '^ *template +<[^>]+>\s*struct +\w+\s*\w*\s*[^{;]+{'
cpp_structname = '^ *struct +\w+\s*\w*\s*[^{;]+{'

cpp_leftbrace = '{'
cpp_rightbrace = '};|}'

cpp_stripbodies = re.compile('(?<={|})\s*{\s*};?', re.M)

cpp_regex = joinfields([cpp_namespace,
						cpp_tclassname,
						cpp_classname,
						cpp_tstructname,
						cpp_structname,
						cpp_leftbrace,
						cpp_rightbrace],'|')
cpp_search= re.compile(cpp_regex, re.M)

# Find different comment styles

# C++-style
scomment2    = '(?P<scomment2>(^[ \t]*///(?!/)[^\n]*\n))'
scomment3    = '(?P<scomment3>(^[ \t]*//(?!/)[^\n]*\n)+)'

# C-style
ccomment     = '(?P<ccomment>^[ \t]*/[*].+?(?=[*]/)[*]/(?! \)))'

# Doxygen-style
ocomment     = '(?P<ocomment>^[ \t]*///[^\n]+?\n[ \t]*/[*][*].+?(?=[*]/)[*]/)'

groups = (ocomment,scomment3,scomment2,ccomment)
format = (len(groups)-1)*'%s|'+'%s'
cpp_stripcomments = re.compile(format % groups,re.M+re.S)
cpp_stripinlinecomments = re.compile('//.*\n|/\*\*.*\*/\n', re.M)
cpp_stripbodies   = re.compile('(?<={|})\s*{\s*};?', re.M)
cpp_stripincludes = re.compile('^#include .*\s*', re.M)
cpp_stripstrings  = re.compile('"[^"]+"\s*', re.M)

cpp_findweird   = re.compile('(class|struct) +(?P<weird>\w+\s+)\w+')

def cpp(record, items):
	record = cpp_stripcomments.sub("", record)
	record = replace(record, "\\\"","")
	record = cpp_stripinlinecomments.sub("", record)
	record = cpp_stripincludes.sub("",record)
	record = cpp_stripstrings.sub("", record)
	results = map(lambda x: strip(replace(x,'\n',' ')),
				  cpp_search.findall(record))
	record = ''
	col = 0
	previous = ''
	for result in results:
		if find(result, 'class') > -1 or find(result, 'struct') > -1:
			m = cpp_findweird.search(result)
			if m != None:
				weird = m.group('weird')
				result = replace(result, weird, '')
						
		if find(result, '{') > -1:
			if previous == '{':
				col += 1
			previous = '{'
		elif find(result, '}') > -1:
			if previous == '}':
				col -= 1
			previous = "}"
		tab = '  '*col
		record += "%s%s\n" % (tab, result)
	newrecord = ''
	count = 0
	while (newrecord != record) and (count < 10):
		if newrecord != '': record = newrecord
		newrecord = cpp_stripbodies.sub("", record)
		count += 1
	record = newrecord
	return record
#---------------------------------------------------------------------------
# Class to write out stuff in XML...kinda obvious huh!
#---------------------------------------------------------------------------
def namespaceName(record):
	name = splitfields(record,'{',1)[0]
	name = strip(replace(name,'namespace',''))
	return name
#---------------------------------------------------------------------------
# Splice together a list of strings at specified locations within a record
#---------------------------------------------------------------------------
def splice(record, strlist, newline='\n'):
	records = []
	ii = 0
	for str, start, end in strlist:
		jj = start
		if jj >= 0:
			s  = record[ii:jj]
			records.append(s)
		records.append('%s%s' % (str,newline)) # Important for header2xml!
		ii = end        
	if ii < len(record):
		s  = record[ii:]
		records.append(s)
	return joinfields(records,"")
#---------------------------------------------------------------------------
#---------------------------------------------------------------------------
# This is the most important routine. If it breaks, nothing will work!
# It is used to find constructs of the form
#
#      <Identifier ...> <leftDelim> .... <rightDelim>
#---------------------------------------------------------------------------
#---------------------------------------------------------------------------
def findComponents(regex, record, leftDelim, rightDelim,
				   same=1, leftCount=0, rightCount=0):
	if same:
		groups = findAllSame(regex, record)
	else:
		groups = findAll(regex, record)

	clist = []
	for str, group, start, end in groups:
		count = 0
		iii = start
		nr  = len(record)
		n = 0
		first = 1
		while iii < nr:
			n += 1
			if n > MAXCONSTRUCT:
				str = "\tcan't find the end of construct .. go boil your head!"
				#print str
				raise ImConfused, str

			# Search for nearest left or right delimeter
			l = find(record[iii:],leftDelim)
			r = find(record[iii:],rightDelim)
			if l < 0: l = nr
			if r < 0: r = nr
			k = min(l,r)
			iii = iii + k + 1

			if k < nr:
				if k == l:
					count += 1
					if first:
						first = 0
						left  = iii-1 # Location of leftmost delimeter

				if k == r:
					count -= 1
					right = iii       # Location of rightmost delimeter

				if count == 0:
					start = max(start - leftCount, 0)
					end   = min(iii + rightCount, nr)
					clist.append((record[start:end],group,
								  start,left,right,end))
					break
			else:
				message = "\tboundary mismatch ..I'm bailing\n"
				print message, "\t",str
				raise ImConfused, message

	return clist

def findClasses(record):
	return findComponents(beginclasses,record,'{','}',1,0,1)

def namespaceNames(record):
	return findComponents(namespaces,record,'{','}')
#---------------------------------------------------------------------------
bodies      = re.compile('(?P<body>\{)',re.M)
def findBodies(record):
	return findComponents(bodies,record,'{','}',0)

def parseFunctionBodies(record, items):
	bodybnds = findBodies(record)
	skip = len(bodybnds)*[0]
	for i, (str, group, start, left, right, end) in enumerate(bodybnds):
		for j, (s, g, a, l, r, b) in enumerate(bodybnds):
			if (start < a) and (b < end):
				skip[j] = 1
	tokens = []
	for i, (str, group, start, left, right, end) in enumerate(bodybnds):
		if skip[i]: continue
		tokens.append(('{}',start,end))
		#debug(2,[str,78*'-'],0)
	if len(tokens) > 0:
		record = splice(record, tokens)
	return record
#---------------------------------------------------------------------------
# Find all strings that satisfy given regular expression.
#---------------------------------------------------------------------------
def findAll(regex,s):
	iterator = regex.finditer(s)
	strlist = []
	for m in iterator:
		keylist = []
		for key in m.groupdict().keys():
			if m.groupdict()[key] <> None:
				keylist.append(key)
				if len(keylist) > 1:
					fatal("double match \n%s" % keylist)
				record = rstrip(m.group())
				record = stripBlanklines(record) 
				strlist.append((record,key,m.start(),m.end()))
	return strlist

def findAllSame(regex,s):
	iterator = regex.finditer(s)
	strlist = []
	for m in iterator:
		s = (rstrip(m.group()),m.groupdict().keys()[0],m.start(),m.end())
		strlist.append(s)
	return strlist
#===========================================================================
# Parse header and try to identify fully scoped class names
#===========================================================================
def parseHeader(file):
	record = strip(open(file).read())
	# ---------------------------------------------------
	# Clean up with CPP
	# ---------------------------------------------------
	items = {}
	record = cpp(record, items)

	# ---------------------------------------------------
	# Find namespace preambles and ends and replace them.
	# Since these could be nested we need to order the
	# identified constructs before replacing them in
	# record
	# ---------------------------------------------------
	try:
		record = parseNamespaceBoundaries(record, items)
	except:
		return ('',{})
	
	# ---------------------------------------------------    
	# Find class preambles and ends and replace with
	# placeholders. Like namespaces, make sure we sort
	# them in case we have nested classes.
	# ---------------------------------------------------    
	oldrecord = record
	try:
		record = parseClassBoundaries(record, items)
	except:
		return ('', {})
	if record == '': record = oldrecord
	return (record, items)
#---------------------------------------------------------------------------
#------------------------------------ FUNCTIONS USED BY parseHeader --------
#---------------------------------------------------------------------------
def placeHolder(str, n):
	return "\%s%3.3d" % (str,n)
#--------------------------------------------------------
# Apply an OR of the regular expressions to the record
# and return all groups that match in the order in which
# they match.
#--------------------------------------------------------
def splitHeader(record):
	# First find placeholders then look for remaining groups
	# Search for "member" last
	exp = ""
	for item in PLACEHOLDERS:
		exp = exp + '(?P<%s>\\\%s[0-9]{3})|' % (item,item)
	regex  = re.compile(exp,re.M+re.S)
	return findAll(regex, record)

# Compare function for ordering nested constructs

def groupcmp(x, y):
	a = x[2]
	b = y[2]
	if a < b:
		return -1
	elif a > b:
		return 1
	else:
		return 0
#--------------------------------------------------------
# Find namespace preambles and ends and replace them.
# Since these could be nested we need to order the
# identified constructs before replacing them.
#--------------------------------------------------------        
def parseNamespaceBoundaries(record, items):
	debug(1,['Find Namespace Boundaries'],0)
	namespbnds = namespaceNames(record)
	namespacebnds = []
	tokens = []
	for i, (str, group, start, left, right, end) in enumerate(namespbnds):
		endpreamble = left + 1
		endbody     = right- 1
		preamble    = stripBlanklines(rstrip(record[start:endpreamble]))
		finale      = record[endbody:end]
		namespacebnds.append((preamble,'namespace',start,endpreamble))
		namespacebnds.append((finale,'endnamespace',endbody,end))

	debug(1,['Namespace Boundaries'])        
	namespacebnds.sort(groupcmp)
	tokens = []
	for i, (str, group, start, end) in enumerate(namespacebnds):
		debug(3,[str, 78*'-'],0)        
		placeholder = placeHolder(group, i)
		if group == 'endnamespace': placeholder = '\n%s' % placeholder
		items[placeholder] = str        
		tokens.append((placeholder,start,end))
	if len(tokens) > 0:
		record = splice(record, tokens)
		debug(3,['After Replace Namespace Boundaries',rstrip(record)])
	return record
#--------------------------------------------------------
# Find class preambles and ends and replace with
# placeholders. Like namespaces, make sure we sort
# them in case we have nested classes.
#--------------------------------------------------------
def parseClassBoundaries(record, items):
	debug(1,['Find Class Boundaries'],0)
	classbnds = findClasses(record)
	if len(classbnds) == 0:
		debug(2,['NO CLASSES FOUND',rstrip(record)])
		return ''
	debug(1,['Class Boundaries'])

	classbounds = []
	for i,(str, group, start, left, right, end) in enumerate(classbnds):
		endpreamble = left +1
		endbody     = right-1
		preamble    = stripBlanklines(rstrip(record[start:endpreamble]))
		preamble    = findInlineComments.sub("", preamble)
		finale      = record[endbody:end]

		m = classtype.search(preamble)
		if m == None:
			fatal("unable to get class type\n%s" % str)
		ctype = m.group("classtype")            
		if ctype == "class":
			group = "class"
		else:
			group = "structclass"
		classbounds.append((preamble,group,start,endpreamble))
		classbounds.append((finale,'end%s' % group,endbody,end))

	tokens = []
	classbounds.sort(groupcmp)
	for i,(str, group, start, end) in enumerate(classbounds):        
		debug(2,[str,78*'-'],0)
		placeholder = placeHolder(group, i)
		if group[0:3] == 'end': placeholder = '\n'+ placeholder #NB: \n!
		items[placeholder] = str
		tokens.append((placeholder,start, end))        
	if len(tokens) > 0:
		record = splice(record, tokens)        
	return record
#---------------------------------------------------------------------------
# Extract class type, name and possible base classes
#---------------------------------------------------------------------------
def getClassname(record):
	m = classtitle.search(record)
	if m == None:
		fatal("classtitle regex FAILED on line\n%s\n" % record)

	template = m.group("template")
	if template <> None:
		template = joinfields(split(strip(template)))

	title = strip(m.group("classtitle"))

	m = basenames.search(title)
	if m <> None:
		bname = strip(strip(m.group("basenames"))[1:])
		bname = replace(bname,"public ","%spublic " % WEIRD)
		bname = replace(bname,"protected ","%sprotected " % WEIRD)
		bname = replace(bname,"private ","%sprivate " % WEIRD)
		bname = split(bname,WEIRD)[1:]
		for i, name in enumerate(bname):
			name = strip(name)
			if name[-1] == ',':
				name = name[:-1]
			bname[i] = name
		cname = strip(title[:m.start()])
	else:
		bname = []
		cname = strip(title[:len(title)-1])

	return (cname,bname,template)
#============================================================================
# Main Program
#============================================================================
def main():
	
	print "mkclassmap.py $Revision: 1.2 $\n"

	subpackagelist = SUBPACKAGELIST
	filelist = []
	if Update:
		for subpackage in subpackagelist:
			print "scan sub-package: %s" % subpackage
			file = "%s/interface/*.h" % subpackage
			hlist = glob(file)
			hlist.sort()
			filelist += hlist
	else:

		for subpackage in subpackagelist:
			package, t = split(subpackage,'/')
			
			if subpackage[-1] == "*":
				cmd = "%s/%s" % (PKGBASE, subpackage)
				subsystems = filter(lambda x: os.path.isdir(x), glob(cmd))
				subsystems = map(lambda x: split(x, '/').pop(), subsystems)
			else:
				subsystems = [split(subpackage, '/').pop()]

			for subsystem in subsystems:
				dirpath = "%s/%s/%s" % (PKGBASE, package, subsystem)
				if skipsubsystem.match(subsystem) != None: continue

				print "scan sub-package: %s/%s" % (package, subsystem)
				file = "%s/interface/*.h" % dirpath
				hlist = glob(file)
				hlist.sort()
				filelist += hlist

		# Add user.h

		file = "%s/%s/%s/interface/user.h" % \
			   (LOCALBASE,
				"PhysicsTools",
				"Mkntuple")
		filelist.append(file)

	# Filter headers

	filelist = filter(lambda x: skipheader.search(x) == None, filelist)
	
	#-------------------------------------------------
	# Loop over header files to be scanned
	#-------------------------------------------------
	cmap = {}
	count = 0
	for index, file in enumerate(filelist):
		if not os.path.exists(file):
			print "** file %s not found" % file
			continue
		
		file = os.path.abspath(file)
		
		# Scan header and parse it for classes
		
		record, items = parseHeader(file)
		if record == '': continue
		records = splitHeader(record)
		if len(records) == 0: continue

		# Now strip away path up to "/src/" in pathname of header
		
		header = file
		k = rfind(header, "/src/") # search from right
		if k > 0: header = header[k+5:]
		
		names = []
		for irecord, (record, group, start, end) in enumerate(records):

			# Get actual record from items map

			key = strip(record)
			if items.has_key(key):
				record = items[key]
				if type(record) == type(()):
					record, extraRecord = record
			record = stripBlanklines(record)
			
			if group == "namespace":
				name = strip(namespaceName(record))
				if name != '': names.append(name)

			elif group == "endnamespace":
				if len(names) > 0: names.pop()
				
			elif group in ["endclass", "endstructclass"]:
				
				fullname = joinfields(names, "::")

				# Check for uninstantiated templates

				if find(fullname, '<') > -1:
					tplate = True
					key = split(fullname, '<')[0]
				else:
					tplate = False
					key = fullname
					
				if Update:
					ClassToHeaderMap[key] = header
					ClassToHeaderMap[fullname] = header
				else:
					cmap[key] = header
					cmap[fullname] = header

				count += 1
				print "%5d\t%s" % (count, fullname)
								
				names.pop()
				
			elif group in ["class", "structclass"]:
				classname, basenames, template = getClassname(record)
				names.append(classname)

	# Write out class to header map

	recs = []
	if Update:
		print "updating classmap.py..."
		keys = ClassToHeaderMap.keys()
		keys.sort()
		for fullname in keys:
			recs.append("'%s': '%s'" % (fullname, ClassToHeaderMap[fullname]))
	else:
		print "creating classmap.py..."
		keys = cmap.keys()
		keys.sort()
		recs = []
		for fullname in keys:
			recs.append("'%s': '%s'" % (fullname, cmap[fullname]))		

	record = joinfields(recs,',\n')		
	outfile = CLASSMAPFILE
	out = open(outfile,'w')
	out.write('# Created: %s\n' % ctime(time()))
	out.write('# Version: %s\n' % VERSION)
	out.write('#$Revision: 1.2 $\n')
	out.write("ClassToHeaderMap = {\\\n")
	out.write(record+'\n')
	out.write("}\n")
#---------------------------------------------------------------------------
main()










