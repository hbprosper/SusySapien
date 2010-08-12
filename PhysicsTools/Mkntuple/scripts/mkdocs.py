#!/usr/bin/env python
#---------------------------------------------------------------------------
# File:        mkdocs.py
# Description: Create html and txt files giving an exhaustive listing of
#              accessor methods
# Created:     23-Jan-2010 Harrison B. Prosper
# Updated:     15-Feb-2010 HBP, make it possible to be run anywhere
#              09-Mar-2010 HBP, add search of SimDataFormats
#              08-Aug-2010 HBP, fix search of user.h in Mkntuple
#$Revision: 1.5 $
#---------------------------------------------------------------------------
import os, sys, re
from ROOT import *
from string import *
from time import *
from getopt import getopt
#---------------------------------------------------------------------------
# Constants
#---------------------------------------------------------------------------
HEADERFILELIST = "headers"
PACKAGELIST="packagelist"
MAXCONSTRUCT=5000
USAGE='''
Usage:
  mkdocs.py [options] [header1 [header2..]]

            -I<project-area> [default = $CMSSW_RELEASE_BASE/src]
			
            If a header is not specified, mkdocs.py searches for the
			file headers and loops over the headers listed
			therein. If that is not found, then mkdocs.py looks
			for packagelist and scans the listed packages for
			headers.
'''
def usage():
	print USAGE
	sys.exit(0)
	
CURDIR = os.path.basename(os.environ["PWD"])
	
if os.environ.has_key("CMSSW_RELEASE_BASE"):
	BASE = "%s/src/"  % os.environ["CMSSW_RELEASE_BASE"]
else:
	BASE = ""

if os.environ.has_key("CMSSW_VERSION"):
	VERSION = os.environ["CMSSW_VERSION"]
else:
	VERSION = "unknown"

CLASSMAPFILE = 'map/classmap.py'

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

SHORTOPTIONS = 'hI:'

# Load needed libraries
from PhysicsTools.LiteAnalysis.AutoLoader import *
gSystem.Load("libPhysicsToolsMkntuple")

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
						   'CLHEP|'\
						   'Common|'\
						   'FWLite|'\
						   'Geometry|'\
						   'Histograms|'\
						   'Math|'\
						   'Provenance|'\
						   'Road|'\
						   'StdDict|'\
						   'Streamer|'\
						   'TestObj|'\
						   'VZero|'\
						   'Wrapped')

skipheader = re.compile('(classes|Fwd|print).h$')

leadingblanks= re.compile('(?P<leadingblanks>(^[ \t]*\n)+)',re.M)

fixresop     = re.compile('(?<=\w) *:: *(?=\w)',re.M)

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

# To extract template parameters

templatepar = re.compile('\w[^<]*<(?P<par>[^>]*)>')

# Return string containing base classes

basenames = re.compile('(?P<basenames>' \
					   '\s*(?<!:):(?!:)\s*(public|private|protected)?[^\{]+)')

forward      = '(?P<forward>^([ \t]*template\s*<[^\n]*>\s*)?\s*' \
			   'class[ \t]+[a-zA-Z_]\w*[ \t]*;[ \t]*\n)'

forwardname  = '^([ \t]*template\s*<[^\n]*>\s*)?\s*' \
			   'class[ \t]+(?P<name>\w+)[ \t]*;[ \t]*'

findInlineComments      = re.compile('[ \t]*//[^\n]*|[ \t]*/[*].*(?=\*/)\*/')
findEndClassSpace       = re.compile("\}\s*;",re.M)
findQualSpace           = re.compile("(?<=\))\s*(?=[a-zA-Z])",re.M)
findSemiColon           = re.compile('(?P<end>;(?![ \t]*(/[*]|//)))', re.M)
findForwards            = re.compile(forward,re.M)
getclassname= re.compile(r"\bclass[ \t]+(?P<name>\w+)[ \t]*;",re.M)

# Create command to scrub header of constructs that could cause mkclassmap
# to fail
SCRUB = ['ClassDef',
		 'Q_OBJECT',
		 'R__EXTERN']
STRIP = map(lambda x: "\\b%s\\b" % x, SCRUB)
cmd = '^\s*using [^\n]+;|' + joinfields(STRIP,"|") + '[^\n]*\n'
scrub = re.compile(cmd, re.M)
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

def forwardName(record):
	m = findForwardname.search(record)
	if m <> None:
		return m.group()
	else:
		return ""
	
def stripBlanklines(record):
    """
    Strip away leading and trailng blank lines.
    """
    m = leadingblanks.match(record) # Important: Use match not search!
    if m:
        record = record[m.end():]
    return rstrip(record)
#---------------------------------------------------------------------------
# Use CPP to clean-up header before parsing
#---------------------------------------------------------------------------
def cpp(record, items):
	debug(1,['CPP'])

	open('.header.h','w').write(record+'\n')
	# No linenumbers
	# c++
	# Don't search system headers
	# Discard comments
	command = "(cpp -P -xc++ -nostdinc -nostdinc++" \
			  " .header.h > .cpp) >& .log"
	os.system(command)
	record = open(".cpp").read()
	# ---------------------------------------------------    
	# Get rid of some weird affectations
	# ---------------------------------------------------    
	record = replace(record," ;", ";")
	record = replace(record,"( ", "(")
	record = replace(record," )", ")")
	record = replace(record,"public :",   "public:")
	record = replace(record,"protected :","protected:")
	record = replace(record,"private :",  "private:")
	record = replace(record,"public:",   "public:\n")
	record = replace(record,"protected:","protected:\n")
	record = replace(record,"private:",  "private:\n")
	record = fixresop.sub("::", record)
	record = findEndClassSpace.sub("};",record)
	record = findQualSpace.sub(" ",record)
	record = findSemiColon.sub(";\n",record)
	record = scrub.sub("", record)
	
	# Add a \n after "{". Should be harmless, but easy way to take
	# care of things like: namespace { class YaaHoo; }
	record = replace(record,'{','{\n')
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
				print str
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
	# Find forward declarations and replace them
	# in record
	# ---------------------------------------------------
	record = parseForwards(record, items)

	# ---------------------------------------------------
	# Find namespace preambles and ends and replace them.
	# Since these could be nested we need to order the
	# identified constructs before replacing them in
	# record
	# ---------------------------------------------------    
	record = parseNamespaceBoundaries(record, items)

	# ---------------------------------------------------    
	# Find class preambles and ends and replace with
	# placeholders. Like namespaces, make sure we sort
	# them in case we have nested classes.
	# ---------------------------------------------------    
	oldrecord = record
	record = parseClassBoundaries(record, items)
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
def parseForwards(record, items):
	allforwards = findAll(findForwards, record)
	tokens = []
	for i, (str, group, start, end) in enumerate(allforwards):
		str = stripBlanklines(rstrip(str))
		placeholder = placeHolder("forward", i)
		items[placeholder] = strip(str)
		tokens.append((placeholder,start,end))
	if len(tokens) > 0:            
		record = splice(record, tokens)
	return record
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
#----------------------------------------------------------------------------
# Code to extract methods etc.
#----------------------------------------------------------------------------
FINAL   = 1
SCOPED  = 4
BLACK     = '"black"'
DARKRED   = '"darkred"'
DARKGREEN = '"darkgreen"'
DARKBLUE  = '"darkblue"'
BLUE      = '"blue"'

stripcolon = re.compile(r':')
striptemplatepars = re.compile(r'\<.*\>')
skipmethod = re.compile(r'TClass|TBuffer|TMember|operator|^__')
ref = re.compile(r'edm::Ref\<std::vector\<.+?(?=,)')
reftype = re.compile(r'(?<=edm::Ref\<std::vector\<)(?P<name>.+?)(?=\>,)')
basicstr = re.compile(r'std::basic_string\<char\>')
vsqueeze = re.compile(r'(?<=[^>]) +\>')
#----------------------------------------------------------------------------
def classMethods(classname, db, depth=0):
	depth += 1
	if depth > 20:
		print "lost in trees"
		return
	tab = "  " * (depth-1)

	##D
	#print "%s( %s )" % (tab, classname)

	cdb = {'classname': classname,
		   'methods': []}
	
	thing = Reflex.Type()
	c = thing.ByName(classname)
	n = c.FunctionMemberSize()
	##D
	#print "\tnumber of methods: %d" % n
	
	for i in xrange(n):
		m = c.FunctionMemberAt(i)
		if not m.IsPublic(): continue
		if not m.IsFunctionMember(): continue
		if m.IsConstructor(): continue
		if m.IsDestructor():  continue

		name  = m.Name()		
		mtype = m.TypeOf().Name(SCOPED)
		
		rtype, args = split(mtype, '(')
		args  = replace('(%s' % args, '(void)', '()')

		# In C++ there is no overloading across scopes
		# only within scopes
		if db['scopes'].has_key(name):
			# This method is potentially an overload.
			# If we are not in the same scope, however, it cannot
			# overload the existing method
			scope = db['scopes'][name]
			if  scope != classname: continue
		db['scopes'][name] = classname

		###D
		#print "SCOPE(%s) METHOD(%s)" % (classname, name)
		
		signature = name + args
		
		# Skip setters
		rtype = strip(rtype)
		if rtype in ['void', 'void*']: continue

		# Expand typedefs, but check first for pointers and
		# references
		fullrtype = rtype
		if rtype[-1] in ['*','&']:
			r = thing.ByName(rtype[:-1])
			if r.IsTypedef():
				fullrtype = "%s%s" % (r.Name(SCOPED+FINAL), rtype[-1])
				rtype = fullrtype # Fri Jan 29
		else:
			r = thing.ByName(rtype)
			if r.IsTypedef():
				fullrtype = r.Name(SCOPED+FINAL)			
		##D
		#print "RTYPE(%s)\n\tFULLTYPE(%s)" % (rtype, fullrtype)
		
		rtype     = strip(basicstr.sub("std::string", rtype))
		fullrtype = strip(basicstr.sub("std::string", fullrtype))
		signature = basicstr.sub("std::string", signature)
		str = "%s  %s" % (rtype, signature)
		if skipmethod.search(str) != None: continue
		
		m = reftype.findall(str)
		if len(m) != 0:
			for x in m:
				cname = "%sRef" % x
				cmd = re.compile(r"edm::Ref\<.+?,%s\> \>" % x)
				rtype = cmd.sub(cname, rtype)
				signature = cmd.sub(cname, signature)

		# Ok, now added to methods list
		rtype = vsqueeze.sub(">", rtype)
		signature = vsqueeze.sub(">", signature)
		method    = "%32s  %s" % (rtype, signature)

		##D
		#print "METHOD(%s)" % method
		# Important: make sure we don't have duplicates
		if db['methods'].has_key(method):
			###D
			#print "\t*** DUPLICATE METHOD(%s)" % method
			continue
		db['methods'][method] = classname
		
		##D
		#print "%smethod( %s )" % (tab, method)
		cdb['methods'].append((fullrtype, method))

	db['classlist'].append( cdb )
	
	nb = c.BaseSize()
	for i in xrange(nb):
		b = c.BaseAt(i).ToType()
		basename = b.Name(SCOPED)
		db['baseclassnames'].append(basename)
		classMethods(basename, db, depth)
#----------------------------------------------------------------------------
def convert2html(record):
	record = replace(record,'&','&amp;')
	record = replace(record,'<','&lt;')
	record = replace(record,'>','&gt;')
	record = replace(record,'~','&atilde;')
	record = replace(record,"\t","&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;")
	record = replace(record,"\n","<br>")
	record = replace(record," ","&nbsp;")
	return record
#----------------------------------------------------------------------------
def color(c,s):
	return '<font color=%s>%s</font>' % (c,s)
#----------------------------------------------------------------------------
def boldcolor(c,s):
	return '<b><font color=%s>%s</font></b>' % (c,s)
#-----------------------------------------------------------------------------
def printMethods(db, out):
	classlist = db['classlist']
	for cdb in classlist:		
		cname   = cdb['classname']
		methods = cdb['methods']
		out.write('\nAccessMethods - %s\n' % cname)
		for fullrtype, method in methods:
			out.write("  %s\n" % method)
#-----------------------------------------------------------------------------
def printHeader(db, out):
	cname    = db['classname']
	bname    = db['baseclassnames']
	header   = db['header']
	filestem = db['filestem']
	
	if cname == '':
		print "\t** %s not processed!" % header
		out.close()
		os.system("rm -rf txt/%s*" % filestem)
		sys.exit(0)

	# Write class name and its header file

	tab = '             '
	out.write('Class:       %s\n\n'  % cname)
	out.write('Header:      %s\n\n'  % header)

	# Write base class names
	
	if len(bname) > 1:
		delim = ''
		out.write('BaseClasses: ')
		for i, fullname in enumerate(bname):
			out.write('%s%s\n' % (delim, fullname))
			delim = tab
			
		out.write("\n")
	out.write('Version:     %s\n' % db['version'])
	out.write('Created:     %s\tmkdocs.py\n' % ctime(time()))
#-----------------------------------------------------------------------------
def writeHTML(db, txtfilename):

	fullname = db['classname']
	filestem = db['filestem']

	record = open(txtfilename).read()
	record = convert2html(record)	
	cname  = split(split(fullname, '<')[0],'::').pop()

 	record = replace(record, 'Class:',
					 boldcolor(BLUE, 'Class:'))

 	record = replace(record, 'Header:',
					 boldcolor(BLUE, 'Header:'))
	
  	record = replace(record, 'BaseClasses:',
 					 boldcolor(BLUE, 'BaseClasses:'))

  	record = replace(record, 'AccessMethods',
 					 boldcolor(DARKBLUE, 'AccessMethods'))

	htmlfile = "html/%s.%s.html" % (filestem, cname)
	out = open(htmlfile, 'w')
	out.write('<code>\n')
	out.write('\t<font size=+1 color=%s>\n' % BLACK)
	out.write(record)
	out.write('\t</font>\n')
	out.write('</code>\n')
	out.close()
#============================================================================
# Main Program
#  Read header list
#  for each header in list
#     decode header
#============================================================================
def main():
	
	print "\n\tmkdocs.py\n"
	global BASE

	# decode command line
	
	argv = sys.argv[1:]
	opts, args = getopt(argv, SHORTOPTIONS)
	outfile = CLASSMAPFILE

	for option, value in opts:

		if   option == "-h":
			usage()

		elif option == "-I":
			BASE = value

	if len(args) > 0:
		filelist = args
	else:
		if os.path.exists(HEADERFILELIST):
			filelist = map(strip, open(HEADERFILELIST))
			
		else:

			filelist = []
			for package in ["DataFormats", "SimDataFormats"]:
				
				cmd = "ls -1 $CMSSW_RELEASE_BASE/src/%s/" % package
				
				subsystems = map(strip, os.popen(cmd).readlines())

				for subsystem in subsystems:
					if skipsubsystem.match(subsystem) != None: continue

					file = "%s%s/%s/interface/*.h" % (BASE, package, subsystem)
					cmd  = "find %s" % file
					print "\tscan package: %s/%s" % (package, subsystem)
					hlist = map(strip, os.popen(cmd).readlines())
					filelist += hlist

			# Add user.h

			file = "%s/src/%s/%s/interface/user.h" % \
				   (os.environ["CMSSW_BASE"],
					"PhysicsTools",
					"Mkntuple")
			filelist.append(file)

	# Filter headers

	filelist = filter(lambda x: skipheader.search(x) == None, filelist)
	
	#-------------------------------------------------
	# Loop over header files to be scanned
	#-------------------------------------------------

	print "mkdocs.py $Revision: 1.5 $\n"

	# Make sure html and txt directories exist
	
	os.system("mkdir -p html; mkdir -p txt")
	
	recs = []
	count = 0
	for index, file in enumerate(filelist):

		# Create full pathname to header
		
		if file[0:8] in ["DataForm", "SimDataF"]: file = BASE + file
		if not os.path.exists(file):
			print "** file %s not found" % file
			continue
		file = os.path.abspath(file)
		
		# Scan header and parse it for classes
		
		record, items = parseHeader(file)
		records = splitHeader(record)
		if len(records) == 0: continue

		# Now strip away path up to "/src/" in pathname of header
		
		header = file
		k = rfind(header, "/src/") # search from right
		if k > 0: header = header[k+5:]
	
		filestem = replace(header, 'interface/', '')
		filestem = split(filestem, '.h')[0]
		filestem = replace(filestem, '/', '.')

		###D
		#print "filestem(%s) header(%s)" % (filestem, header)
		
		# Initialize map to contain info about classes and methods
		
		db = {'version':  VERSION,
			  'filestem': filestem,
			  'header':   header}
		
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
				recs.append("'%s': '%s'" % (fullname, header))

				# For now ignore uninstantiated templates
				
				if find(fullname, '<') > -1:
					print "\t\t** skip class %s\n" % fullname 
					continue

				count += 1
				print "%5d\t%s" % (count, fullname)
				
				# Get methods and write them out

				db['scopes'] = {}
				db['methods'] = {}
				db['classname'] = fullname
				db['classlist'] = []
				db['baseclassnames'] = []
				db['signature'] = {}
				classMethods(fullname, db)

				#cname = striptemplatepars.sub("", fullname)
				cname  = split(fullname,'::').pop()
				methfilename = "txt/%s.%s.txt" % (filestem, cname)

				out = open(methfilename, 'w')
				printHeader(db, out)
				printMethods(db, out)
				out.close()

				writeHTML(db, methfilename)
				
				names.pop()
				
			elif group in ["class", "structclass"]:
				classname, basenames, template = getClassname(record)
				names.append(classname)

	# Write out a class to header map
	
	os.system("mkdir -p map")
	out = open(outfile,'w')
	if os.environ.has_key("CMSSW_VERSION"):
		out.write('# VERSION: %s\n' % os.environ['CMSSW_VERSION'])
	else:
		out.write('# VERSION: unknown\n')
	out.write("ClassToHeaderMap = {\\\n")
	record = joinfields(recs,',\n')
	out.write(record+'\n')
	out.write("}\n")

	# Create index.html
	
	os.system("mkindex.py")
	print "\n\tdone!\n"
#---------------------------------------------------------------------------
#---------------------------------------------------------------------------
main()










