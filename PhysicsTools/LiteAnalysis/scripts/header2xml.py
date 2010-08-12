#!/usr/bin/env python
#---------------------------------------------------------------------------
# File:        header2xml.py
#
# Description: Create XML files that describe the structure and contents of
#              C++ header files. We ignore protected and private sections.
#
#              As a target, make sure we can handle the CMS COBRA, ORCA
#              headers as well as Root, Coin and CLHEP.
#
#              The basic approach is to use heuristic rules encoded in
#              regular expressions to parse the headers. We find the easiest
#              constructs first, replace them with numbered placeholders then
#              continue with the next easiest and so on, until all that
#              remains are class members, which are picked up with a rather
#              loose rule.
#
#              Finally, we search the header for the placeholders and write
#              out the corresponding construct in XML. For an approach based
#              on a rigorous parsing of C++ headers the state-of-the-art is
#              gccxml, as used in pyste. Unlike the output of gccxml, the
#              output of header2xml is designed to be easily understood. To
#              get a non-html view of these files do
#
#                        xmlshow.py < <file.xml>
#
#---------------------------------------------------------------------------
# Created:     14-Oct-2003 Harrison B. Prosper, CMS Experiment, CERN
# Updated:     21-Oct-2003 HBP Handle templated classes using typedefs
#              22-Oct-2002 HBP Handle STL vectors with typedefs
#              23-Oct-2003 HBP Allow the loading of comments from file
#              03-Nov-2003 HBP Handle namespaces
#              19-Nov-2003 HBP Add cpp processing
#              27-Nov-2003 HBP Add more comments
#              05-Dec-2003 HBP Handle nested namespaces and classes
#              12-Mar-2004 HBP Fix problem with inline comments
#                              associated with arguments.
#              10-May-2004 HBP Some bug fixes. Add more documentation.
#              11-May-2004 HBP Clean up code
#              21-May-2004 HBP Handle explicit keyword
#              14-Sep-2004 HBP Handle inline constructors
#              20-Sep-2004 HBP Fix friend, seach until we find a ";"
#              28-Oct-2004 HBP Save variable names
#              06-Dec-2004 HBP (at CERN) Fix bug in basenames regex
#              22-Apr-2005 HBP
#              21-May-2005 HBP Fix typedef unfolding bug
#              16-Aug-2005 HBP Allow "qual" to be on next line!
#              17-Oct-2005 HBP Remove dependence on CMS_SYS
#              10-Feb-2006 HBP Fix templateDecoration (failed on MapVector!)
#              31-Mar-2006 HBP Fix decodeMethod (failed on EDProducer)
#              08-May-2006 HBP Add printable attribute to classes with
#                              operator<<
#              11-Dec-2009 HBP Fix problem with __attribute__(...) 
#$Revision: 1.3 $
#---------------------------------------------------------------------------
import os, sys, re, posixpath, shelve
from time   import time, ctime
from string import *
from glob   import glob
from getopt import getopt
from elementtree.ElementTree import ElementTree
from xml.parsers.expat import ExpatError
#---------------------------------------------------------------------------
from boostlib import *
#---------------------------------------------------------------------------
# Constants
#---------------------------------------------------------------------------
VERSION='$Revision: 1.3 $'
VERSION=split(VERSION[:-1]).pop()
HEADERSFILE = 'Headers'
MAXCONSTRUCT=50000 # Maximum number of constructs to search for

if not os.environ.has_key('PYTHON_PROJECTS'):
	print "\n\t** PYTHON_PROJECTS undefined ** choi!\n"
	sys.exit(1)

BASE = os.environ['PYTHON_PROJECTS']
sys.path.insert(0, "%s/python" %  BASE)

# Determine module etc.

def getProject():

## 	if not os.environ.has_key('PYTHON_PROJECTS'):
## 		print "\n\t** PYTHON_PROJECTS undefined ** choi!\n"
## 		sys.exit(1)

## 	BASE = os.environ['PYTHON_PROJECTS']
	PWD  = strip(os.popen('pwd').read())
	PROJECT = ''
	PACKAGE = ''
	MODULE  = ''
	PROJECTPATH = BASE
	XMLMAPFILE  = '%s/python/XmlMap.py' % BASE
	
	tmp  = split(replace(PWD, BASE, ''),'/')

	try:
		tmp.remove('src')
	except:
		pass

	if len(tmp) == 0:
		fatal("Wrong directory")

	tmp = tmp[:-1]
	if len(tmp) == 0:
		return (BASE, PWD, PROJECT, PACKAGE, MODULE, PROJECTPATH, XMLMAPFILE)

	PROJECT = tmp[0]
	MODULE  = joinfields(tmp[1:],'.')
	PROJECTPATH = BASE + PROJECT

	if len(tmp[1:]) > 0:
		PACKAGE = joinfields(tmp[1:-1],'.')
	else:
		PACKAGE = ''

	#print "PROJECTPATH", PROJECTPATH
	#print "PROJECT", PROJECT
	#print "PACKAGE", PACKAGE
	#print "MODULE", MODULE

	return (BASE, PWD, PROJECT, PACKAGE, MODULE, PROJECTPATH, XMLMAPFILE)
#---------------------------------------------------------------------------
BASE, PWD, PROJECT, PACKAGE, MODULE, PROJECTPATH, XMLMAPFILE = getProject()
EDITSFILE       = 'comments.py'
GLOBALEDITSFILE = '%s/config/%s' % (PROJECTPATH, EDITSFILE)
PROBLEMSFILE= 'CHECKME'
DEBUGFILE   = 'DEBUG.h'
XMLDIR      = '.'
FILEKEY     = 1
VERBOSE     = 0
SILENT      = 1
STANDALONE  = 1

SYNOPSIS=\
'''h2xml         [-Iinclude-path...]
				 [-binclude-base]
				 [-ddebug-level]\n
				 [-Dmacro=value...]
				 [-c]
				 [-n]=namespace[,namespace[,]..]
				 [-v] version
				 [-S]
				 [-C]   [infile ...]
				 [-s] standalone''' 
# For print statements

COLORS = {'black':  '\x1b[0;30;48m', # Ctrl[attribute;foreground;backgroundm
		  'red':    '\x1b[0;31;48m', # Ctrl[attribute;foreground;backgroundm
		  'green':  '\x1b[0;32;48m', # \x1b 0,1,...   30+color   40+color
		  'yellow': '\x1b[0;33;48m', # color=8 means leave color unchanged
		  'blue':   '\x1b[0;34;48m',
		  'magenta':'\x1b[0;35;48m',
		  'cyan':   '\x1b[0;36;48m',
		  'white':  '\x1b[0;37;48m'}

CONTEXT={'black':  COLORS['black'],
		 'red':    COLORS['red'],
		 'green':  COLORS['green'],
		 'yellow': COLORS['yellow'],
		 'blue':   COLORS['blue'],
		 'magenta':COLORS['magenta'],
		 'cyan':   COLORS['cyan'],
		 'white':  COLORS['white'],
		 'pwd':       PWD,
		 'xml':       XMLDIR,
		 'synopsis':  SYNOPSIS,
		 'time':      ctime(time()),
		 'VERSION':   VERSION,
		 'headersfile': HEADERSFILE,
		 'editsfile':   EDITSFILE}

USAGE='''
Usage:
%(synopsis)s
''' % CONTEXT

HELP = '''
%(blue)sNAME%(black)s
\theader2xml.py - convert a C++ header to a file in a simple XML format

%(blue)sSYNOPSIS%(black)s
\t%(synopsis)s

%(blue)sDESCRIPTION%(black)s
\tParse one or more C++ header files and write their contents to output 
\tfiles in XML format. The name of each XML file is the same as that
\tof the header but with extension .xml. By default, the program reads
\tthe list of header files to be processed from the files in the src
\tdirectory, each of which contains a header file name. Alternatively,
\tthe list of header files to be converted can be given on the command
\tline.

\tThe files created with header2xml.py can be read using the Python
\tmodule elementtree. For example, suppose the file test.xml contains
\tan XML data structure. It can be read as follows
%(blue)s
\t\tfrom elementtree.ElementTree import ElementTree
\t\troot = ElementTree(file="%(green)stest.xml%(blue)s").getroot()
%(black)s
\t\tNote: All xml files are created in the sub-directory xml,
\t\t      created relative to the working directory.

%(blue)sOPTIONS%(black)s
\t-I include-path
\t\tSpecify a search path for the header files.

\t-d debug-level
\t\tActivate debug printout. The larger the level the greater the
\t\tlevel of detail. Output goes to the file debug.txt.

\t-D macro=value
\t\tSpecify a macro to be passed to the C pre-processor.

\t-c 
\t\tPrintout class and method comments formatted as Python maps.

\t-C
\t\tCreate an XML file only if one does not exist.
''' % CONTEXT

SHORTOPTIONS = 'sphCcvVSI:d:D:n:r:p:'

TTYPE        = {}
MEMBERTYPE   = 0; TTYPE[MEMBERTYPE]  = 'member'
CLASSTYPE    = 1; TTYPE[CLASSTYPE]   = 'class'
ENUMTYPE     = 2; TTYPE[ENUMTYPE]    = 'enum'
TYPEDEFTYPE  = 3; TTYPE[TYPEDEFTYPE] = 'typedef'
FTYPEDEFTYPE = 4; TTYPE[FTYPEDEFTYPE]= 'ftypedef'

NAMESPACELIST=['std']

# Switches

DEBUG=0
writeOUTMETHODCOM=0
writeOUTCLASSCOM=0

# File descriptors

OUTDEBUG=0
OUTMETHODCOM=0
OUTCLASSCOM=0

# Items within class scope

MEMBER  = ['member',
		   'enum',
		   'struct',
		   'typedef',
		   'typedefstruct']

METHOD  = ['method1',
		   'method2',
		   'function1',
		   'function2',
		   'operator',
		   'destructor',
		   'constructor',
		   'copyconstructor',
		   'macro']

# Items outside class scope

OUTCLASS= ['enum',
		   'operator',
		   'method1',
		   'method2',
		   'struct',
		   'typedef',
		   'typedefstruct']

PLACEHOLDERS =['enum',
			   'constant',
			   'struct',
			   'typedef',
			   'typedefstruct',
			   'method1',
			   'method2',
			   'operator',
			   'destructor',
			   'constructor',
			   'namespace',
			   'endnamespace',
			   'include',
			   'forward',
			   'class',
			   'endclass',
			   'structclass',
			   'endstructclass',
			   'friend',
			   'inlineclass',
			   'inlinestructclass',
			   'comment']


STL_TYPES=['vector',
		   'list',
		   'pair',
		   'map',
		   'complex',
		   'deque',           
		   'fstream',
		   'ifstream',
		   'ofstream',
		   'hash_map',
		   'hash_set',
		   'istream',
		   'ostream',
		   'type_info',
		   'typeid',
		   'bitset',
		   'istream',
		   'ostream',
		   'stringstream',
		   'stack',
		   'string',
		   'stack',
		   'string',
		   'iterator']

# Stuff to zero out (NB: entire line!)

SCRUB = ['using namespace',
		 'ClassDef',
		 'Q_OBJECT',
		 'R__EXTERN']

FENDOFFSET = 12

# Add optional edit structures to Python global namespace

CLASSCOMMENTS={}
METHODCOMMENTS={}

if os.path.exists(GLOBALEDITSFILE):
	execfile(GLOBALEDITSFILE)

if os.path.exists(EDITSFILE):
	execfile(EDITSFILE)
#---------------------------------------------------------------------------
# Exceptions
#---------------------------------------------------------------------------
class ImConfused(Exception):
	def __init__(self, msg):
		self.msg = msg

	def __str__(self):
		return `self.value`
#===========================================================================
# Heuristic regular expressions to take apart C++ header
#
# Notes:
# 1. Use named groups (?P<groupname>....) so that we can retrieve matched
#    strings by group name.
#
# 2. Use lookaheads (?=...), (?!...) and lookbehinds (?<=...), (?<!...)
#    to simplify regexes and make matches more robust. Note: lookaheads and
#    lookbehinds as zero width matches so they do not consume characters.
#
# 3. Warning: The "member" regex is the least selective. It must therefore be
#    tried after all those that are more selective.
#
# 4. Occasionally, we use raw strings r'....' to make the escaping of
#    special characters less of a chore.
#===========================================================================

beginnamespace= '(?P<namespace>' \
			   '\\bnamespace\\b\s*(\w+)?\s*[^\{]*(?=\{)\{)'

endnamespace  = '(?P<endnamespace>\}(?!;))'

#templatecmd  = '(?P<template>template'\
#               '([^\n]*|\s*<\s*([^\n]+\s+)+([^\n]+)?\s*)>\s*)'

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

section      = '(?P<section>' \
			   '(^\s*private:(?=\s*))|' \
			   '(^\s*protected:(?=\s*))|' \
			   '(^\s*signals:(?=\s*))|' \
			   '(^\s*public:(?=\s*)))'

include      = '(?P<include>#\s*include\s+' \
			   '(<|")[\w/.]+(>|"))'

includename  = '#\s*include\s+' \
			   '(<|")(?P<name>[\w/.]+)(>|")'

typedefstruct= '(?P<typedefstruct>^\s*typedef\s+struct'   \
			   '(\s+\w+)?\s*'  \
			   '\{[^\}]+\}'    \
			   '(\s*\w+)?[ \t]*;)'

typedef      = '(?P<typedef>^\s*typedef[ \t]+[^;]+;)'

struct       = '(?P<struct>^[ \t]*struct'   \
			   '(\s+\w+)?\s*'   \
			   '\{[^\}]+\}'     \
			   '(\s*\w+)?[ \t]*;)'

enum         = '(?P<enum>^\s*enum\s*'  \
			   '(\s*\w+\s*)?'   \
			   '\{[^\}]+\}'     \
			   '(\s*\w+)?[ \t]*;)'

# Super loose definition of a constant
constant     = '(?P<constant>^\s*(static)?\s*const\s+'\
			   '(?!char[ ]?\*\*?)[^\n]+)'

constructor  = '(?P<constructor>^[ \t]*(inline[ \t]+)?(explicit[ \t]+)?'\
			   '[a-zA-Z_]\w*\s*(?=\())'

destructor   = '(?P<destructor>^[ \t]*(inline[ \t]+)?(virtual[ \t]+)?~[ ]*'\
			   '[a-zA-Z_]\w*\s*(?=\())'

operator     = '(?P<operator>operator[ \t]*(new|delete)?)'

# WARNING: This is a *very* heuristic definition of a method!

method       = '(?P<method>^[ \t]*' \
			   '[a-zA-Z_]\w*(::)?\w*(?=\s|\*|\&|<)' \
			   '[^\(\)\{\};#]+?\()'


function     = '%s|%s|%s' % (constructor,
							 destructor,
							 method)

forward      = '(?P<forward>^([ \t]*template\s*<[^\n]*>\s*)?\s*' \
			   'class[ \t]+[a-zA-Z_]\w*[ \t]*;[ \t]*\n)'

forwardname  = '^([ \t]*template\s*<[^\n]*>\s*)?\s*' \
			   'class[ \t]+(?P<name>\w+)[ \t]*;[ \t]*'

extern       = '(?P<extern>^[ \t]*extern[^\n]+\n)'

initlist     = '(?<=\))\s*:[^\{]*?(?=\{)'

# WARNING: and here is an even more heuristic definition of a member!

member       = '(?P<member>^[ \t]*' \
			   '[a-zA-Z_]\w*([^\(;]+?(?!\());[^\n]*)'

# Find different comment styles

# C++-style
scomment2    = '(?P<scomment2>(^[ \t]*///(?!/)[^\n]*\n))'
scomment3    = '(?P<scomment3>(^[ \t]*//(?!/)[^\n]*\n)+)'

# C-style
ccomment     = '(?P<ccomment>^[ \t]*/[*].+?(?=[*]/)[*]/(?! \)))'

# Doxygen-style
ocomment     = '(?P<ocomment>^[ \t]*///[^\n]+?\n[ \t]*/[*][*].+?(?=[*]/)[*]/)'


ifdef        = '(?P<ifdef>^#[ \t]*ifn?(def)?[^\n]*\n)'

elseif       = '(?P<elseif>^#[ \t]*else(if)?[^\n]*\n)'

endif        = '(?P<endif>^#[ \t]*endif[^\n]*\n)'

macrodef     = r'(?P<define>^#[ \t]*define[ \t]+'\
			   r'\w+\([\w, ]+\)[ \t]*\\[ \t]*\n'\
			   r'(^[^\\\n]+(\\[ \t]*)?\n)+)'

macrodefname = r'^#[ \t]*define[ \t]+'\
			   r'(?P<name>\w+)\('
#===========================================================================
# Compiled regular expressions
#===========================================================================

# Find #if... #define

define      = re.compile( '(?P<define>^#[ \t]*ifn?def[^\n]*\n' \
						  '^#[ \t]*define [^\n]*\n)', re.M)

# Regex to strip away initialization lists

fixinitlist = re.compile('(?<=\))\s*:[^\{]+?(?=\{)',re.M)

# Convert inline comments to following doxygen style: ///<

fixinline    = re.compile('//(?=[^\n]+)',re.M)
fixinline1   = re.compile('/[*](?=[^\n]+)',re.M)
fixinline2   = re.compile('[*]/(?=[^\n]*)',re.M)
fixinline1in = re.compile('/[*](?![^;]+;)',re.M)
fixinline2in = re.compile('[*]/(?![^;]*;)',re.M)

fixresop     = re.compile('(?<=\w) *:: *(?=\w)',re.M)

# Return class preambles (that is, classs <name>.... {)

beginclasses = re.compile(beginclass,re.M)

# Return template preambles

begintemplates = re.compile(begintemplate,re.M)

# Return namespace preambles (that is, namespace <name> {)

namespaces= re.compile(beginnamespace,re.M)

classtitle= re.compile('%s?'\
					   '\s*(class|struct)\s+' \
					   '(?P<classtitle>[a-zA-Z]\w*[^\{]*\{)' % \
					   templatecmd, re.M)

classtype = re.compile('(?P<classtype>\\b(class|struct)\\b)',re.M)

# Remove ";" at end of enum

enumtype    = re.compile(enum[0:-2]+')')

# To extract template parameters

templatepar = re.compile('\w[^<]*<(?P<par>[^>]*)>')

# Return string containing base classes

#basenames = re.compile('(?P<basenames>' \
#                       '\s*:(?!:)\s*(public|private|protected)?[^\{]+)')

basenames = re.compile('(?P<basenames>' \
					   '\s*(?<!:):(?!:)\s*(public|private|protected)?[^\{]+)')

# Return friends

friends   = re.compile('(?P<friend>^[ \t]*(inline)?[ \t]*friend\s+[^;]+;)',
					   re.M)

constants = re.compile(constant, re.M)

colonvalue= re.compile('[ ]*:(?!:)[ ]*[^;]+',re.M)

# Return function candidates

functions   = re.compile(function,re.M)
findctor    = re.compile(constructor,re.M)
constructors= re.compile("%s|%s"    % (constructor,destructor),re.M)
functionend = re.compile('(?P<funend>[^\n/\{;]*(?=\n|/|\{|;);?)',re.M)
bodies      = re.compile('(?P<body>\{)',re.M)
operatorPP  = re.compile('(?P<operator>operator[ \t]*\([ \t]*\))',re.M)
operatorPP  = re.compile('(?P<operator>operator[ \t]*'\
						 '(?P<name>new|delete)?[ \t]*\([ \t]*\))',re.M)
operatornewPP  = re.compile('(?P<operator>operator[ \t]*new[ \t]*\(\))',re.M)
operatorlongPP  = re.compile('(?P<operator>operator[ \t]*long[ \t]*\(\))',
							re.M)
operatordeletePP  = re.compile('(?P<operator>operator[ \t]*delete[ \t]*\(\))',
							re.M)
throwPP     = re.compile(r'(?P<throw>\bthrow\b[ \t]*\([ \t]*\))',re.M)

__attribute__ = re.compile(r'__attribute__\(.+\)',re.M)

# Find enums, structs etc.

groups  = (struct,enum,typedefstruct,typedef)
format  = (len(groups)-1)*'%s|'+'%s'
exp     = format % groups
enumetc = re.compile(exp,re.M+re.S)
members = re.compile(member,re.M)

# To handle ifdef...endif constructs

reif   = r'(?P<if>^#[ \t]*if[^\n\\]+' \
		 r'(\\[ \t]*\n[^\n\\]+)*\s+)'

reelif = r'(?P<elif>^#[ \t]*elif[^\n+\\]+' \
		 r'(\\[ \t]*\n[^\n\\]+)*\s+)'

reelse = r'(?P<else>^#[ \t]*else\s+)'

reendif= r'(?P<endif>^#[ \t]*endif\s+)'

allif  = '%s|%s|%s|%s' % (reif,reelif,reelse,reendif)

stripBlanks = re.compile('(?P<hash>^#[ \t]*)')

getif  = re.compile(allif,re.M)

# Find stuff

findInlineComments1     = re.compile('[ \t]*//[^\n]*')
findInlineComments      = re.compile('[ \t]*//[^\n]*|[ \t]*/[*].*(?=\*/)\*/')
findCommentAfterLBraces = re.compile('(?<=\{)[ \t]*//[^\n]*')
findCommentAfterRBraces = re.compile('(?<=\})[ \t]*//[^\n]*')
findCommentAfterRBraces2= re.compile('(?<=\};)[ \t]*//[^\n]*')
findEndClassSpace       = re.compile("\}\s*;",re.M)
findQualSpace           = re.compile("(?<=\))\s*(?=[a-zA-Z])",re.M)
findSections            = re.compile(section,re.M) 
findForwards            = re.compile(forward,re.M)
findIncludes            = re.compile(include,re.M)
findExterns             = re.compile(extern,re.M)
findInitlists           = re.compile(initlist,re.M)
findMacros              = re.compile(macrodef,re.M)
findMacroname           = re.compile(macrodefname,re.M)
findForwardname         = re.compile(forwardname, re.M)
findIncludename         = re.compile(includename, re.M)
findReturnTypeQual      = re.compile('(virtual|static|'
									 'explicit|friend|inline)[ \t]+',re.M)
findEnumStructType      = re.compile(r'(?P<name>\b(enum|struct)\b(\s+\w+)?)',
									 re.M)
findSemiColon           = re.compile('(?P<end>;(?![ \t]*(/[*]|//)))', re.M)
findColon               = re.compile('(?P<end>:(?![ \t]*(/[*]|//)))', re.M)
findEnumName            = re.compile('\\benum\\b\s*(?P<name>\w+)?\s*',re.M)

groups = (ocomment,scomment3,scomment2,ccomment)
format = (len(groups)-1)*'%s|'+'%s'
allcomments = re.compile(format % groups,re.M+re.S)
getclassname= re.compile(r"\bclass[ \t]+(?P<name>\w+)[ \t]*;",re.M)
findcommenttoken = re.compile(r'(?P<comment>\\comment[0-9]{3})')
#===========================================================================
# Functions
#===========================================================================
def usage():
	print USAGE

def help():
	print HELP

def fatal(s):
	print "** Error ** %s" %s
	print "\tgoodbye!"
	sys.exit(1)

def warning(s):
	print "\t** Warning ** %s" %s

def inform(s):
	print "\t** %s" %s

def tabString(s,tab=0):
	return "%s%s" % (tab*'\t',strip(s))

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

def macroName(macro):
	m = findMacroname.search(macro)
	if m <> None:
		return m.group('name')
	else:
		return ""

def forwardName(record):
	m = findForwardname.search(record)
	if m <> None:
		return m.group()
	else:
		return ""

def classNamesFromHeader(record):
	names = []
	recs = findAll(beginclasses,record)
	for str, g, s, e in recs:
		name = getClassname(str)[0]
		names.append(name)
	return names

def includeName(record):
	m = findForwardname.search(record)
	if m <> None:
		return nameonly(m.group())
	else:
		return ''

def constructorName(classname):
	name = strip(splitfields(classname,'<',1)[0])
	return name

#---------------------------------------------------------------------------
def squeezeblanks(record):
	last = ''
	recs = []
	for rec in split(record,'\n'):
		if strip(rec) == '':
			rec = ''
			if last == '': continue
		recs.append(rec)
		last = strip(rec)        
	return joinfields(recs,'\n')
#---------------------------------------------------------------------------
# This class models the variables that define a given class context.
# The class is necessary because we may have nested classes
#---------------------------------------------------------------------------
def classType(names):
	abstractClass = find(names['type'], 'abstract') > -1
	regularClass  = find(names['type'], 'regular') > -1
	templateClass = find(names['type'], 'template') > -1        
	specializedClass = find(names['type'], 'specialized') > -1        
	return (abstractClass,
			regularClass,
			specializedClass,
			templateClass)

def setClassType(names,
				 abstractClass,
				 regularClass,
				 specializedClass,
				 templateClass):        
	if abstractClass:
		names['type'] = 'abstract'
	else:
		names['type'] = ''

	if specializedClass:
		names['type'] = names['type'] + ' specialized'

	if templateClass:
		names['type'] = names['type'] + ' template'

	if regularClass:
		names['type'] = names['type'] + ' regular'

	if names['group'] == 'structclass':
		names['type'] = names['type'] + ' struct'

class ClassContext:
	def __init__(self, names):
		self.context = []

		# Initial class context

		names['group'] = ''
		names['inclass'] = 0
		names['namespace'] = []
		names['classname'] = ''
		names['fullclassname'] = ''
		names['classdepth'] = 0
		names['classpath'] = ''
		names['template'] = ''
		names['type'] = ''
		names['section'] = ''
		names['constructorname'] = ''
		names['basenames'] = []
		names['cppbody'] = []
		names['cppend'] = []
		names['argslist'] = []

	def __del__(self):
		pass

	def push(self, names):
		a = {} # Important: Make copies!
		for key in names.keys():
			a[key] = names[key]            
		self.context.append(a)

	def pop(self):
		if len(self.context) == 0: return {}
		a = self.context.pop()
		return a

	def show(self, names=None):
		if names == None:
			s = '\n\tPrevious Context\n'
			a = self.context[len(self.context)-1]
			tab = '\t\t'
		else:
			s = '\n\tCurrent Context\n'
			tab = '\t'
		s = s + tab + 'namespace:  %(namespace)s\n' 
		s = s + tab + 'classname:  %(classname)s\n'
		s = s + tab + 'classdepth: %(classdepth)d\n' 
		s = s + tab + 'type:       %(type)s\n'
		s = s + tab + 'template:   %(template)s\n'
		s = s + tab + 'section:    %(section)s\n'
		s = s + tab + 'inclass:    %(inclass)s\n'
		if names == None:
			return s % a
		else:
			return s % names            
#---------------------------------------------------------------------------
# Update XmlMap.py 
#---------------------------------------------------------------------------
def updateXmlMap(xmlfile):
	try:
		from XmlMap import XmlMap
	except:
		XmlMap = {}
		warning("Re-building XmlMap **")

	from pprint import PrettyPrinter

	headerelement = readXmlFile(xmlfile)
	if headerelement == None:
		fatal("unable to read file\n%s" % file)

	# Get path name of XML file, relative to PYTHON_PROJECTS
	
	xmlfile = "%s/%s" % (PWD, xmlfile)
	
	xmlfile = replace(xmlfile, BASE+"/", '')
	
	# Add an entry to XmlMap for the header in case its name
	# differs from the name of one of its classes.

	update = 0

	name = nameonly(headerelement.attrib['name'])
	
	if not XmlMap.has_key(name):
		update = 1
		XmlMap[name] = []
		XmlMap[name].append(xmlfile)
		inform("Add entry %s to XmlMap" % name)

	elif not xmlfile in XmlMap[name]:
		update = 1
		XmlMap[name].append(xmlfile)
		inform("Update entry %s in XmlMap" % name)

	# Add an entry to XmlMap for each class in header

	classes = []
	findClassElements(headerelement, classes)
	for element in classes:
		name = element.attrib['name']
		if not XmlMap.has_key(name):
			update = 1
			XmlMap[name] = []
			XmlMap[name].append(xmlfile)
			inform("Add entry %s to XmlMap" % name)

		elif not xmlfile in XmlMap[name]:
			update = 1
			XmlMap[name].append(xmlfile)
			inform("Update entry %s in XmlMap" % name)

		fullname = element.attrib['fullname']
		if not XmlMap.has_key(fullname):
			update = 1
			XmlMap[fullname] = []
			XmlMap[fullname].append(xmlfile)
			inform("Add entry %s to XmlMap" % fullname)

		elif not xmlfile in XmlMap[fullname]:
			update = 1
			XmlMap[fullname].append(xmlfile)
			inform("Update entry %s in XmlMap" % fullname)

		fullname = split(fullname,'<')[0]
		if not XmlMap.has_key(fullname):
			update = 1
			XmlMap[fullname] = []
			XmlMap[fullname].append(xmlfile)
			inform("Add entry %s to XmlMap" % fullname)

		elif not xmlfile in XmlMap[fullname]:
			update = 1
			XmlMap[fullname].append(xmlfile)
			inform("Update entry %s in XmlMap" % fullname)                 

	# Re-write XmlMap only if needed

	if not update: return

	# Need to update database.

	#inform("Re-building XmlMap Database")

	base = os.environ['PYTHON_PROJECTS']
	xmlmap = XMLMAPFILE

	names={'base':   base,
		   'version':'v1',
		   'date':   ctime(time()),
		   'xmlmap': xmlmap}

	out = open("%(xmlmap)s.tmp" % names,'w')
	out.write(79*"#"+'\n')
	out.write("# File:    XmlMap.py\n" % names)
	out.write("# Created: %(date)s"
			  " %(version)s\n" % names)
	out.write("#$Revision: 1.3 $\n")
	out.write(79*"#"+'\n')
	out.write("XmlMap =\\\n")
	PP = PrettyPrinter(stream=out)
	PP.pprint(XmlMap)
	out.write("\n")

	# Now rename
	command = ""
	if os.path.exists("%(xmlmap)s" % names):
		command = command + "cp %(xmlmap)s %(xmlmap)s.bak; " % names
	command = command + "mv %(xmlmap)s.tmp %(xmlmap)s;" % names
	command = command + "rm -rf %(xmlmap)sc" % names
	os.system(command)
#---------------------------------------------------------------------------
# Read list of headers from src directory
#---------------------------------------------------------------------------
def getHeaderList():
	if not os.path.exists("src"):
		fatal("getHeaderList - can't find directory src\n")

	if not os.path.exists(HEADERSFILE):
		fatal("getHeaderList - can't find file %s\n" % HEADERSFILE)

	files = map(lambda x: strip(x), open(HEADERSFILE,"r").readlines())
	names = set()
	filelist = []
	for file in files:
		name = nameonly(file)
		if name in names:
			print "** Duplicate file %s ignored" % file
		else:
			filelist.append(file)
			names.add(name)
	return filelist
#---------------------------------------------------------------------------
# Extract possible macros from header
#---------------------------------------------------------------------------
def headerMacros(record):
	macros = []
	macs = findAll(findMacros,record)
	for macro, group, start, end in macs:
		name = macroName(macro)
		macros.append((name,macro))
	return macros
#---------------------------------------------------------------------------
# Use CPP to clean-up header before parsing
#---------------------------------------------------------------------------
# A faster version
def findExpandedIncludes(record):
	iii = 0
	jjj = 0
	cnt = 0
	bnds = []
	while jjj > -1 and cnt < 500:
		cnt += 1
		jjj = find(record[iii:],'\INCLBEGIN')
		if jjj < 0: break
		start = iii + jjj
		kkk = find(record[start:],'\INCLEND')
		end   = start + kkk + 8
		bnds.append((start,end))
		iii = end
	return bnds

def cpp(record, macros, includepaths, items):
	debug(1,['CPP',macros])

	allincs = findAll(findIncludes, record)
	debug(2,['Includes'])
	tokens = []
	for i, (str, group, start, end) in enumerate(allincs):
		str = stripBlanklines(rstrip(str))
		debug(2,[str,78*'-'],0)
		placeholder = placeHolder("include", i)
		items[placeholder] = strip(splitfields(str,"include ",1)[1])
		newstr = placeholder+'\n\INCLBEGIN\n'+str+'\n\INCLEND'
		tokens.append((newstr,start,end))
	if len(tokens) > 0:            
		record = splice(record, tokens) 

	open('.header.h','w').write(record+'\n')
	# No linenumbers
	# c++
	# Don't search system headers
	# Discard comments
	command = "(cpp -P -xc++ -nostdinc -nostdinc++ %s %s" % \
			  (macros,includepaths)
	command = command + " .header.h > .cpp.h) >& .cpp.log"
	os.system(command)
	record = open(".cpp.h").read()
	debug(4,['Expanded Header',record])

	# ---------------------------------------------------
	# Get rid of expanded headers
	# ---------------------------------------------------
	if VERBOSE: print "find expanded includes"
	expheaders = findExpandedIncludes(record)
	tokens = []
	for i, (start, end) in enumerate(expheaders):
		#print "%d\t<%s>\t%d\t<%s>" % (start,
		#                              record[start:start+10],
		#                              end, record[end-8:end])
		tokens.append(('',start,end))
	if len(tokens) > 0:
		record = splice(record, tokens)

	# ---------------------------------------------------    
	# Get rid of some weird affectations
	# ---------------------------------------------------    
	record = replace(record," ;", ";")
	record = replace(record,"( ", "(")
	record = replace(record," )", ")")
	#record = replace(record,"{};", ";") #### Apr 23, 2005 HBP
	record = replace(record,"public :",   "public:")
	record = replace(record,"protected :","protected:")
	record = replace(record,"private :",  "private:")
	record = replace(record,"public:",   "public:\n")
	record = replace(record,"protected:","protected:\n")
	record = replace(record,"private:",  "private:\n")
	record = fixresop.sub("::", record)
	#record = replace(record,"std:: ",  "std::")
	record = findEndClassSpace.sub("};",record)
	record = findQualSpace.sub(" ",record)
	record = findSemiColon.sub(";\n",record)

	# Add a \n after "{". Should be harmless, but easy way to take
	# care of things like: namespace { class YaaHoo; }
	record = replace(record,'{','{\n')
	debug(1,['After CPP',rstrip(record)])
	return record
#---------------------------------------------------------------------------
# Class to write out stuff in XML...kinda obvious huh!
#---------------------------------------------------------------------------
def namespaceName(record):
	name = splitfields(record,'{',1)[0]
	name = strip(replace(name,'namespace',''))
	return name

class XMLWriter:
	def __init__(self):
		self.records = []
		self.xml = None

	def __del__(self):
		pass

	def open(self, xmlfile, names):
		self.xmlfile = xmlfile
		self.records = []

		writeXML = self.writeXML        
		writeXML('<?xml version="1.0"?>')
		writeXML("<!--")
		writeXML(78*'=')
		writeXML("  XML File: %(headername)s.xml" % names)
		writeXML("  Created:  %(time)s with header2xml.py %(VERSION)s" % names)
		writeXML(78*'=')
		writeXML("-->")

		self.xmltab = 0
		writeXML('<header name="%(headerpath)s"' % names)
		writeXML('        file="%(headerfile)s">' % names)
		writeXML('')
		self.xmltab += TAB
		return 1

	def writeXML(self, s, tab=0, newLine=1):
		if newLine:
			newline = "\n"
		else:
			newline = ""
		if tab > 0:
			self.records.append('%s%s%s' % (tab*' ',s,newline))
		else:
			self.records.append('%s%s' % (s,newline))

	def write(self, group, comment, record, names, classdepth):
		#---------------------------------------------------------
		# Write out record in XML format
		#---------------------------------------------------------
		writeXML= self.writeXML
		xmltab  = self.xmltab

		debug(1,[record],0)

		comment = convert2html(comment)
		chdata  = convert2html(standardizeName(record))

		if group == "namespace":
			name = namespaceName(record)
			writeXML('')
			writeXML('<namespace name="%s">' % name, xmltab)
			xmltab += TAB
			if comment <> "":
				writeXML('<comment>', xmltab)
				writeXML(comment)
				writeXML('</comment>', xmltab)

		elif group == "endnamespace":
			xmltab -= TAB
			writeXML('</namespace>', xmltab)

		elif group == "class" or group == "structclass":
			classname = convert2html(names['classname'])
			fullname  = convert2html(names['fullclassname'])
			module    = convert2html(names['module'])
			project   = convert2html(names['project'])
			gname = 'class'
			gstr  = len(gname)*' '
			writeXML('')            
			writeXML('<%s name="%s"' % (gname, classname), xmltab)
			writeXML(' %s fullname="%s"' % (gstr,fullname),xmltab)
			writeXML(' %s module="%s"' % (gstr,module),xmltab)
			writeXML(' %s project="%s"' % (gstr,project),xmltab)

			# The placeholder will be replaced later
			classtype = '%(classname)sCLASSTYPE' % names
			writeXML(' %s type="%s"' % (gstr, classtype),xmltab,0)

			ctype = names['template']
			if ctype <> None:
				ctype = findSpace.sub(' ',ctype)
				ctype = convert2html(standardizeName(ctype))
				writeXML('')
				writeXML('       template="%s"' % ctype, xmltab, 0)
			writeXML('>')

			xmltab += TAB
			if len(names['basenames']) > 0:
				for name in names['basenames']:
					name = convert2html(standardizeName(name))
					writeXML('<baseclass name="%s"/>' % name, xmltab)
			if comment <> "":
				writeXML('<comment>', xmltab)
				writeXML(comment)
				writeXML('</comment>', xmltab)
			writeXML('')
			writeXML('<section>', xmltab)                
			xmltab += TAB

		elif group == "endclass" or group == "endstructclass":

			# Update the class type

			classtype = '%(classname)sCLASSTYPE' % names
			classtyperecord = 'type="%s"'% classtype
			for index, record in enumerate(self.records):
				if   find(record, classtyperecord) > -1:
					self.records[index] = replace(self.records[index],
												  classtype,
												  strip(names['type']))
					break

			xmltab -= TAB
			writeXML('</section>', xmltab)
			writeXML('')

			# Add typemaps

			if names.has_key('typemap'):
				typemap = names['typemap']
				keys = typemap.keys()
				keys.sort()
				typeMap = {}
				for key in keys:
					value, ttype = typemap[key]
					if ttype == TYPEDEFTYPE:
						value = unfoldTypedef(key, value, typemap)

					typeMap[key] = (value, ttype)

				gname= 'typemap'
				gstr = len(gname)*' '
				for index, key in enumerate(keys):
					value, ttype = typeMap[key]
					key   = convert2html(key)
					value = convert2html(value)
					writeXML('<%s name="%s"'   % (gname,key), xmltab)
					writeXML(' %s value="%s"'  % (gstr,value),xmltab)
					writeXML(' %s type="%s"/>' % (gstr,TTYPE[ttype]),xmltab)
				if len(keys) > 0: writeXML('')

			xmltab -= TAB
			writeXML('</class>', xmltab)
			self.updateTypeField(names)

		elif group == "section":
			xmltab -= TAB
			writeXML('</section>', xmltab)
			writeXML('<section name="%s">' % chdata, xmltab)
			xmltab += TAB

		else:

			# OTHER GROUP (but we should never get
			# here if this is a comment)

			if group == "comment":
				print "** XMLWriter ** group == comment; should NEVER happen!"
				print classname
				sys.exit(1)

			elif group == "include":
				writeXML('<%s name="%s"/>' % (group,chdata), xmltab)

			elif group == "forward":
				writeXML('<%s name="%s"/>' % (group,chdata), xmltab)

			elif group == "constant":
				chdata = replace(chdata,', ',',')
				tmp = split(chdata,'=')
				if len(tmp) == 2:
					# Assume
					# <type> <name> = <value>
					name, value = tmp
					tmp  = split(name)
					name = tmp.pop()
					ctype= joinfields(tmp)
				else:
					# Assume
					# <type> <name>
					tmp  = split(chdata)
					name = split(tmp.pop(),'(')[0]                    
					ctype= joinfields(tmp)
					value= ''                    
				value = strip(value)
				ctype = strip(ctype)
				gstr = len(group)*' '
				writeXML('<%s name="%s"'   % (group,convert2html(name)),xmltab)
				writeXML(' %s type="%s"'   % (gstr,convert2html(ctype)),xmltab)
				writeXML(' %s value="%s"/>'% (gstr,convert2html(value)),xmltab)

			elif group == "typedef":

				name, value = decodeTypedef(convertFromHtml(chdata))

				value = strip(value)
				name  = strip(name)
				gstr  = len(group)*' '
				writeXML('<%s name="%s"'   % (group,convert2html(name)),xmltab)
				writeXML(' %s value="%s">' % (gstr,convert2html(value)),xmltab)

				writeXML(chdata)
				if comment <> "":
					xmltab += TAB
					writeXML('<comment>', xmltab)
					writeXML(comment)
					writeXML('</comment>', xmltab)
					xmltab -= TAB
				writeXML('</%s>\n' % group, xmltab)

			elif group in METHOD:                
				retype, name, args, vars, qual = names['argslist']
				writeXML('<%s name="%s"' % (group,convert2html(name)),xmltab)
				gstr = len(group)*' '
				writeXML(' %s return="%s"'% (gstr,convert2html(retype)),xmltab)
				writeXML(' %s args="%s"'  % (gstr,
											 convert2html(packArgs(args))),
						 xmltab)
				writeXML(' %s vars="%s"'  % (gstr,
											 convert2html(packArgs(vars))),
						 xmltab)                
				writeXML(' %s qual="%s">' % (gstr,convert2html(qual)),xmltab)
				writeXML(chdata)
				if comment <> "":
					xmltab += TAB
					writeXML('<comment>', xmltab)
					writeXML(comment)
					writeXML('</comment>', xmltab)
					xmltab -= TAB
				writeXML('</%s>\n' % group, xmltab)

			elif group == "enum":
				name, fields = decodeEnum(chdata)

				writeXML('<%s name="%s">' % (group,convert2html(name)),xmltab)
				writeXML(chdata)

				gstr = len('field')*' '
				xmltab += TAB
				for index, (name, value) in enumerate(fields):
					value = convert2html(value)
					str = '<field name="%s" value="%s"/>' % (name, value)
					if len(xmltab*' '+str) < 79:
						writeXML(str, xmltab)
					else:
						writeXML('<field name="%s"' % name, xmltab) 
						writeXML(' %s value="%s"/>' % (gstr,value),xmltab)
				xmltab -= TAB

				if comment <> "":
					xmltab += TAB
					writeXML('<comment>', xmltab)
					writeXML(comment)
					writeXML('</comment>', xmltab)
					xmltab -= TAB
				writeXML('</%s>\n' % group, xmltab)                

			else:
				writeXML('<%s>' % group, xmltab)
				writeXML(chdata)
				if comment <> "":
					xmltab += TAB
					writeXML('<comment>', xmltab)
					writeXML(comment)
					writeXML('</comment>', xmltab)
					xmltab -= TAB
				writeXML('</%s>\n' % group, xmltab)                    
		self.xmltab = xmltab

	def updateTypeField(self, names):
		lrecords = len(self.records)
		for index in xrange(lrecords):
			record = self.records[lrecords-index-1]
			names['type'] = strip(names['type']) # Exclude value
			if find(record,'%(type)s') > -1:
				self.records[lrecords-index-1] = record % names
				break

	def close(self, typelist, classlist):
		self.xml = None
		self.xml = open(self.xmlfile,'w')
		if self.xml == None: return 0

		if self.xml <> None:
			self.writeXML('')
			tlist = map(lambda x: x, typelist)
			tlist.sort()
			for t in tlist:
				chdata  = strip(convert2html(standardizeName(t)))
				self.writeXML('<type name="%s"/>' % chdata, self.xmltab)

			self.xmltab -= TAB
			self.writeXML("</header>", self.xmltab)

			# Ignore empty sections

			for index, record in enumerate(self.records):
				if   find(record,'<section') > -1:
					if find(self.records[index+1],'</section') > -1:
						self.records[index] = self.records[index+1] = ''

			record = joinfields(self.records,'')
			self.xml.write(record)

			self.xml.close()
			self.xml = None
			self.records = []
#---------------------------------------------------------------------------
# Recursively translate a typedef back to underlying type.
# This is a bit hairy but seems to work often enough to be ok for now!
#---------------------------------------------------------------------------
# !IMPORTANT! do not include "iterator" amongst stltypes
stl_types = '\\b('+joinfields(STL_TYPES[:-1],'|')+')\\b'
s = '(std::)?%s\<.+\>::(?P<type>\w+)' % stl_types
stlcheck = re.compile(s)
addstd   = re.compile(r'(?<!std::)\b%s\b' % stl_types)

def unfoldTypedefOnce(value, typemap, used):
	words = swords.findall(value)

	for index, word in enumerate(words):
		if not typemap.has_key(word): continue
		if word in used: continue
		used.append(word)

		# Do not translate if we have <namespace>::<word>,
		# where <namespace> is in NamespaceList

		if index > 0:
			previousword = words[index-1]
			name = '%s::%s' % (previousword, word)
			if find(value, name) > -1:
				if previousword in NAMESPACELIST: continue

		newvalue, ttype = typemap[word]

		# Translate only if we have a typedef

		if ttype <> TYPEDEFTYPE: continue

		# Do not translate if we map directly to an enum

		####D
		#print "\tWORD( %s ), NEWVALUE( %s )" % (word, newvalue)

		swap = re.compile(r'\b%s\b' % word)
		value = swap.sub(newvalue, value)

		#print "\tVALUE( %s )" % value

		# Prefix stl types with "std::" if needed
		values = addstd.findall(value)
		####D
		#print "STLVALUES( %s )" % values
		for v in values:
			value = re.sub(r'(?<!std::)\b%s\b' % v, "std::%s" % v, value)

		####D
		#print "\tAFTER STLVALUES( %s )" % value

		# Fix stl types involving iterators
		m = stlcheck.findall(value)
		for a,b,c in m:
			used.append(c)
			if c in ['iterator',
					 'const_iterator',
					 'reference',
					 'const_reference']:
				if value[0:8] <> 'typename':
					if find(value, 'typename') > -1:
						value = 'typename %s' % value
	####D
	#print 80*'-'
	return value
#---------------------------------------------------------------------------
def unfoldTypedef(key, value, typemap):
	used = [key,'typename']
	count = 0
	while count < 20:
		newvalue = unfoldTypedefOnce(value, typemap, used)
		if newvalue == value: return replace(newvalue,'>>','> >') # HACK!
		value = newvalue

		count += 1
	fatal("unfoldTypedef - lost in woods\n"\
		  "\tkey=%s, value=%s" % (key, value))
	return value
#---------------------------------------------------------------------------
# Write out some book-keeping stuff
#---------------------------------------------------------------------------
def writeOutMethodComment(key,value,delim):
	if value <> "":
		OUTMETHODCOM.write("\n\t'%s':\n'''\n%s\n'''%s" % (key,value,delim))
	else:
		OUTMETHODCOM.write("\n\t'%s':\n'''\n'''%s" % (key,delim))

def writeOutClassComment(key,value,delim):
	if value <> "":
		OUTCLASSCOM.write("\n\t'%s':\n'''\n%s\n'''%s" % (key,value,delim))
	else:
		OUTCLASSCOM.write("\n\t'%s':\n'''\n'''%s" % (key,delim))
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
# This is the most important routine. If it breaks, header2xml breaks!
# It is used to find constructs of the form
#
#      <Identifier ...> <leftDelim> .... <rightDelim>
#
# For example,
#      SomeRoutine (......)
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

def findCandidateFunctions(record):
	return findComponents(functions,record,'(',')',0)

def findBodies(record):
	return findComponents(bodies,record,'{','}',0)

def findTemplates(record):
	return findComponents(begintemplates,record,'{','}')
#---------------------------------------------------------------------------
# Look for classes in current element and recursively within each class
#---------------------------------------------------------------------------
def findClassElements(elem, classes, depth=0):
	if elem == None: return

	depth += 1
	if depth >= 20:
		fatal("lost in tree...I'm bailing!\n\t\t%s" % elem.tag)

	# Loop over elements

	for element in elem.getchildren():
		if element.tag == 'namespace':
			findClassElements(element, classes, depth)
		elif element.tag == 'section':
			findClassElements(element, classes, depth)
		elif element.tag == 'class':
			classes.append(element)
			findClassElements(element, classes, depth)
	return
#---------------------------------------------------------------------------
# Look for typedefs in current element and recursively
#---------------------------------------------------------------------------
def findTypedefElements(elem, elements, depth=0, namespace=[]):
	depth += 1
	if depth >= 20:
		fatal("lost in tree...I'm bailing!\n\t\t%s" % elem.tag)

	# Loop over elements

	for element in elem.getchildren():
		if element.tag == 'namespace':
			if element.attrib.has_key('name'):
				namespace.append(element.attrib['name'])

			findTypedefElements(element, elements, depth, namespace)

			if element.attrib.has_key('name'):
				namespace.pop()

		elif element.tag == 'class':
			namespace.append(element.attrib['name'])

			findTypedefElements(element, elements, depth, namespace)

			namespace.pop()

		elif element.tag == 'section':

			findTypedefElements(element, elements, depth, namespace)          

		elif element.tag == 'typedef':
			key, value = decodeTypedef(element.text)
			namespace.append(key)
			fullname = joinfields(namespace,'::')
			namespace.pop()
			element.attrib = {'name':key, 'fullname': fullname, 'value':value}
			elements.append(element)
	return
#---------------------------------------------------------------------------
# Look for namespaces
#---------------------------------------------------------------------------
def listNamespaces(elem, names, depth=0, namespace=[]):
	depth += 1
	if depth >= 20:
		fatal("lost in tree...I'm bailing!\n\t\t%s" % elem.tag)

	# Loop over elements

	for element in elem.getchildren():
		if element.tag == 'namespace':
			if element.attrib.has_key('name'):                                
				namespace.append(element.attrib['name'])
				str = joinfields(namespace,'::')
				if not str in names: names.append(str)

			listNamespaces(element, names, depth, namespace)

			if element.attrib.has_key('name'):
				namespace.pop()
	return
#---------------------------------------------------------------------------
# Given template <class X, etc.> extract [X,...]
#---------------------------------------------------------------------------
findtemplatepars = re.compile('(?<=class )(?P<par>\w+)')
def templateParameters(element):
	if element == None: return []
	if type(element) == type(""):
		template = element
	else:
		if not element.attrib.has_key('template'): return []        
		template = element.attrib['template']
	return findtemplatepars.findall(replace(template,'typename','class'))

squeezeEq = re.compile('\s*=\s*')
def templateParList(record):
	m = templatepar.search(record)
	if m == None: fatal("templateParList - "\
						"can't get template parameters from\n\t" + record)
	template = m.group("par")
	template = squeezeEq.sub('=', template)
	template = replace(replace(template,'class ',''),'typename ','')
	template = map(lambda x: strip(split(x,'=')[0]), split(template,','))
	return template
#---------------------------------------------------------------------------
# Find all strings that satisfy given regular expression.
# For each string return its group name and its position within the record.
#---------------------------------------------------------------------------
def findAllSame(regex,s):
	iterator = regex.finditer(s)
	strlist = []
	for m in iterator:
		s = (rstrip(m.group()),m.groupdict().keys()[0],m.start(),m.end())
		strlist.append(s)
	return strlist

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

def printAll(l,toscreen=0):
	str = ""
	for x in l:
		if len(x) == 4:
			s, groupname, start, end = x
		elif len(x) == 6:
			s, groupname, start, left, right, end = x            
		else:
			s, bnds = x
			start, end = bnds
			groupname = "unknown"                
		str = str + "\t\t\t\t\t\t//%s\n%s\n" % (groupname,s)
		str = str + 78*"-" + "\n"
	if toscreen:
		print str
	return str

def packArgs(args):
	s = '['
	for arg in args:
		s = s + "'%s'," % arg
	s = "%s]" % s[:len(s)-1]
	return s
#---------------------------------------------------------------------------
# Get C and C++ style non-inline comments and convert to a unified
# style known to doxygen.
#---------------------------------------------------------------------------
def getComments(record):
	origcomments = findAll(allcomments, record)

	comments = []
	for i, (comment,group,start,end) in enumerate(origcomments):
		if group == "scomment":
			pass
			#end -= 1 # Hmm...a bit of a hack!
		elif group == "ocomment":
			comments.append((rstrip(comment),group,start,end))
			continue

		lines = splitfields(comment,'\n')
		s = ""
		for line in lines:
			l = line
			l = replace(l, "/**","   ")
			l = replace(l, "/*!","   ")
			l = replace(l, "//!","   ")
			l = replace(l, "/*","  ")
			l = replace(l, "*/","")
			l = replace(l, "*\\","")
			l = replace(l, "\\*","")
			if find(l,'****************************') > -1: continue
			if find(l,'////////////////////////////') > -1: continue
			if find(l,'----------------------------') > -1: continue

			n = len(l)
			if l[n-2:n] == " *": l = l[:n-2]
			if l[n-2:n] == "//": l = l[:n-2]
			l = rstrip(l)
			a = strip(l)
			if a == '/': break # A hack!

			if len(a) > 0:
				if a[0]   == "*":
					l = replace(l,'*',' ',1)
			if len(a) > 3:
				if    a[0:4] == "///<":
					l = replace(l,'///<','    ',1)
				elif a[0:4] == "//!<":
					l = replace(l,'//!<','    ',1)
			if len(a) > 2:
				if a[0:3] == "///":
					l = replace(l,'///','   ',1)
			if len(a) > 1:
				if a[0:2] == "//":
					l = replace(l,'//','  ',1)
			if (s == "") and (a == ""): continue
			s = s + l + "\n"
		s = stripBlanklines(s)
		if s <> "":
			comments.append((s,group,start,end))
	return comments
#---------------------------------------------------------------------------
# Convert inline comments to a style known to doxygen
#---------------------------------------------------------------------------
def fixInlineComments(record, inclass=1):
	record = replace(record,"/**","\t///< ")
	record = fixinline.sub("\t///< ",record)
	if inclass:
		record = fixinline1in.sub("\t///< ",record)
		record = fixinline2in.sub("",record)
	else:
		record = fixinline1.sub("\t///< ",record)
		record = fixinline2.sub("",record)
	return record
#--------------------------------------------------------
# Find template functions or classes
#--------------------------------------------------------
def extractTemplatedItems(record):
	items = findTemplates(record)
	if len(items) == 0: return (record, [])

	skip = len(items)*[0]
	for i,(str, group, start, left, right, end) in enumerate(items):
		for j, (str1, g, s, l, r, e) in enumerate(items):
			if (start < s) and (e < end):
				skip[j] = 1

	trecs = []
	tokens = []
	for i, (str, group, start, left, right, end) in enumerate(items):
		if skip[i]: continue
		end += 1
		if record[end-1] == ';':
			end += 1
			str += ';'

		tokens.append(('',start,end))
		trecs.append(str)
	if len(tokens) > 0:
		record = splice(record, tokens)
	record = squeezeblanks(record)
	return (record, trecs)
#===========================================================================
# The workhorse
# -------------
# Parse header and try to identify different constructs. The basic idea is
# to identify a construct and replace it with a placeholder.
#
# IMPORTANT: We must find constructs in the right order so that the heuristic
#            rules work. For example, it is important to get rid of function
#            bodies before we look for functions. It is necessary, however,
#            to find enums, structs etc. before we remove bodies and so on!
#            This is necessary because we are not performing a rigorous
#            syntax analysis.
#===========================================================================
def parseHeader(record, scrub, macros, includepaths): 
	record = rstrip(record)
	debug(1,['=== Original Header ===',record])

	# ---------------------------------------------------
	# Remove application-specific stuff
	# ---------------------------------------------------
	if VERBOSE: print "scrub header"
	record = scrub.sub("", record)
	debug(2,['=== After SCRUB ===',record])

	items  = {} # map of identified constructs

	# ---------------------------------------------------
	# Find non-inline comments and replace
	# with numbered placeholders
	# ---------------------------------------------------
	if VERBOSE: print "parse comments"
	record = parseComments(record, items)

	# ---------------------------------------------------
	# Clean up with CPP
	# ---------------------------------------------------
	if VERBOSE: print "invoke cpp"
	record = cpp(record, macros, includepaths, items)

	# ---------------------------------------------------
	# Find forward declarations
	# ---------------------------------------------------
	if VERBOSE: print "parse forward declarations"
	record = parseForwards(record, items)

	# ---------------------------------------------------
	# Find namespace preambles and ends and replace them.
	# Since these could be nested we need to order the
	# identified constructs before replacing them.
	# ---------------------------------------------------    
	if VERBOSE: print "parse namespace boundaries"    
	record = parseNamespaceBoundaries(record, items)

	# ---------------------------------------------------    
	# Find class preambles and ends and replace with
	# placeholders. Like namespaces, make sure we sort
	# them in case we have nested classes.
	# ---------------------------------------------------    
	if VERBOSE: print "parse class boundaries"
	oldrecord = record
	record = parseClassBoundaries(record, items)
	if record == '': record = oldrecord

	# ---------------------------------------------------        
	# Find structs, enums, typedefs etc.
	# and replace them with placeholders
	# ---------------------------------------------------    
	if VERBOSE: print "parse enums"
	record = parseEnums(record, items)

	# ---------------------------------------------------
	# Find function bodies and empty them
	# ---------------------------------------------------
	if VERBOSE: print "find bodies"
	record = parseFunctionBodies(record, items)

	# ---------------------------------------------------
	# Can *now* get rid of initialization lists
	# ---------------------------------------------------    
	if VERBOSE: print "remove initialization lists"
	record = findInitlists.sub("",record)
	debug(3,['After Removal of Initialization Lists',record])

	# ---------------------------------------------------
	# Can *now* remove empty bodies
	# ---------------------------------------------------    
	if VERBOSE: print "remove bodies"
	record = replace(record,'{}','')
	debug(3,['After Removal of Empty Bodies', record])

	# ---------------------------------------------------
	# Find functions
	# ---------------------------------------------------    
	if VERBOSE: print "find functions"
	record = parseFunctions(record, items)

	# ---------------------------------------------------
	# Get rid of extern declarations
	# ---------------------------------------------------
	if VERBOSE: print "remove extern declarations"
	allexterns = findAll(findExterns, record)
	debug(3,['Extern Declarations', printAll(allexterns)])
	record = findExterns.sub("",record)
	debug(3,['After Removal of Extern Declarations',rstrip(record)])

	# ---------------------------------------------------
	# Find friends and replace them!
	# ---------------------------------------------------
	if VERBOSE: print "find friends"
	record = parseFriends(record, items)

	# ---------------------------------------------------
	# Find constants; NB: MUST do after find functions!
	# ---------------------------------------------------    
	if VERBOSE: print "find constants"
	record = parseConstants(record, items)

	# ---------------------------------------------------
	# Hey! We're done!
	# ---------------------------------------------------                
	debug(1,['<<< Parsed Header >>>',rstrip(record)])
	if VERBOSE: print "parsing complete"
	return (record, items)
#---------------------------------------------------------------------------
#------------------------------------ FUNCTIONS USED BY parseHeader --------
#---------------------------------------------------------------------------
def placeHolder(str, n):
	return "\%s%3.3d" % (str,n)

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
# Find non-inline comments and replace
# with numbered placeholders
#--------------------------------------------------------
def parseComments(record, items):
	debug(2,['Comments'])
	comments = getComments(record)
	tokens = []
	for i, (str, group, start, end) in enumerate(comments):
		debug(2,[str,78*'-'],0)
		placeholder = placeHolder("comment", i)
		items[placeholder] = str
		tokens.append((placeholder,start,end))
	if len(tokens) > 0:
		record = splice(record, tokens)
		debug(2,['After Replace Comments',rstrip(record)])
	return record
#--------------------------------------------------------        
def parseForwards(record, items):
	debug(3,['Forward Declarations'])
	allforwards = findAll(findForwards, record)
	tokens = []
	for i, (str, group, start, end) in enumerate(allforwards):
		str = stripBlanklines(rstrip(str))
		debug(3,[str,78*'-'],0)
		placeholder = placeHolder("forward", i)
		items[placeholder] = strip(str)
		tokens.append((placeholder,start,end))
	if len(tokens) > 0:            
		record = splice(record, tokens)
	return record
#--------------------------------------------------------        
def parseConstants(record, items):
	debug(2,['Constants'])
	allconstants = findAll(constants, record)
	tokens = []
	for i, (str, group, start, end) in enumerate(allconstants):
		str = stripBlanklines(rstrip(str))
		debug(2,[str,78*'-'],0)
		placeholder = placeHolder("constant", i)
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
		debug(1,['After Replace Class Boundaries', rstrip(record)])
	return record
#--------------------------------------------------------
def parseEnums(record, items):
	debug(2,['Find Enums Etc.'],0)
	enums = findAll(enumetc, record)
	debug(2,['Enums Etc.'])
	tokens = []
	for i, (str, group, start, end) in enumerate(enums):
		str = stripBlanklines(rstrip(str))
		# Get rid of wayward comment tokens
		str = findcommenttoken.sub("", str)
		placeholder = placeHolder(group, i)
		debug(2,[str,78*'-'],0)
		items[placeholder] = str
		tokens.append((placeholder,start,end))
	if len(tokens) > 0:
		record = splice(record, tokens)
		debug(2,['After Replace Enums Etc.',record])    
	return record
#--------------------------------------------------------
# Find function bodies and empty them
#--------------------------------------------------------
def parseFunctionBodies(record, items):
	debug(4,['Find Body Candidates'],0)
	bodybnds = findBodies(record)
	debug(4,['Body Candidates',printAll(bodybnds)])
	skip = len(bodybnds)*[0]
	for i, (str, group, start, left, right, end) in enumerate(bodybnds):
		for j, (s, g, a, l, r, b) in enumerate(bodybnds):
			if (start < a) and (b < end):
				skip[j] = 1
	tokens = []
	debug(2,['Bodies'])
	for i, (str, group, start, left, right, end) in enumerate(bodybnds):
		if skip[i]: continue
		tokens.append(('{}',start,end))
		debug(2,[str,78*'-'],0)
	if len(tokens) > 0:
		record = splice(record, tokens)
		debug(2,['After Find Bodies',rstrip(record)])
	return record
#--------------------------------------------------------
#
# NB: We assume that any qualifier such as "const",
#     "const=0" lie on same line as the ")" that
#     ends the function signature.
#--------------------------------------------------------
goodname = re.compile('^(operator.*|[^=.]+)$',re.M)

# Must be a valid type
integer = '([-]|[+])?[0-9]+'
float   = '([-]|[+])?[0-9]*([0-9]\.|\.[0-9])[0-9]*[ ]?((e|E)([-]|[+])?[0-9]+)?'
number  = re.compile('%s|%s' % (integer, float),re.M)
funcall = re.compile('(?P<name>[^\(\)]+(?=\())',re.M)
def goodargs(args):
	for arg in args:
		if number.match(arg) <> None: return 0
	return 1

def parseFunctions(record, items):
	functions, record = findFunctions(__attribute__.sub("", record))

	tokens = []
	for i, (str, group, start, left, right, end) in enumerate(functions):

		# Replace newlines and tabs with a space

		rec = replace(str,"\n"," ")
		rec = replace(rec,"\t"," ")

		# Ignore rare case in which a method is of the type
		# name(B) method(...). Most likely its group will be
		# incorrectly given as "constructor"

		if group == 'constructor':

			fns = findComponents(funcall, rec,'(',')')

			# Count number of valid functions
			n = len(fns)
			for j, (s1, g1, a1, l1, r1, b1) in enumerate(fns):
				for j, (s, g, a, l, r, b) in enumerate(fns):
					if (a1 < a) and (b < b1):
						n -= 1
			if n > 1:
				inform('Ignoring:\n%s\n' % rec)
				group = 'group'
				retype,name,args,vars,qual = ('','','','','')

			# Also check to make sure this is not really a
			# callback. (See FCN in TMinuit.h for instance.)
			# If it is, we ignore it for now!

			if callback.search(rec) <> None:
				group = 'group'
				retype,name,args,vars,qual = ('','','','','')


		# Determine method type. If not recognized
		# the type remains "group" and will therefore
		# not be written out.

		####D
		#print "GROUP( %s ) REC( %s )" % (group, rec)

		if group <> 'group':

			retype, name, args, vars, qual = decodeMethodName(rec)

			####D
			#print "\tRETYPE( %s ) NAME( %s )" % (retype, name)

			# Ignore extern functions

			if find(retype, 'extern') > -1: continue

			# Make sure name is sensible

			if goodname.match(name) == None: continue
			####
			#print "\t\t\t\tGOOD"

			# Make sure arguments are sensible

			if not goodargs(args):
				warning("assuming\n%s\n\tis NOT a function" % rec)
				continue

			# Okay, let's see what kind of function we have

			if group == 'method':

				if find(name, 'operator') > -1:
					group = "operator"
					str  = replace(str,"PP", "()")
					name = replace(name,"PP", "()")

				elif (find(rec,'<') > -1) and (find(rec,'>') > -1):
					group = 'method2'
				else:
					group = 'method1'

			# Hack to take account of declarations of simple "throws"
			if find(rec, 'throwPP') > -1: rec = replace(rec, 'PP', '()')

			dstr = ["\tReturn: %s" % retype,
					"\tName:   %s" % name]
			dstr.append("\tArgs:   %s" % packArgs(args))
			dstr.append("\tQual:   %s" % qual)
			debug(1,dstr,0)

		# Update placeholder

		placeholder = placeHolder(group, i)            
		items[placeholder] = (rec,(retype,name,args,vars,qual))
		tokens.append((placeholder,start,end))

		debug(1,["\t\t\t\t\t%s" % placeholder, rec, 78*'-'],0)
	if len(tokens) > 0: record = splice(record, tokens)
	return record
#--------------------------------------------------------
# Find routines taking care not to be confused by
# implementation code.
#
#     Before parsing replace all operator xxx () by
#     operator xxxPP before finding functions, then
#     reverse operation, otherwise the heuristic rule
#     that looks for xxxxxx () will break on
#     operator xxx (). Likewise for throw().
#     Note: if throw is more complicated this code
#     will fail.
#--------------------------------------------------------
destfix = re.compile('(?<= )~[ ]*(?=\w+)',re.M)
def findFunctions(record):
	op = operatorPP.search(record)
	if op <> None:
		opname = op.group('name')
		if opname == None: opname = ''
		if opname <> '': opname = ' ' + opname
		record = operatorPP.sub("operator%sPP" % opname, record)
	#record = operatornewPP.sub("operator newPP", record)
	#record = operatorlongPP.sub("operator longPP", record)
	#record = operatordeletePP.sub("operator deletePP", record)
	record = throwPP.sub("throwPP", record)

	record = destfix.sub('~', record) # Fix tilde in destructor name

	debug(2,['Find Candidate Functions'],0)
	candidates = findCandidateFunctions(record)

	if VERBOSE: print "found functions"

	debug(3,['Candidate Functions',printAll(candidates)])

	keep = len(candidates)*[1]
	for i, (str, group, start, left, right, end) in enumerate(candidates):
		for j, (s, g, a, l, r, b) in enumerate(candidates):
			if (start < a) and (b < end):
				# This function is within signature so ignore
				keep[j] = 0

	debug(1,["Functions"])
	functions = []
	for i, (str, group, start, left, right, end) in enumerate(candidates):
		if not keep[i]: continue
		str = stripBlanklines(str)
		str = standardizeOpName(str)
		str = standardizeName(str)      

		# Find end of method declaration

		m = functionend.search(record[end:])
		fend = rstrip(m.group())
		end = end + len(fend)
		str = str + fend
		if str[len(str)-1] <> ";":
			str = str + ";"

		functions.append((str, group, start, left, right, end))
	return (functions, record)
#--------------------------------------------------------
def parseFriends(record, items):
	friendlies = findAll(friends, record)
	debug(2,['Friends'])
	tokens = []
	for i, (str, group, start, end) in enumerate(friendlies):
		str = stripBlanklines(rstrip(str))
		debug(2,[str,78*'-'],0)
		placeholder = placeHolder("friend", i)
		items[placeholder] = str
		tokens.append((placeholder,start,end))
	if len(tokens) > 0:
		record = splice(record, tokens)
	return record
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

	groups = (section, member)
	format = (len(groups)-1)*'%s|'+'%s'
	exp    = exp + format % groups
	regex  = re.compile(exp,re.M+re.S)
	return findAll(regex, record)
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
#---------------------------------------------------------------------------
def findHeader(file, includes):

	# Find header file

	filepath, headerpath = findFile(file, includes)

	if filepath == "":
		print "**Error** findHeader - Can't find %s" % file
		sys.exit(1)

	if headerpath == filepath:
		try:
			base, headerpath = splitfields(filepath,'include/',1)
		except:
			try:
				base, headerpath = splitfields(filepath,'inc/',1)
			except:
				try:
					base, headerpath = splitfields(filepath,'src/',1)
				except:
					pass
	return (filepath, headerpath)
#---------------------------------------------------------------------------
# Procedures to decode types
#---------------------------------------------------------------------------
cleantype   = re.compile(r'\bconst\b|\bclass\b|[*]|[&]|[;]')
def updateTypeList(typelist, name, ignoredtypes):
	name = strip(standardizeName(findReturnTypeQual.sub("", name)))
	if name <> "":
		# Remove possible default value
		name = stripDefaultValues(name)
		str  = strip(cleantype.sub("",name))
		if str in ignoredtypes: return
		typelist.add(name)
#---------------------------------------------------------------------------
# Strip value away from a defaulted argument. This beast is needed to
# handle complex template types with default arguments.
#---------------------------------------------------------------------------
trickyValue    = re.compile('.+(?P<delim>\(|\<)')
trickyValueMap = {'<':'>','(':')'}
whitespace     = re.compile('\s+',re.M)
stringUpToComma= re.compile('^[^,]+') # Anything but a comma
astring        = re.compile('^\"[^"]*\"')
def stripDefaultValues(name):
	t = splitfields(name,'=',1) # Split at first "=" sign
	if len(t) == 1: return name
	if len(t) == 2: return strip(t[0])

	# OK we need to work harder!

	name = whitespace.sub(' ', name)  # squeeze out most white space
	t = split(name,'=')

	index = 1
	while index < len(t):
		value = t[index]

		# Value could be
		# 1. a string
		# 2. a function
		# 3. a template
		# 4. a simple type, with possibly one or more namespace resolution
		#    operators (::)
		String = 0
		m = astring.match(value)
		if m <> None:
			String = 1
			value = m.group()
			value = re.sub('\*|\+|\(|\)','@',value) # To avoid regex problems
		else:
			m = trickyValue.match(value) # check for a function or a template
			if m <> None:
				x = m.group('delim')     # Get closing partner symbol
				value = findComponents(trickyValue, value, x,
									   trickyValueMap[x])[0][0]
			else:
				# Ok this is a simple value. String could have form:
				# "previous-val, next-arg". Strip away string up to
				# possible comma
				m = stringUpToComma.match(value)
				if m <> None:
					value = m.group()
				else:
					fatal("stripDefaultValues - "\
						  "Unable to decode argument: %s" % name)
		# strip away value
		value = strip(value)
		name  = re.sub(r'\s*=\s*%s\s*' % value, '', name)
		index += 1
	return name
#---------------------------------------------------------------------------
def updateTypemap(context, group, name, fullname, record):

	typemap = context['typemap']

	fullname = standardizeName(fullname)

	if   group in ['class', 'structclass']:
		key = name
		typemap[key] = (fullname, CLASSTYPE)

	elif group == 'enum':
		name, fields = decodeEnum(record)
		if name <> '':
			namen = fullname + '::' + name
			typemap[name] = (namen, ENUMTYPE)

		for key, value in fields:
			namen = fullname + '::' + key
			typemap[key] = (namen, ENUMTYPE)

	elif group == 'typedef':
		key, value = decodeTypedef(record)

		# If this translates to an enum, just qualify the name fully.

		if enumtype.match(value) <> None:
			namen = fullname
		else:
			namen = value

		typemap[key] = (namen, TYPEDEFTYPE)

	elif group == 'ftypedef':
		record = stripTypedef.sub("",record)
		key = name
		typemap[key] = (fullname, FTYPEDEFTYPE)

	elif group == 'member':
		key = name
		typemap[key] = (fullname, MEMBERTYPE)
#---------------------------------------------------------------------------
def updateClassList(classlist, name):
	if name <> "":
		classlist.add(name)
#---------------------------------------------------------------------------
# Check for implementations in header
#---------------------------------------------------------------------------
def implementation(name, classnamelist):
	for classname, basenames in classnamelist:
		prefix = '%s::' % classname
		if find(name, prefix) > -1:
			return 1
	return 0
#---------------------------------------------------------------------------
def decodeEnum(chdata):
	name = findEnumName.search(chdata).group('name')
	if name == None:
		name = ""
	else:
		name = strip(name)
	enumname = name

	body   = findInlineComments.sub("",chdata)
	i = find(body,'{')
	j = rfind(body,'}')

	fields = map(lambda x: split(strip(x),'='),
				 split(body[i+1:j],','))

	records = []
	for index, field in enumerate(fields):
		if len(field) == 2:
			name, value = field
		else:
			name = field[0]
			value= ""
		if name <> '':
			records.append((name, value))
	return (enumname, records)
#---------------------------------------------------------------------------
# Template Decoration:
#   <A, B, C, ...>
#---------------------------------------------------------------------------
tempclasspar = re.compile('typename *[a-zA-Z]\w*|'\
						  'class *[a-zA-Z]\w*')
striptypenameOrclass = re.compile('typename |class ')

def templateDecorationOLD(element):
	if element == None: return ''
	if type(element) == type(""):
		template = element
	else:
		if not element.attrib.has_key('template'): return ''
		template = element.attrib['template']
	s = replace(template,'\n', ' ')
	t = tempclasspar.findall(s)
	s = '<' + joinfields(t,', ') + '>'
	s = striptypenameOrclass.sub("",s)
	return s

def templateDecoration(element):
	if element == None: return ''
	if type(element) == type(""):
		template = element
	else:
		if not element.attrib.has_key('template'): return ''
		template = element.attrib['template']
	s = replace(template,'\n', ' ')

	# Create a dummy method name, so we can use decodeMethodName

	i = find(s, '<')
	j = rfind(s,'>')
	s = "TMP(%s)" % strip(s[i+1:j])
	retype, name, arglist, varlist, qual = decodeMethodName(s)
	s = '<'+joinfields(varlist,', ')+'>'
	return s
#---------------------------------------------------------------------------
# Stripped Template
# Template with default values stripped away
#   template<class A, class B, class C, ...>
#---------------------------------------------------------------------------
def strippedTemplate(name):

	# Create a dummy method name, so we can use decodeMethodName

	i = find(name, '<')
	j = rfind(name,'>')
	s = "TMP(%s)" % strip(name[i+1:j])
	retype, name, arglist, varlist, qual = decodeMethodName(s)
	arglist = map(lambda x: split(x,"=")[0], arglist)
	vars = map(lambda x, y: "%s %s" % (x,y), arglist, varlist)
	return "template<%s>" % joinfields(vars,', ')
#----------------------------------------------------------------------------
squeezeHeadername = re.compile(r'\b(src|interface|include|inc|source)\b/')
def standardizeHeadername(name):
	#name = squeezeHeadername.sub("",name)
	#name = replace(name,'/','')
	return nameonly(name)
#============================================================================
#============================================================================
# Main Program
#  Read header list
#  for each header in list
#     decode header
#     construct interface file
#============================================================================
#============================================================================
def main():

	global DEBUG, OUTDEBUG, OUTCLASSNAME, OUTMETHODCOM, OUTCLASSCOM, VERBOSE
	global CONTEXT, SILENT, NAMESPACELIST, STANDALONE

	#-------------------------------------------------
	# Decode command line
	#-------------------------------------------------
	# Defaults

	FORCE = 1                 # Set to 0 to not force re-building of file
	MACROS= ""
	INCLUDE = []
	INCLUDEPATHS = ""
	VERBOSE = 1
	SILENT  = 0
	STANDALONE = 0

	argv = sys.argv[1:]
	opts, args = getopt(argv, SHORTOPTIONS)

	for option, value in opts:
		if   option == "-d":
			DEBUG = atoi(value)
			OUTDEBUG = open(DEBUGFILE,"w")
			print "----DEBUG"
			
		elif option == "-D":
			MACROS = MACROS + " -D%s" % value

		elif option == "-h":
			help()
			sys.exit(0)

		elif option == "-c":
			OUTMETHODCOM = open("methodcomments.txt","w")
			OUTMETHODCOM.write("METHODCOMMENTS = {\n")

			OUTCLASSCOM = open("classcomments.txt","w")
			OUTCLASSCOM.write("CLASSCOMMENTS = {\n")

		elif option == "-I":
			INCLUDE.append(value)
			INCLUDEPATHS = INCLUDEPATHS + " -I%s" % value

		elif option == "-C":
			FORCE = 0

		elif option == "-v":
			print VERSION
			sys.exit(0)

		elif option == "-V":
			VERBOSE = 1

		elif option == "-S":
			SILENT = 1

		elif option == "-n":
			t = split(value,',')
			NAMESPACELIST = NAMESPACELIST + t

		elif option == "-s":
			STANDALONE = 1
			SILENT = 1

	# Make directories, if they do not let exist

	if not STANDALONE: os.system('mkdir -p logs; ')

	# Create command to scrub header of constructs that could cause
	# header2xml to fail

	STRIP = map(lambda x: "\\b%s\\b" % x, SCRUB)
	replaceClassComment  = len(CLASSCOMMENTS) > 0
	replaceMethodComment = len(METHODCOMMENTS) > 0
	cmd = joinfields(STRIP,"|") + '[^\n]*\n'
	scrub = re.compile(cmd, re.M)

	# Get list of headers to be converted
	# Note: filelist is the list of header files, *not* the files
	#       in src that each point to a headerfile.
	if len(args) > 0:
		filelist = args
	else:
		filelist = getHeaderList()

	# Open problems file. Delete at end if no problems found

	problemCount = 0
	PROBLEMS = None
	if not STANDALONE: PROBLEMS = open(PROBLEMSFILE,"a")

	xmlWriter = XMLWriter()

	#-------------------------------------------------
	# Loop over header files to be converted
	#-------------------------------------------------

	for index, file in enumerate(filelist):

		# Find header file
		# headerfilepath is full path
		# headerpath     is path relative to the search path

		headerfilepath, headerpath = findHeader(file, INCLUDE)
		headername = standardizeHeadername(headerpath)
		
		# Check if XML file exists.
		# If it does do nothing unless force switch (-F) is active

		xmlfile = "%s.xml" % headername

		if FORCE:
			pass
		else:
			if os.path.exists(xmlfile): continue

		#-------------------------------------------------
		# 1. Read header file into a single record.
		# 2. Parse header, that is, split the record into
		#    recognized constructs
		#-------------------------------------------------
		if not SILENT: print "%4d - %s" % (index+1, headerpath)

		try:
			headers = map(lambda x: rstrip(x),
						  open(headerfilepath,'r').readlines())
			header  = joinfields(headers,'\n')

			header, items = parseHeader(header, scrub, MACROS, INCLUDEPATHS)

			records = splitHeader(header)

		except ImConfused, e:
			if PROBLEMS:
				problemCount += 1
				PROBLEMS.write("CONFUSING:\t%s\n" % headerpath)
			continue

		#-------------------------------------------------        
		# Open xml writer. File is actually opened and
		# close in the "close" method. This way, if this
		# program fails, no xml file is written.
		#-------------------------------------------------        

		CONTEXT['headername'] = headername
		CONTEXT['headerpath'] = headerpath
		CONTEXT['headerfile'] = headerfilepath
		CONTEXT['module']     = MODULE
		CONTEXT['project']    = PROJECT
		NAMESPACE  = []

		xmlWriter.open(xmlfile, CONTEXT)

		# ------------------------------------------------------------------
		# Now loop over each group, that is, recognized construct.
		# Some groups only make sense within class scope so
		# distinguish between being within or outside class scope
		# Note: only one group is active at a time
		# ------------------------------------------------------------------

		inclass = 0
		noClassFound = 1 # Will be set to 0 if a class is found 
		Section = ""
		comment = ""
		previousGroup = ""
		classnameList = [] # Could have more than one class per file
		abstractClass = 0
		typelist = set()   # To keep track of all types within classes
		classlist= set()
		typemap  = {}
		ignoredtypes = ['static',
						'explicit',
						'inline',
						'const',
						'virtual',
						'friend']

		classContext = ClassContext(CONTEXT)

		debug(1,['Loop Over Records'])
		if VERBOSE: print "write out records"

		# Initialize class depth. Used to identify nested classes.

		classdepth = 0

		for irecord, rec in enumerate(records):            
			if irecord > 0:
				previousGroup = records[irecord-1][1]

			record, group, start, end = rec
			debug(1,[78*'-',"\t\t\t\t\t\t\t%s" % group],0)

			#-------------------------------------------------
			# Get actual record from items map
			#-------------------------------------------------
			key = strip(record)
			if items.has_key(key):
				record = items[key]
				if type(record) == type(()):
					record, extraRecord = record
				debug(1,["\t\t\t\t\t\t\t%s" % key],0)
			else:
				debug(1,[key],0)

			skip = 0   # Default: write out record

			record = stripBlanklines(record)

			# Restore some key class context variables
			# just in case we have switched context

			inclass = CONTEXT['inclass']
			section = CONTEXT['section']
			abstractClass,\
			regularClass,\
			specializedClass,\
			templateClass = classType(CONTEXT)

			if   group == "comment":
				comment = record    # Note comment, but defer writing out
				continue            # Must continue            

			elif group == "include":
				name = includeName(record)
				updateClassList(classlist, name)

			elif group == "forward":
				name = forwardName(record)
				record = replace(record,"class ","")
				record = strip(replace(record,";",""))
				updateClassList(classlist, name)

			elif group == "constant":
				record = strip(replace(record,";",""))

			elif group == "namespace":
				name = strip(namespaceName(record))
				if name != '': NAMESPACE.append(name)

			elif group == "endnamespace":
				if len(NAMESPACE) > 0: NAMESPACE.pop()
				record  = "}"

			elif group == "class" or group == "structclass":

				classContext.push(CONTEXT) # Push previous context onto stack

				CONTEXT['typemap'] = {}

				classdepth += 1

				inclass = 1
				noClassFound = 0
				abstractClass= 0
				section = ""
				classname, basenames, template = getClassname(record)
				classnameList.append((classname,basenames))

				# Determine class type

				regularClass = template == None                
				templateClass = not regularClass                
				if templateClass:
					# Could be a specialized template or an
					# instantiated template
					specializedClass = find(template,'<>') > -1
					if not specializedClass:
						n = find(classname,'<')
						if n > -1:
							# The name is a template
							# Make set of parameters from "template <....>"
							# as well as from the template decoration in
							# the name (that is, "<...>". If the first is
							# a subset of the second then we have another
							# kind of specialized template. See, for example,
							# IsPointer in TClass.
							tset = set(templateParameters(template))
							t = set(swords.findall(classname[n:]))
							specializedClass = tset.issubset(t)
				else:
					specializedClass = 0                

				ignoredtypes = ignoredtypes + templateParameters(template)

				if specializedClass:
					fullname = classname
				else:
					fullname = classname + templateDecoration(template)

				####D
				#print "FULLNAME( %s ) TEMPLATE( %s )" % (fullname, template)

				if strip(fullname) != '': NAMESPACE.append(fullname)
				fullyqualifiedname = joinfields(NAMESPACE,'::')

				# Store fullname. It will be written out
				CONTEXT['fullclassname'] = fullyqualifiedname
				updateTypeList(typelist, fullyqualifiedname, ignoredtypes)

				# Only for nested classes
				if classdepth > 1: 
					updateTypemap(CONTEXT,
								  group,
								  classname,
								  fullyqualifiedname,
								  record)

				#-------------------------------------------------        
				# Get name of class or its template (minus template
				# parameter). 
				#-------------------------------------------------        
				constructorname = constructorName(classname)

				for basename in basenames:
					t = split(basename)
					name = joinfields(t[1:])
					updateTypeList(typelist, name, ignoredtypes)

				if replaceClassComment:
					if CLASSCOMMENTS.has_key(classname):
						comment = stripBlanklines(CLASSCOMMENTS[classname])
						previousGroup = "comment" # NB!

				if OUTCLASSCOM:
					writeOutClassComment(classname, comment,",\n")

				CONTEXT['group'] = group
				CONTEXT['inclass'] = inclass
				CONTEXT['classname'] = classname
				CONTEXT['template'] = template
				CONTEXT['basenames'] = basenames
				CONTEXT['constructorname'] = constructorname
				CONTEXT['type'] = ' no_init'

				str = '\\b%(constructorname)s\\b\s*('\
					  '\((const )?\\b%(constructorname)s\\b|'\
					  '\(\\b%(constructorname)s\\b const)'\
					  '[^&]*&[ ]*\w*\)'%\
					  CONTEXT

				CONTEXT['copycon'] = (re.compile(str),str)

				setClassType(CONTEXT,
							 abstractClass,
							 regularClass,
							 specializedClass,
							 templateClass)
				debug(1,[classContext.show(CONTEXT)],0)
				debug(1,[classContext.show()],0)

			# ---------------------------------------------------
			# We are either within a class scope or we are not
			# ---------------------------------------------------            
			elif inclass:

				# ---------------------------------------------------
				# Apply additional processing according to group name
				# ---------------------------------------------------
				if   group == "endclass" or group == "endstructclass":

					classdepth -= 1

					NAMESPACE.pop()
					record = "};"

				elif group == "section":
					record = strip(replace(record,':',''))
					section= record
					CONTEXT['section'] = section # Update current context

				# ---------------------------------------------------
				# Ignore private or protected sections
				# ---------------------------------------------------
				elif section in ["private", "protected", "signals"]:
					skip = 1

					if group in METHOD:

						retype, name, args, vars, qual = extraRecord

						CONTEXT['argslist']  = extraRecord

						# Check for copy constructor
						if group == "constructor":

							regex, str = CONTEXT['copycon']
							if regex.search(record) <> None:
								group = "copyconstructor"
								CONTEXT['type'] = CONTEXT['type'] + \
												  ' %scopyctor' % section

							elif len(args) == 1 and args[0] == 'void':
								CONTEXT['type'] = CONTEXT['type'] + \
												  ' %sdefctor' % section
							else:
								CONTEXT['type'] = CONTEXT['type'] + \
												  ' %sctor' % section

						elif group == "destructor":
							CONTEXT['type'] = CONTEXT['type'] + \
										  ' %sdtor' % section

						elif group in ['method1','method2']:
							# Check for abstract class
							if not abstractClass:
								abstractClass = find(qual, '=') > -1
								if abstractClass:
									CONTEXT['type'] = 'abstract' + \
													  CONTEXT['type']

				# Okay, we are either in a public section or the preamble
				# section of a class

				elif group == "friend":
					pass

				elif group in METHOD:

					retype, name, args, vars, qual = extraRecord

					# This may be a template method
					m = istemplate.match(retype)
					if m <> None:
						template = m.group('template')
						ignoredtypes = ignoredtypes + \
									   templateParameters(template)

					CONTEXT['argslist']  = extraRecord

					# Check for abstract class
					if not abstractClass:
						abstractClass = find(qual, '=') > -1
						if abstractClass:
							CONTEXT['type'] = 'abstract' + CONTEXT['type']

					if not retype in ["virtual",
									  "inline",
									  "static"]:
						updateTypeList(typelist, retype, ignoredtypes)
					for arg in args:
						updateTypeList(typelist, arg, ignoredtypes)

					record = fixInlineComments(record)

					if replaceMethodComment or OUTMETHODCOM:
						signature = methodSignature(classname,record)
					else:
						signature = ""

					if replaceMethodComment:
						if METHODCOMMENTS.has_key(signature):
							comment=stripBlanklines(METHODCOMMENTS[signature])
							previousGroup = "comment"

					if OUTMETHODCOM:
						writeOutMethodComment(signature, comment,",\n")

					# Apply some special processing to methods

					if   group == "method2":
						pass

					elif group == "operator":

						# Set object to printable if it defines the function
						# operator<<
						if find(name, '<<') > -1:
							CONTEXT['type'] += " printable"

					elif group == "constructor":
						CONTEXT['type']=replace(CONTEXT['type'],' no_init','')

						regex, str = CONTEXT['copycon']
						if regex.search(record) <> None:
							group = "copyconstructor"

				elif group in MEMBER:
					record = fixInlineComments(record)

					if group == 'enum' or group == 'struct':
						m = findEnumStructType.search(record)
						if m <> None:
							name = strip(m.group('name'))
							updateTypeList(typelist, name, ignoredtypes)
							fullyqualifiedname = joinfields(NAMESPACE,'::')
							updateTypemap(CONTEXT,
										  group,
										  name,
										  fullyqualifiedname,
										  record)
						else:
							fatal("can't get type of enum or struct\n"
								  "\t\t%s" % record)

					elif group == 'typedef':
						t = decodeCallback(record)
						if t <> None:
							r, name, a, v, q = t
							updateTypeList(typelist, r, ignoredtypes)
							fullyqualifiedname = joinfields(NAMESPACE+[name],
															'::')
							updateTypeList(typelist,
										   fullyqualifiedname,
										   ignoredtypes)

							updateTypemap(CONTEXT,
										  "f"+group,
										  name,
										  fullyqualifiedname,
										  record)
							for arg in a:
								updateTypeList(typelist, arg, ignoredtypes)
						else:
							name, value = decodeTypedef(record)
							fullyqualifiedname = joinfields(NAMESPACE+[name],
															'::')
							updateTypemap(CONTEXT,
										  group,
										  name,
										  fullyqualifiedname,
										  record)

					elif group == 'typedefstruct':
						pass

					elif group == 'member':
						# Skip "using" statements
						if strip(record)[0:6] == 'using ':
							skip = 1
						else:
							record = splitfields(record,'//',1)[0]
							record = splitfields(record,'/*',1)[0]
							t = split(record)
							name = joinfields(t[:-1])
							tname= strip(strip(t[-1])[:-1])
							updateTypeList(typelist, name, ignoredtypes)

							fullyqualifiedname = joinfields(NAMESPACE+[tname],
															'::')
							updateTypemap(CONTEXT,
										  group,
										  tname,
										  fullyqualifiedname,
										  record)
				else:
					skip = 1

			elif group in OUTCLASS:
				record = fixInlineComments(record,0)

				# Skip functions containing class scope (A::)

				if group in ('operator','method1','method2'):
					retype, name, args, vars, qual = extraRecord

					####D
					#print "RETYPE( %s ) NAME( %s )" % (retype, name)
					if implementation(name, classnameList):
						skip = 1

						# Check if any of these are template functions
					elif istemplate.search(retype) <> None:
						skip = 1

					else:
						CONTEXT['argslist'] = extraRecord
						if group == 'operator':
							pass
						elif group == 'method1':
							# Make sure this is not a string declaration
							if len(args) == 1 and args[0][0] == '"':
								skip = 1
							else:
								group = 'function1'

						elif group == 'method2':
							# Make sure this is not a string declaration
							if len(args) == 1 and args[0][0] == '"':
								skip = 1
							else:
								group = 'function2'

				elif group == 'typedef':
					t = decodeCallback(record)
					if t <> None:
						r, n, a, v, q = t
						updateTypeList(typelist, r, ignoredtypes)
						for arg in a:
							updateTypeList(typelist, arg, ignoredtypes)
			else:
				skip = 1

			# Write out record if requested

			if skip:
				comment = ""
				continue
			else:
				xmlWriter.write(group, comment, record, CONTEXT, classdepth)
				comment = ""
				if group == "endclass" or group == "endstructclass":
					CONTEXT = classContext.pop() # Previous class context

		# END FOR

		# Write out xml description and close file

		xmlWriter.close(typelist, classlist)

		#---------------------------------------------------------
		# Validate XML file
		#---------------------------------------------------------
		if VERBOSE: print "\tvalidating xml file"
		errStr = validateXmlFile(xmlfile)
		if errStr <> "":
			print errStr
			invalidXML = 1
		else:
			invalidXML = 0
			
		# Update XmlMap.py

		if not invalidXML:			
			if not STANDALONE:
				updateXmlMap(xmlfile)

		# END-OF-FOR

		# Check for problems

		if PROBLEMS:

			if noClassFound:
				problemCount += 1
				PROBLEMS.write("NOCLASS:\t%s\n" % headerpath)

			elif invalidXML:
				problemCount += 1
				PROBLEMS.write("BADXML:\t%s\n" % headerpath)
				PROBLEMS.write("%s\n" % errStr)

	#---- END

	if OUTDEBUG:
		OUTDEBUG.close()

	if OUTMETHODCOM:
		writeOutMethodComment("NULL", "NULL","}\n")
		OUTMETHODCOM.close()

	if OUTCLASSCOM:
		writeOutClassComment("NULL", "NULL","}\n")
		OUTCLASSCOM.close()

	if PROBLEMS:
		PROBLEMS.close()
		os.system('sort --unique %s > %s1; mv %s1 %s' % (PROBLEMSFILE,
														 PROBLEMSFILE,
														 PROBLEMSFILE,
														 PROBLEMSFILE))
	if problemCount == 0:
		os.system("rm -rf %s" % PROBLEMSFILE)
#---------------------------------------------------------------------------
#---------------------------------------------------------------------------
if __name__ == "__main__":
	main()










