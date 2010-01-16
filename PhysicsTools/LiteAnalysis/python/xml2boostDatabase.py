#!/usr/bin/env python
#---------------------------------------------------------------------------
# File:        xml2boostDatabase.py
#
# Description: Create boost interface files from the XML files created with
#              header2xml.py. The XML files describe the structure and
#              contents of C++ header files in a format considerably easier
#              to read and understand than that produced by gccxml.
#              xml2boost.py is a valiant attempt to build Boost.Python
#              interface files automatically. The basic method is to use a
#              large number of heuristic rules. 
#
#              xml2boost makes heavy use of recursion, generally regarded as
#              a computationally inefficient technique. However, recursive 
#              algorithms are extremely powerful and lead to much simpler
#              code. Therefore, since speed is not an issue and given the
#              difficulty of what is being attempted, many of the
#              algorithms used are recursive.
#---------------------------------------------------------------------------
# Created:     17-Dec-2003 Harrison B. Prosper, CERN
# Updated:     08-Mar-2004 HBP Fix a few problems
#              09-Mar-2004 HBP Consolidate export files into a single file
#              15-Mar-2004 HBP
#              22-Mar-2004 HBP Change convertor method
#           04-14-May-2004 HBP at CERN. Numerous fixes
#              21-May-2004 HBP Add registration checks
#              10-Sep-2004 HBP Bug fixes
#              15-Sep-2004 HBP Fix optional argument problem with proxies
#              19-Sep-2004 HBP Fix updateStruct
#              22-Sep-2004 HBP Use TypesDB
#              19-Oct-2004 HBP Improve handling of callbacks
#              18-Nov-2004 HBP Swap src <-> interface
#              19-Nov-2004 HBP Handle templated functions
#              08-Dec-2004 HBP Fix iterator translation bug
#              10-Apr-2005 HBP Complete XML database cache
#              01-May-2005 HBP Adding more comments (at 30,000 feet!)
#              14-May-2005 HBP Add automatic scanning of global typedefs
#              21-May-2005 HBP Fix translation
#                              Change default to no holder class
#              22-May-2005 HBP Go back to previous default!
#              28-May-2005 HBP split xml2boost.py into xml2boostDatabase.py
#                              and xml2boost.py
#              19-Aug-2005 HBP fix findElements (can now search structs)
#              17-Oct-2005 HBP Assume this is run from directory into which
#                          export functions reside
#              28-Oct-2005 HBP Change simpletype to be simple types only!
#              22-Jan-2005 HBP Fix simpletype to handle UInt_t
#                              Fix search for header
#              05-Mar-2006 HBP Use isCkeywordNative in type translation
#              29-Apr-2009 HBP change opaque to opaque_
#$Revision: 1.23 $
#---------------------------------------------------------------------------
import os, sys, re, posixpath
from sets   import Set
from time   import *
from string import *
from glob   import glob
from getopt import getopt
from pprint import PrettyPrinter
from elementtree.ElementTree import ElementTree, XML
#---------------------------------------------------------------------------
from boostlib import *
from header2xml import \
     findComponents, \
     findHeader, \
     findClassElements, \
     findTypedefElements, \
     listNamespaces, \
     findAll,\
     findFile, \
     templateDecoration, \
     getHeaderList, \
     ocomment, \
     classType, \
     standardizeHeadername, \
     callback, \
     callback2, \
     number, \
     istemplate, \
     isspecializedtemplate, \
     swords, \
     astring, \
     stripDefaultValues, \
     strippedTemplate, \
     templatepar, \
     templateParList, \
     cleantype, \
     unfoldTypedef, TYPEDEFTYPE, FTYPEDEFTYPE, \
     getProject, \
     enumtype
#---------------------------------------------------------------------------
pp = PrettyPrinter()
def fatal(s):
    print "** Error ** %s" %s
    print "\tgoodbye!"
    sys.exit(1)
#---------------------------------------------------------------------------
# Constants
#---------------------------------------------------------------------------
VERSION='v1.25'    
day, month, daynum, hour, year = split(ctime(time()))
date='%2s-%3s-%4s' % (daynum,month,year)
#---------------------------------------------------------------------------
# Determine module etc.
# Structure of area:
#       <PROJECT>/src/<PROJECT>/[PACKAGE]/<MODULE>
#                                                 /include
#                                                 /python
#                                                 /src
#                                                 /test
#                                                 /tmp
#       <PROJECT>/lib/<PROJECT>/[PACKAGE]
#       <PROJECT>/config
#---------------------------------------------------------------------------
BASE, PWD, PROJECT, PACKAGE, MODULE, PROJECTPATH, XMLMAP = getProject()
#---------------------------------------------------------------------------
USAGE='''
Usage:
  xml2boost [-I<include-path>]
            [-D<macro>]
            [-d<number>]
            [-h]
            [-v] version
            [-w] issue warnings
            [-m<"export-files">] create <module>.py
            [-M]<directory> create <module>.py
            [-c] create only if export file does not exist
            [-t] Instantiate templates
            [-p] Print tree
            [-S] Show classes and typemaps
            [-s] standalone mode

            [--mapvectortolist="function"]
            [--defineargtype="class|'',function, arg, argtype"]
            
            [xmlfile [xmlfile ..] ]
'''

SHORTOPTIONS='hvwctpsSD:I:d:m:M:r:'
LONGOPTIONS = ['mapvectortolist=',
               'defineargtype=']

SRCDIR='.'          # Directory for export_<headername>.cpp files
INTDIR='.'          # Directory for export_<headername>.hpp files
TMPDIR='.'          # Directory for temporary files
XMLDIR='../xml'     # Directory for <headername>.xml files
DOCDIR='../doc'     # Directory for documents

MAXARGS=15          # Maximum number of arguments allowed by Boost.Python

TAB0=''
TAB1=2*' '
TAB2=4*' '

STANDALONE   = 0

TEMPLATEFILE = 'instantiate.py'
TEMPLATEIMPORTFILE = 'ttypes.py'

# Various switches for debugging

DEBUG=0
OUTTEMPLATE=0
WARN=0
PRINT=0

SHOW=os.environ.has_key('XML2BSHOW') and \
      os.environ['XML2BSHOW'] != ''

SHOWTRANS=os.environ.has_key('XML2BSHOWTRANS') and \
           os.environ['XML2BSHOWTRANS'] != ''

SHOWHEADER=os.environ.has_key('XML2BSHOWHEADER') and \
            os.environ['XML2BSHOWHEADER'] != ''

SHOWCLASS=os.environ.has_key('XML2BSHOWCLASS') and \
            os.environ['XML2BSHOWCLASS'] != ''

SHOWTYPEMAP=os.environ.has_key('XML2BSTYPEMAP') and \
            os.environ['XML2BSHOWTYPEMAP'] != ''

SHOWTEMPLATE=os.environ.has_key('XML2BSHOWTEMPLATE') and \
            os.environ['XML2BSHOWTEMPLATE'] != ''

SHOWINCLUDE=os.environ.has_key('XML2BSHOWINCLUDE') and \
            os.environ['XML2BSHOWINCLUDE'] != ''

SHOWBASE=0
#---------------------------------------------------------------------------
# This map is used to build strings and keep track of
# context information such as current class name etc.
#---------------------------------------------------------------------------
CONTEXT ={'date': date,
          'time': hour,          
          'project': PROJECT,
          'package': PACKAGE,
          'module' : MODULE,
          'srcdir' : SRCDIR,
          'xmldir' : XMLDIR,
          'tmpdir' : TMPDIR,
          'docdir' : DOCDIR,
          'ctypesdir' : "%s/stl/src/ttypes/python" % BASE,
          'ttypesdir' : "%s/src/ttypes/python" % PROJECTPATH,
          'intdir' : INTDIR,
          'projectpath': PROJECTPATH,
          'version': VERSION,
          'elements' : [],
          'tab1': TAB1,
          'tab2': TAB2,
          'opaque': 'opaque_',
          'depends': [],
          'dependencies': [''], # NB: '' is important! (See main program)
          'otherdependencies': [''] # NB: '' is important! (See main program)
          } 

INCLUDE = []
MACRO = []

# Maximum number of characters per line before forcing a newline

MAXWIDTH=75

# Type codes

FUNCTION=1
INLINE=1
CLASSTYPE=1
FUNCTYPE=2
ENUMTYPE=3
TYPEDEFTYPE=4
EXPORT_FUNCTION=0

# I/O types

INPUT  = 1
OUTPUT = 2
INOUT  = 3
BYVALUE= 4

# Ordinal value of ouput elements of analyzeArgument

K_TYPEMAPPED = 0
K_ARGTYPE    = 1
K_OLDARG     = 2
K_ARG        = 3
K_CASTARG    = 4
K_VAL        = 5
K_TTYPE      = 6
K_CBNAME     = 7
K_IOTYPE     = 8
K_IOSIZE     = 9
K_BYVALUE    = 10
K_VAR        = 11
K_OLDVAR     = 12

K_TOTAL      = 13

C_SKIP       = 3

ROUTINES=Set(['method1',
              'method2',
              'function1',
              'function2',
              'operator',
              'constructor'])

CONTAINERS=['header','namespace','class','section']

IGNORE_OPERATORS=['>>', '<<']

STANDARD_TYPES=['void',
                'explicit',
                'FILE',
                'va_list',
                'timeval',
                'enum',
                'type_info',
                'typeid',
                'bitset',
                'complex',
                'deque',
                'fstream',
                'ofstream',
                'ifstream',
                'hash_map',
                'hash_set',
                'istream',
                'ostream',
                'stringstream',
                'stack',
                'string',
                'iterator',
                'const_iterator']

s = '\\b('+joinfields(STANDARD_TYPES,'|')+')\\b'
standardtypes = re.compile(s)

IGNORED_TYPES = STANDARD_TYPES + ['template',
                                  'file',
                                  'pyobject',
                                  'vector',
                                  'map',
                                  'pair',
                                  'list']
s = '\\b('+joinfields(IGNORED_TYPES,'|')+')\\b'
ignoredtypes  = re.compile(s)
s = '\\b('+joinfields(STANDARD_TYPES[:-2]+\
                      ['vector','map','pair','list'],'|')+')\\b'
ignoredtypes2 = re.compile(s)

OTHER_TYPES = ['class',
               'const',
               'void',
               'std',
               'template',
               'typename',
               'false',
               'true',
               'virtual',
               '_t',
               '[.][.][.]',
               '[0-9]+']
s = '\\b('+joinfields(OTHER_TYPES,'|')+')\\b'
isOtherType = re.compile(s)

CKEYWORDS = ['int',
             'float',
             'double',
             'long',
             'bool',
             'char',
             'short',
             'void',
             'const',
             'class',
             'enum',
             'FILE',             
             'template',
             'typename',
             'false',
             'true',
             'virtual',
             'explicit',
             '\.\.\.',
             '[0-9]+']
s = '\\b('+joinfields(CKEYWORDS,'|')+')\\b'
isCkeywordNative = re.compile(s)

CKEYWORDS += ['u?int(8|16|32)?(_t)?',
              'U?Int(8|16|32)?(_t)?',
              'Bool_t',
              'U?Long_t']

isCkeyword = re.compile(s)


STL_TYPES=['vector',
           'list',
           'pair',
           'map',
           'set',
           'type_info',
           'bitset',
           'complex',
           'deque',           
           'fstream',
           'ifstream',
           'ofstream',
           'hash_map',
           'hash_set',
           'iterator',
           'const_iterator',
           'istream',
           'ostream',
           'stringstream',
           'stack',
           'string']
t = joinfields(STL_TYPES,'|')
s = '\\b('+t+')\\b'
stltype = re.compile(s)

s = '(?<!std::)\\b(%s)\\b' % t
badstl = re.compile(s)

STL=['vector']
STL_VECTOR_TYPES=['vint','vfloat','vdouble','vstring',
                  'vvint','vvfloat','vvdouble']

t = joinfields(STL_VECTOR_TYPES,'|')
s = '\\b('+t+')\\b'
stl_vtype = re.compile(s)

ITERABLE_TYPES=['vector',
                'list',
                'map',
                'set',
                'hash_map',
                'hash_set',
                'stack']
t = joinfields(ITERABLE_TYPES,'|')
s = '\\b('+t+')\\b'
iterabletype = re.compile(s)

TAB=2*' '
NTAB=len(TAB)

# A bit-field has format:
# <type> : <identifier>

isBitField = re.compile('[\w_ ]+ *: *[\w_]+')

#===========================================================================
# Notes:
# 1. Use named groups (?P<groupname>....) so that we can retrieve matched
#    strings by group name.
#
# 2. Use lookaheads (?=...), (?!...) and lookbehinds (?<=...), (?<!...)
#    to simplify regexes and make matches more robust. Note: lookaheads and
#    lookbehinds as zero width matches, that is, they do not consume
#    characters.
#===========================================================================

# Return templated identifiers

templates = re.compile('(?P<templates>\s*\w+(::)?\w+<)')

# Return operators

operator  = re.compile('(?P<operator>^' \
                       '(?P<return>.+?(?=operator))' \
                       'operator' \
                       '(?P<name>.+?(?=\(.+?\)))' \
                       '(?P<args>\(.+?(?=\))\)))',re.M)

# To check for simple types (NB: must use match(..))

simpletype = re.compile('(?P<name>(const\s+)?(unsigned\s+)?' \
                        '\\b('
                        'float|double|'\
                        'bool|char|short|'
                        '(long )?int|(long )?long|'\
                        'size_t|'
                        'Ssiz_t|'
                        '(i|I)nt(8|16|32)?_t|'
                        '(ui|UI)nt(8|16|32)?_t)\\b'\
                        '\s*([*][*]?|[&])?( const)?)')

simplectype= re.compile('(?P<name>(const\s+)?(unsigned\s+)?' \
                        '\\b('
                        'float|double|bool|void|'\
                        'char|short|'
                        '(long )?int|(long )?long|'\
                        'size_t|'
                        'Ssiz_t|'
                        '(i|I)nt(8|16|32)?_t|'
                        '(ui|UI)nt(8|16|32)?_t)\\b'\
                        '\s*([*][*]?|[&])?( const)?)')

strictlysimplectype= re.compile('(?P<name>(const\s+)?(unsigned\s+)?' \
                                '\\b('
                                'float|double|bool|void|'\
                                'char|short|'
                                '(long )?int|(long )?long|'\
                                'size_t)\\b'
                                '\s*([*][*]?|[&])?( const)?)')

intctype  = re.compile('(?P<name>(const\s+)?(?P<type>(unsigned\s+)?' \
                       '\\b('
                       'size_t|'
                       'ssiz_t|'
                       '(i|I)nt(8|16|32)?_t|'
                       '(ui|UI)nt(8|16|32)?_t|'
                       'short|(long )?int|'
                       '(long )?long|bool)\\b)'
                       '\s*([*][*]?|[&])?( const)?)')

longctype  = re.compile('(?P<name>(const\s+)?(?P<type>(unsigned\s+)?' \
                        '\\b(long )?long\\b)\s*([*][*]?|[&])?)')

floatctype = re.compile('(?P<name>(const\s+)?' \
                        '(?P<type>\\b(float|double)\\b)\s*([*][*]?|[&])?)')

numctype   = re.compile('(?P<name>\\b(unsigned )?'
                        '('
                        'size_t|'
                        'Ssiz_t|'
                        '(i|I)nt(8|16|32)?_t|'
                        '(ui|UI)nt(8|16|32)?_t|'
                        'short|(long )?int|'
                        '(long )?long|float|double|bool)\\b)')


misctype= re.compile('(?P<name>\\b(unsigned|const|FILE)\\b)')
                       
array1d = re.compile(r'(?<=\w)\s*(?P<name>\[(\*|[1-9]+|)\])(?!=\[)')
array1d = re.compile(r'(?<=\w)\s*(?P<name>\[(\*|[1-9]+|\w+|)\])(?!=\[)')

array2d = re.compile(r'(?<=\w)\s*(?P<name>\[(\*|[1-9]*|\w+|)\]'\
                     '\[(\*|[1-9]+|\w+|)\])(?!=\[)')

# To check for vector types

vectortype= re.compile('^\s*(?P<name>(const\s+)?(std::)?'\
                       'vector<(?P<type>.+)>(\s*[&])?)$')

vtype     = re.compile('^\s*(?P<name>(const\s+)?' \
                       'v(?P<type>string|double|int|float'\
                       '|vdouble|vint|vfloat)'\
                       '(\s*[&])?)$')

stliteratortype= re.compile('(?P<name>^\s*(const\s+)?(std::)?'\
                            'vector|map|pair|list<(?P<type>.+)>'\
                            '(?=::iterator|const_iterator)(\s*[&])?)')

stlattribtype = '(?P<name>\\b(vector|map|pair|list)\s*<.+>::%s\\b)'

# To check for iterator types and references

iteratortype = re.compile(r'(?P<name>\biterator\b)')
const_iteratortype = re.compile(r'(?P<name>\bconst_iterator\b)')

referencetype = re.compile(r'(?P<name>\breference\b|.+[&]$)')
const_referencetype = re.compile(r'(?P<name>\bconst_reference\b|'\
                                 '\bconst\b .+[&]$|.+\bconst\b *[&]$)')

const_type = re.compile(r'(?P<name>\bconst\b .+$|.+\bconst\b *$)')

cstringtype = re.compile(r'(?P<name>^(const )?char *[*]$)')
pointertype = re.compile(r'(?P<name>.+\*( +const)?$)')
const_pointertype = re.compile(r'(?P<name>const .+[*]$|.+const[ ]*[*]$)')
#const_pointertype = re.compile(r'(?P<name>.+[*] +const$)')
                                 
# To check for template types (NB: must use search(..))

templatetype= re.compile('(?P<name>\w[^<]*<[^>]+>)')

opaquetype = re.compile(r'\b%(opaque)s\b' % CONTEXT)

voidtype   = re.compile(r'\bvoid\b')

# To check for reference type (NB: must use searc(..))

reference = re.compile('(?P<type>([&]$))')
constreference = re.compile('(?P<type> const([&]$))')

# To check for pointer type (NB: must use search(..))

pointer   = re.compile('(?P<type>((\*|\])( const)?$))')
ppointer  = re.compile('(?P<type>(\*\*( const)?$))')

# To clean up return types

stripreturn = re.compile('(friend|virtual|inline|static|explicit)\s*',re.M)
stripsome = re.compile('(friend|inline|virtual|explicit)\s*',re.M)

# To check for readonly parameters (but make sure we ignore const char*)

readonly  = re.compile(r'\bconst\b(?! char\*)')

# Return enum value and name

enumvalue = re.compile(r'\b(?P<name>[A-Za-z_]\w*)(?=\s*([=]|,|\}))',re.M)
enumname  = re.compile(r'(?<=enum)\s+(?P<name>\w+)?\s*\{',re.M)

# Get operator name

opname    = re.compile(r'(?<=operator)\s*'
                       '(?P<name>(\+=|-=|\*\=|/=|<<|<=|<|>=|>(?!>)|'\
                       '\+\+|\+|--|-|\*|/|\[\]|==|!=|!))',
                       re.M)

staticWord = re.compile(r'\bstatic\b')

constWord  = re.compile(r'\bconst\b')

friendWord = re.compile(r'\bfriend\b')

underscore_t = re.compile(r'\w+_t\b')

isVirtual = re.compile(r'\bvirtual\b')
isStatic  = re.compile(r'\bstatic\b')
templatetype= re.compile('(?P<name>\w[^<]*<[^>]+>)')
striptpar = re.compile('\<.+\>')

# To get rid of inline and explicit keywords

stripret = re.compile('(?P<type>inline |explicit )',re.M)

#===========================================================================
# Utility Functions/Classes
#===========================================================================
class Logger:

    def __init__(self):
        os.system('mkdir -p logs')
        logfile  = 'logs/export.log'
        self.log = open(logfile,"w")
        self.log.write("%s\n\n" % ctime(time()))

    def write(self, str):
        self.log.write(str)

    def close(self, headername):
        self.log.close()
        os.system("mv logs/export.log logs/%s.log" % headername)
        
    def __del__(self):
        pass

# Instantiate a logger

LOG = Logger()

def log(name,s):
    global LOG
    if name != '': name + ': '
    if LOG <> None: LOG.write("%s%s\n\n" % (name, s))

def warning(s):
    global WARN, CONTEXT
    if CONTEXT.has_key('classname'):
        c = CONTEXT['classname']
    else:
        c = ''
    s = "**Warning** %s" % s
    if WARN:
        print "\t%s",s
    else:
        s = '\n%s' % s
    log(c,s)
    
def inform(s):
    print "\t** %s" %s
    global CONTEXT
    if CONTEXT.has_key('classname'):
        c = CONTEXT['classname']
    else:
        c = ''
    s = '\n** %s' % s
    log(c,s)    
              
def usage():
    print USAGE
    sys.exit(0)

def write(s, tab=0):
    return "%s%s\n" % (tab*' ',s)

# To align function arguments.

def align(s, delim='(',first=0):
    m = find(s,delim)
    if m < 0: return ''
    lines = split(s,'\n')
    if first:
        for line in lines:
            k = find(line, delim) + 1
            if k > 0: return k*' '
    else:
        k = find(lines.pop(), delim) + 1
        return k*' '

def tabright(s, n=0):
    global CONTEXT
    tab = (len(CONTEXT['classdepth'])-1+n)*'  '
    return joinfields(map(lambda x: '%s%s' % (tab,x), split(s,'\n')),'\n')
    
def debug(code,records,hasTitle=1):
    if (DEBUG >= code):
        if hasTitle:
            LOG.write("\n\t<==== %s ===>\n" % rstrip(records[0]))
            if len(records) > 1:
                records = records[1:]
            else:
                records = []        
        for record in records:
            if code == 0:
                print rstrip(record)
            else:
                LOG.write(rstrip(record)+"\n")
#---------------------------------------------------------------------------
# Update typemap given a class element.
#---------------------------------------------------------------------------
def updatetypeMap(typemap, element):
    for e in element.findall('typemap'):
        key =  e.attrib['name']
        value = e.attrib['value']
        if typemap.has_key(key): continue
        typemap[key] = value
#---------------------------------------------------------------------------
# Function returns true if type is to be ignored.
#---------------------------------------------------------------------------
t = []
for x in CKEYWORDS:
    unique(t, x)
for x in STL_TYPES:
    unique(t, x)
for x in STANDARD_TYPES:
    unique(t, x)

t.append('const')
t.append('unsigned')
t.append('long')

s = joinfields(t,'|')
s = '\\b('+s+')\\b'
ignorethis = re.compile(s)
def ignorethisType(name):
    return ignorethis.match(name) <> None
#===========================================================================
# This class models a locally cached database of XML objects. The success
# of xml2boost hinges on the completeness and accuracy of this database.
# In particular, this database, which is built from existing XML files, is
# used to effect type translation to fully qualified types and to perform
# recursive unfolding of typedefs. It is also used to include the correct
# headers in the interface files, including those for forwarded classes.
# One critical item is the map XmlMap, which is a map between a class-name
# and the XML file in which the class is described. Note: XmlMap is updated
# by header2xml, on the fly, as new XML class descriptions are created. One
# reason why xml2boost could fail is if a class is needed for which no XML
# description exists in XmlMap. The solution is to update XmlMap using
# header2xml.
#
# IMPORTANT:
# We make crucial use of Python's reference model, namely, an object
# has unique identity and multiple references can be made to it. It is
# crucial that we have only one instance of each header and each class
# element so that whenever each object is referenced, we are sure that we
# reference the same object.
#===========================================================================
class Database:

    def __init__(self, context):
        self.context = context
        try:
            from XmlMap import XmlMap
        except:
            fatal("Unable to import XmlMap")
            
        self.xmlmap = XmlMap
        
        try:
            from HeaderMap import HeaderMap
            self.headermap = HeaderMap
        except:
            self.headermap = {}
        
            
        self.inform        = []
        self.warning       = []
        self.INCLUDE       = []
        self.MACRO         = []
        self.classelements = []  # All class elements
        self.headerelements= []  # All header elements

        self.typeDB        = {}  # Potential class types
        self.classDB       = {}  # Class element database
        self.headerDB      = {}  # Header element database
        self.namespaceDB   = {'std':1}
        self.callbackDB    = {}
        self.includeDB     = {}
        self.identifierDB  = {}
        self.globaltypemap = {}        
        self.typedefMap    = {}

    def __del__(self):
        pass

    #-----------------------------------------------------------------
    # Populate the database.
    # 1. Recursively scan current header for all headers on which it
    #    depends. But if the current heaader contains forwarded classes,
    #    limit the set of headers by scanning only these forwards,
    #    recursively; that is, if a forwarded "header" contains forwarded
    #    classes, do not scan the latter. If required, we can always
    #    relax this requirement.
    #    
    # 2. For each header, include all classes it contains.
    #
    #-----------------------------------------------------------------
    def commit(self, headerelement):

        #-----------------------------------------------------------------
        # Recursively scan for headers
        #-----------------------------------------------------------------
        self.__loadheaderDB(headerelement)
        
        #-----------------------------------------------------------------
        # Populate class database with class elements, given list of
        # headers.
        #-----------------------------------------------------------------
        for element in self.headerelements:
            self.__loadclassDB(element)

        #-----------------------------------------------------------------
        # Unfold global typedefs. That is, translate typedefs, recursively,
        # back to their underlying types. We do this so we can more
        # readily determine whether a type is a pointer or a reference
        # to something simple or complicated.
        #-----------------------------------------------------------------
        self.__unfoldtypedefs()
        
        #-----------------------------------------------------------------
        # For each header element, update typemaps of classes therein
        #-----------------------------------------------------------------
        for element in self.headerelements:
            self.__updateclassTypemaps(element)

        #-----------------------------------------------------------------
        # For each class element, update list of identifiers
        #-----------------------------------------------------------------
        for element in self.classelements:
            self.__checkforIterators(element)
            self.__updateclassIdentifiers(element)

        #-----------------------------------------------------------------
        # Add types in current header to typeDB
        #-----------------------------------------------------------------
        self.__loadtypeDB(headerelement)

        #-----------------------------------------------------------------
        # Update the list of includes using contents of typeDB
        #-----------------------------------------------------------------
        self.__updateincludes(headerelement)

        if SHOW: self.show()
    #-----------------------------------------------------------------        
    def forwardname(self, name):
        name = strip(name)
        # Check: This may be of the form template<...> name
        if name[0:8] == 'template': name = split(name).pop()        
        return name
    #-----------------------------------------------------------------
    def headername(self, element):
        name = element.attrib['name']
        return standardizeHeadername(name)
    #-----------------------------------------------------------------
    # Add type to list of types and update the databases accordingly.
    #-----------------------------------------------------------------
    def add(self, name):

        typenames = swords.findall(name)
        for typename in typenames:

            if ignorethisType(typename): continue

            # NB: skip known namespaces
            if self.namespaceDB.has_key(typename): continue

            # Ignore strings
            if typename[0] == '"': continue
            
            # If this is an iterator, assume it is part of another
            # class.
            if find(typename, 'iterator') > -1: continue

            # NB: if class not in database then add it
            if not self.classDB.has_key(typename):

                h = self.headerElement(typename)
                if h == None:
                    warning("No XML description for %s" % typename)
                    continue

                self.__updateheaderDB(h)
                c = self.__classElement(typename, h)
                if c == None: continue
                self.__updateclassDB(c, h)

            if not self.classDB.has_key(typename):
                warning("No XML description for %s" % typename)
                continue
            
            h = self.classDB[typename].attrib['header']
            include = h.attrib['name']
            name = nameonly(include)
            if self.includeDB.has_key(name): continue
            
            if SHOWINCLUDE:
                print "TYPENAME( %s ) ADD( %s )" % (name, include)
            
            self.includeDB[name] = include
            
    #-----------------------------------------------------------------
    # Add include for given type. Again, in the spirit of this
    # program, we rely upon a sequence of heuristic rules.
    #-----------------------------------------------------------------
    def addInclude(self, name, where=''):

        namespaces = []
        
        # Skip if header already included
            
        if self.includeDB.has_key(name): return namespaces

        # Skip known namespaces
        
        if self.namespaceDB.has_key(name): return namespaces

        typenames = swords2.findall(name)
        
        for index, typename in enumerate(typenames):

            # Skip strings
            
            if typename[0] == '"': continue

            # Skip words such as "(*word)"

            if typename[0] == '(': continue
            
            # Skip if unlikely to be a class
           
            if ignorethisType(typename): continue

            # Skip if header already included
            
            if self.includeDB.has_key(typename): continue

            # Skip known namespaces

            if self.namespaceDB.has_key(typename): continue
            
            # Skip if we have already included the header for class A
            # and if the current identifier B forms a known identifier
            # of the form A::B.

            if index > 0:
                classname = typenames[index-1]
                if self.includeDB.has_key(classname):
                    namen = '%s::%s' % (classname, typename)
                    if self.identifierDB.has_key(namen): return namespaces
           
            # Ok. We need to continue our search
            
            # Skip if this is an iterator. We assume it is part of
            # another class.
            if find(typename, 'iterator') > -1: continue

            # Maybe header is in headerDB, if so add an entry to includeDB

            if self.headerDB.has_key(typename):
                h = self.headerDB[typename]
                include = h.attrib['name']
                self.includeDB[typename] = include

                if SHOWINCLUDE:
                    print "TYPENAME( %s ) headerDB( %s )%s" % \
                          (typename, include, where)

                tmp = []
                listNamespaces(h, tmp)
                namespaces += tmp

            else:
                # Header not yet loaded. If an XML description exists
                # for it, then load it into memory.
                
                h = self.headerElement(typename)
                if h <> None:
                    self.__updateheaderDB(h)
                    include = h.attrib['name']
                    self.includeDB[typename] = include

                    if SHOWINCLUDE:
                        print "TYPENAME( %s ) XmlMap( %s )" % \
                              (typename, include), where
                    tmp = []
                    listNamespaces(h, tmp)
                    namespaces += tmp
                else:
                    # No XML description exists for typename, so let's
                    # scan the list of include paths for a possible header

                    self.__searchforheader(typename, where)

        return namespaces
    #-----------------------------------------------------------------        
    def elements(self, name):
        h = self.headerElement(name)
        if h == None: return (None, None)
        c = self.__classElement(name, h)
        if c == None: return (None, None)
        return (c, h)
    #-----------------------------------------------------------------
    # Show classes and typemaps in database
    #-----------------------------------------------------------------    
    def show(self, what='all', name=''):
        if what in ['classes','all']:
            print "\nClasses"
            self.classDB.keys().sort()
            for name in self.classDB.keys():
                c = self.classDB[name]
                print "\t%-24s\t%s\t%s" % (c.attrib['name'],
                                           c.attrib['project'],
                                           c.attrib['module'])
            print "\nTypes"
            self.typeDB.keys().sort()
            for (key,value) in self.typeDB.items():
                print "\t%-24s\t%-24s" % (key, value)

            print "\nClasses: detail"
            for name in self.classDB.keys():
                c = self.classDB[name]
                print "\t%-40s" % c.attrib['name']

                if what in ['basenames', 'all']:
                    print '\t  Basenames', c.attrib['basenames']

                if what in ['parents', 'all']:
                    print '\t  Parents', c.attrib['parents']

                if what in ['children', 'all']:
                    print '\t  Children', c.attrib['children']

                if what in ['siblings', 'all']:
                    print '\t  Siblings', c.attrib['siblings']

                if what in ['identifiers', 'all']:
                    print '\t  Identifiers', c.attrib['identifiers'].keys()

                if what in ['typemap', 'all']:
                    print '\t  Typemaps'
                    for key, value in c.attrib['TypeMap'].items():
                        print "\t\t%s\t%s" % (key,value)

            if what in ['globaltypemap', 'all']:
                print '\nGlobal Typemaps'
                for key, value in self.globaltypemap.items():
                    print "\t%-20s%s" % (key,value)                
    #-----------------------------------------------------------------
    # Query database
    #-----------------------------------------------------------------    
    def query(self, key, name=''):
        if key == 'includes':
            try:
                return self.includeDB
            except:
                return None

        elif key == 'globaltypemap':
            try:
                return self.globaltypemap
            except:
                return None            

        try:
            if not self.classDB.has_key(name): return None
            element = self.classDB[name]
            
        except:
            element, headerelement = self.elements(name)
            if headerelement == None: return None

        headerelement = element.attrib['header']
        typemap       = element.attrib['TypeMap']
        basenames     = element.attrib['basenames']
        children      = element.attrib['children']
        parents       = element.attrib['parents']
            
        if   key == 'project':
            return element.attrib['project']

        elif key == 'module':
            return element.attrib['module']

        elif key == 'fullname':
            return element.attrib['fullname']

        elif key == 'class':
            return element

        elif key == 'basenames':
            return basenames

        elif key == 'parents':
            return parents

        elif key == 'children':
            return children

        elif key == 'typemap':
            return typemap        
        
        elif key == 'headername':
            return headerelement.attrib['name']        

        elif key == 'headerfile':
            return headerelement.attrib['file']

        elif key == 'header':
            return headerelement
    #-----------------------------------------------------------------        
    def __updateheaderDB(self, headerelement):
        headername = self.headername(headerelement)
        if self.headerDB.has_key(headername): return
        self.headerDB[headername] = headerelement
        self.headerelements.append(headerelement)
    #-----------------------------------------------------------------
    # Recursively scan for includes and forwarded headers
    #-----------------------------------------------------------------        
    def __loadheaderDB(self, elem, depth=0):

        depth += 1
        if depth == 1:

            self.done = {}  # To keep track of processed headers
            self.headerscope = [elem.attrib['name']]
            
            self.__updateheaderDB(elem)

            if SHOWHEADER:
                print "\nHEADER( %s ) %s" % (self.headername(elem), elem)
            
        elif depth >= 25:
            fatal('lost in woods..! tag: %s' % elem.tag)    
        
        tab = depth*' '

        #-------------------------------------------------
        # Loop over elements
        #-------------------------------------------------
        for element in elem.getchildren():

            if   element.tag == 'namespace':

                self.__loadheaderDB(element, depth)

            elif element.tag == 'include':

                # Only scan headers
                name, ext = os.path.splitext(element.attrib['name'][1:-1])
                if ext[0:2] <> '.h': continue
                name = nameonly(name)

                if self.done.has_key(name): continue
                self.done[name] = 1
                
                h = self.headerElement(name)
                if h == None:
                    warning("No XML description for class %s" % name)
                    if SHOWHEADER:
                        print "%sINCLUDE( %s ) ** NO XML DESCRIPTION" % \
                              (tab, name)
                    continue
                
                self.__updateheaderDB(h)
                
                if SHOWHEADER:
                    print "%sINCLUDE( %s ) %s" % (tab, name,
                                                  "FOUND XML DESCRIPTION")

                self.headerscope.append(name)
                self.__loadheaderDB(h, depth)
                self.headerscope.pop()
                
            elif element.tag == 'forward':
                
                if len(self.headerscope) > 1: continue

                name = self.forwardname(element.attrib['name'])

                if self.done.has_key(name): continue
                self.done[name] = 1
                
                self.add(name)       # Add to list of types
                h = self.headerElement(name)
                if h == None:
                    warning("No XML description for forwarded "\
                            "class %s\n" % name)

                    if not self.__searchforheader(name, 'FORWARD'):
                    
                        if SHOWHEADER:
                            print "%sFORWARD( %s ) **** NOT FOUND" % \
                                  (tab, name)
                    continue
                
                if SHOWHEADER:
                    print "%sFORWARD( %s ) %s" % (tab, name, h)
                
                self.headerscope.append(name)
                self.__loadheaderDB(h, depth)
                self.headerscope.pop()
    #-----------------------------------------------------------------
    # Search the include search path for a header <name>.h. It is
    # assumed that the user has given the search path in the correct
    # order.
    #-----------------------------------------------------------------    
    def __searchforheader(self, name, where=''):

        if skipSearch(name): return 1
        
        if self.includeDB.has_key(name): return 1

        # First try headermap
        
        if self.headermap.has_key(name):
            
            recs = map(lambda x: split(x), self.headermap[name])

            for path in self.INCLUDE:

                lpath = len(path)

                for (file, include) in recs:

                    if file[0:lpath] == path:

                        self.includeDB[name] = strip(include)

                        if SHOWINCLUDE:
                            print "TYPENAME( %s ) search( %s )%s" % \
                                  (name, include, where)

                        # make sure to return!
                        return 1
        else:

            if SHOWINCLUDE: print "\tSEARCH FOR< %s >" % name
            
            # Search include paths

            for path in self.INCLUDE:
                if SHOWINCLUDE: print "\tPATH< %s >" % path

                lpath = len(path)

                file = []
                if os.path.exists(path):
                    file =  '%s/%s.h*' % (path, name)
                    cmd  = 'find %s/* -name "*%s.h*" -follow' % (path, name)
                    file = os.popen(cmd).readlines()

                if len(file) == 0: continue

                file = file[0]
                include = file[lpath+1:]
                self.includeDB[name] = strip(include)

                if SHOWINCLUDE:
                    print "TYPENAME( %s ) search( %s )%s" % \
                          (name, include, where)

                # make sure to return!
                return 1
                    
        if SHOWINCLUDE:
            print "\theader search FAILED( %s )" % name
        return 0
    #-----------------------------------------------------------------
    # Populate the typeDB with list of types in given header
    # and update headerDB if necessary
    #-----------------------------------------------------------------
    def __loadtypeDB(self, headerelement):
        name = self.headername(headerelement)
        types = headerelement.findall('type')
        for element in types:
            name = element.attrib['name']
            
            typenames = swords2.findall(name)
            for typename in typenames:
                # Skip local identifiers 
                if self.identifierDB.has_key(typename):continue
                if self.typeDB.has_key(typename):      continue
                if self.namespaceDB.has_key(typename): continue
                if ignorethisType(typename):           continue
                # Ignore strings
                if typename[0] == '"':                 continue

                
                h = self.headerElement(typename)
                if h == None: continue
                
                self.typeDB[typename] = DB.headername(h)
                self.__updateheaderDB(h)
                
                if SHOWHEADER:
                    print "TYPENAME( %s ) updateheader( %s )" % \
                          (typename, h.attrib['name'])                
    #-----------------------------------------------------------------
    # load classDB with classes from given header
    # Use two keys: short name and fullname
    #-----------------------------------------------------------------        
    def __loadclassDB(self, elem, depth=0):

        depth += 1
        if depth == 1:
            self.headerelement = elem
            self.scope = []           # To keep track of class scope
            self.namespace  = []      # Keep track of namespaces

            if SHOWCLASS:
                print "\nHEADER( %s )\t%s" % (self.headername(elem), elem)
            
        elif depth >= 50:
            fatal('lost in woods..! tag: %s' % elem.tag)    

        tab = depth*' '
        
        scope      = self.scope
        namespace  = self.namespace
        typedefMap = self.typedefMap
        context    = self.context
        
        #-------------------------------------------------
        # Loop over elements
        #-------------------------------------------------
        for element in elem.getchildren():

            name = ''
            fullname = ''
            tag = element.tag
            if element.attrib.has_key('name'):
                name = element.attrib['name']

            if element.attrib.has_key('fullname'):
                fullname = splitfields(element.attrib['fullname'],'<',1)[0]

                
            if   tag == 'namespace':

                if name <> '':
                    namespace.append(name)
                    self.namespaceDB[name] = 1
                    
                    if SHOWCLASS:
                        print "%sNAMESPACE(%s)" % (tab, name)
                    
                self.__loadclassDB(element, depth)

                if name <> '': namespace.pop()

            elif tag == 'section':

                self.__loadclassDB(element, depth)
                
            elif tag == 'typedef':

                record = joinfields(split(element.text),' ')
                record = replace(record,';','')
                record = standardizeName(record)
                
                updatecallbackDB(record)

                # Update self.typedefMap and possibly DB.callbackDB.
                # But make sure we update only global typedefs, that is,
                # typedefs at the header level

                if elem.tag == 'header':
                    self.__updatetypedefMap(record)
                    if SHOWCLASS: print "%sGLOBALTYPEDEF( %s )" % (tab, record)
                else:
                    if SHOWCLASS: print "%sTYPEDEF( %s )" % (tab, record)
                    
            elif tag == 'class':

               if SHOWCLASS: print "%sCLASS( %s ) %s" % (tab, name, element)

               self.__updateclassDB(element, self.headerelement)
               self.__updateclassIdentifiers(element)
               self.__addbaseclassElements(element)
               
               scope.append(name) # Keep track of class namespace
               self.__loadclassDB(element, depth)                
               scope.pop() # Remember to pop!
    #-----------------------------------------------------------------
    def __updateincludes(self, headerelement):
        name = self.headername(headerelement)
        self.includeDB[name] =  headerelement.attrib['name']

        for typename in self.typeDB.keys():
            self.addInclude(typename,'UPDATE')
    #-----------------------------------------------------------------
    # Given a header, update typemaps of all classes therein
    #-----------------------------------------------------------------
    def __updateclassTypemaps(self, elem, depth=0):

        depth += 1
        tab = depth*' '
        
        if depth == 1:
            self.scope = [] # To keep track of class scope
            
        elif depth > 50:
            fatal('lost in woods..! tag: %s' % elem.tag)    

        scope = self.scope
        
        #-------------------------------------------------
        # Loop over elements
        #-------------------------------------------------
        for element in elem.getchildren():

            tag = element.tag
            if element.attrib.has_key('name'):
                name = element.attrib['name']
            else:
                name = ''

            if tag == 'namespace':

                self.__updateclassTypemaps(element, depth)

            elif tag == 'section':

                self.__updateclassTypemaps(element, depth)
                
            elif tag == 'class':

                # The order matters!

                self.__addparents(element)
                self.__addchildren(element)
                self.__addsiblings(element)
                self.__updatetypeMap(element)
                
                self.scope.append(name) # Keep track of class namespace
                self.__updateclassTypemaps(element, depth)
                self.scope.pop()
    #-----------------------------------------------------------------
    # Given a header, update attributes of all classes therein
    #-----------------------------------------------------------------
    def __updateclassIdentifiers(self, elem, depth=0):

        depth += 1
        tab = depth*' '
        
        if depth == 1:

            self.classname = elem.attrib['name']
            self.scope = [] # To keep track of class scope
            
        elif depth > 50:
            fatal('lost in woods..! tag: %s' % elem.tag)    

        scope = self.scope
        
        #-------------------------------------------------
        # Loop over elements
        #-------------------------------------------------
        for element in elem.getchildren():

            tag = element.tag
            
            
            if element.attrib.has_key('name'):
                name = element.attrib['name']
            else:
                name = ''

            if tag == 'section':

                self.__updateclassIdentifiers(element, depth)

            elif tag == 'typedef':

                record = element.text
                t = decodeCallback(record)
                if t <> None:
                    name = t[1]
                else:
                    name, value = decodeTypedef(record)

                self.__addidentifier(self.classname, name)
                
            elif tag == 'enum':

                if name == '': continue

                self.__addidentifier(self.classname, name)
                
            elif tag == 'class':

                self.__addidentifier(self.classname, name)                    
                self.scope.append(name) # Keep track of class namespace
                self.__updateclassIdentifiers(element, depth)
                self.scope.pop()
                
            elif tag in ROUTINES:
                
                self.__addidentifier(self.classname, name)                    
                self.scope.append(name) # Keep track of class namespace
                self.__updateclassIdentifiers(element, depth)
                self.scope.pop()
    #-----------------------------------------------------------------    
    def __addidentifier(self, classname, name):
        # skip name of parent class
        if classname == name: return

        self.scope.append(name)
        identifier = joinfields(self.scope,'::')
        self.scope.pop()
        if not self.identifierDB.has_key(identifier):
            self.identifierDB[identifier] = classname

        element = self.classDB[classname]
        element.attrib['identifiers'][identifier]=1
        identifier = "%s::%s" % (classname, identifier)
        if not self.identifierDB.has_key(identifier):
            self.identifierDB[identifier] = classname
    #-----------------------------------------------------------------    
    def __updatetypedefMap(self, record):

        typedefMap = self.typedefMap        
        record = strip(record)
        record = joinfields(split(record),' ')

        # Check for function typedefs. If we find one, add to
        # list of potential callbacks. 

        t = decodeCallback(record)
        if t <> None:
            retype, name, arglist, v, q = t
            if typedefMap.has_key(name): return

            code = not INLINE
            skip = 1
            DB.callbackDB[name] =[retype,arglist, code, skip]
        else:
            key, value = decodeTypedef(record)
            if typedefMap.has_key(key): return
            typedefMap[key] = (value, TYPEDEFTYPE)
    #-----------------------------------------------------------------    
    # Unfold typedefs and add new keys to typemap
    #-----------------------------------------------------------------    
    def __unfoldtypedefs(self):
        typeMap = {}
        for key in self.typedefMap.keys():
            value, ttype = self.typedefMap[key]
            typeMap[key] = unfoldTypedef(key, value, self.typedefMap)
        
        for key, value in typeMap.items():
            if self.globaltypemap.has_key(key): continue
            self.globaltypemap[key] = value
    #-----------------------------------------------------------------
    # For given class object, add, recursively, all base classes to
    # the database.
    #-----------------------------------------------------------------
    def __addbaseclassElements(self, classelement, depth=0):
        depth += 1
        if depth == 1:
            # NB: We need name of current class, so cache it.
            self.currentname = classelement.attrib['name']
            classelement.attrib['basenames'] = []            

            # Skip stl classes since they won't be in XmlMap!
            
            if stltype.search(self.currentname) != None: return
            
            if SHOWBASE: print "DERIVED( %s )" % self.currentname
            
        elif depth > 50:
            fatal("__addbaseclassElements - lost in trees")

        tab = depth*' '
        basenames = baseclassNames(classelement)
        
        for fullname in basenames:

            if SHOWBASE: print "%sBASE( %s )" % (2*tab,fullname)
            
            classname = strip(splitfields(fullname, '<')[0])

            # Check if class is iterable by inheritance.
            # A class is iterable  by inheritance if one
            # of its base classes is an stl container type.
            
            if iterabletype.search(classname) <> None:
                self.classDB[self.currentname].attrib['iterable'
                                                      'Byinheritance'] = 1
                self.classDB[self.currentname].attrib['iterableclass']=fullname
            
            # Update list of base class names of the class element
            # with which this function was called.

            unique(self.classDB[self.currentname].attrib['basenames'],
                   fullname)


            # Maybe classDB aleady contains classname
            
            if self.classDB.has_key(classname): continue

            # classname not yet in class database, so update it

            # get header element
            
            h = self.headerElement(classname)
            if h == None:
                inform("No XML description for base class %s\n"
                       "\t\tof derived class %s" % (classname,
                                                    self.currentname))
                continue

            # found header element, now find class element
            
            c = self.__classElement(classname, h)
            if c == None:
                # Maybe this is a typedef, so try using the
                # first field.
                classname = splitfields(classname, '::',1)[0]
                c = self.__classElement(classname, h)
                if c == None:
                    inform("No XML description for base class %s\n"
                           "\t\tof derived class %s" % (classname,
                                                        self.currentname))
                    continue
            
            self.__updateclassDB(c, h)
            
            # classname should now be in classDB; if not fall on sword!

            if not self.classDB.has_key(classname):
                fatal("expected to find classname %s in classDB" % classname)
                
            c = self.classDB[classname]
            
            self.__addbaseclassElements(c, depth)
    #-----------------------------------------------------------------
    def __addparents(self, classelement):
        classelement.attrib['parents'] = []
        name = classelement.attrib['name']
        for name in self.scope:
            unique(classelement.attrib['parents'], name)
        # Reverse order "from outer to inner" to "from inner to outer" 
        classelement.attrib['parents'].reverse()
    #-----------------------------------------------------------------
    def __addchildren(self, classelement):
        classelement.attrib['children'] = []
        children = []
        findClassElements(classelement, children)
        for element in children:
            name = element.attrib['name']
            unique(classelement.attrib['children'], name)
    #-----------------------------------------------------------------
    # Find siblings of current class. I use the term siblings in an
    # idiosyncratic way: siblings include not only actual siblings,
    # but also uncles and grand-uncles etc.!
    #-----------------------------------------------------------------    
    def __addsiblings(self, classelement):
        
        # For each parent find list of children, but ignore self        
        # and own children! Note: the children of a given class,
        # include all generations below parent; that is, it includes
        # grand-children etc.
        
        classname = classelement.attrib['name']
        ignorelist = [classname] + classelement.attrib['children']
        
        classelement.attrib['siblings'] = []
        for name in classelement.attrib['parents']:
            children = self.classDB[name].attrib['children']
            for child in children:
                if child in ignorelist: continue
                unique(classelement.attrib['siblings'], child)

        # Reverse order "from outer to inner" to "from inner to outer" 
        # We do this, so that later we consider siblings by starting with
        # those that have the closest relationship to current class. That
        # is, we visit actual siblings first, then uncles, then grand-
        # uncles etc. This is important in translateTypes to dis-
        # ambiguate, correctly, potentially ambiguous translations.
        
        classelement.attrib['siblings'].reverse()        
    #-----------------------------------------------------------------
    def __updateclassDB(self, classelement, headerelement):

        classname = classelement.attrib['name']
        
        if self.classDB.has_key(classname):

            # Class with short-name "classname" already present.
            # Check full-names and return if they are the same.
            
            fullname = classelement.attrib['fullname']
            fname = self.classDB[classname].attrib['fullname']
            if fullname == fname: return

            # We have different full-names, so add new class to
            # database using its full-name.

            self.classDB[fullname] = classelement
            self.classelements.append(classelement)
            inform('short name conflict between\n'\
                   '\t\t%s and\n\t\t%s' % (fname, fullname))
                   
        else:
            fullname = classelement.attrib['fullname']
            fname = splitfields(fullname,'<',1)[0]
            self.classDB[classname] = classelement
            self.classDB[fname]     = classelement
            self.classDB[fullname]  = classelement
            self.classelements.append(classelement)
        
        classelement.attrib['header']   = headerelement
        classelement.attrib['basenames']= []
        classelement.attrib['parents']  = []
        classelement.attrib['children'] = []
        classelement.attrib['siblings'] = []
        classelement.attrib['doc']      = []
        classelement.attrib['identifiers'] = {}
        classelement.attrib['TypeMap'] = {}
        classelement.attrib['iterableByinheritance'] = 0
        classelement.attrib['iterableBycontainment'] = 0
        classelement.attrib['iterableclass'] = ''
        
        if classelement.attrib.has_key('template'):
            analyzeTemplate(classelement)
            
        if not classelement.attrib.has_key('project'):
            classelement.attrib['project'] = ''
            
        updatetypeMap(classelement.attrib['TypeMap'], classelement)
    #-----------------------------------------------------------------
    # For given class:
    # 1. add the full-names of children to the typemaps.
    # 2. inherit the typemaps from parents and base classes.
    #
    # We have to be careful, however, of inheriting from templated 
    # base classes that may have been partially instantiated.
    # The instantiatetypemaps function tries to deal with this case.
    #-----------------------------------------------------------------
    def __instantiatetypemaps(self, classelement, splitname):
        name = splitname[0]
        c = self.classDB[name]
        if not c.attrib.has_key('template'): return {}
            
        # Associate each template parameter with an actual type, if
        # appropriate.

        # Algorithm
        # 1. Produce a list D of the template parameters of the derived class.
        # 2. Produce a list P of the template parameters of the (possibly
        #    partially) instantiated base class.
        # 3. Produce a list B of the template parameters of the base class.
        # 4. For each x in P, map y in B to x if x is not in D.

        # 1. Produce list D
        if classelement.attrib.has_key('template'):
            D = classelement.attrib['templateparlist']
        else:
            D = []

        # 2. Produce list P
        #------------------
        # For now we assume each actual type is delimited by a comma.
        # This could fail on types that are themselves instantiated
        # templates with multiple actual types. But we'll cross that
        # bridge when we get there!

        P = findActualTypes(splitname[1])

        # 3. Produce list B

        B = c.attrib['templateparlist']

        if SHOWTEMPLATE:
            print "CLASS( %s )" % classelement.attrib['fullname']
            print "  BASECLASS( %s )" % c.attrib['fullname']
            print "\t\tD( %s )" % D
            print "\t\tP( %s )" % P
            print "\t\tB( %s )" % B

        # Here is an example from COBRA:
        # template <class T, class FrameTag>
        # class Point3DBase : public PV3DBase< T, PointTag, FrameTag>
        # template <class T, class PVType, class FrameType>
        # class PV3DBase

        # 4. For each x in P, map y in B to x if x is not in D.
        
        temptypemap = {}
        for index, x in enumerate(P):
            if x in D: continue
            if temptypemap.has_key(x): continue
            if index >= len(B): continue
            y = B[index]
            temptypemap[y] = x
            if SHOWCLASS:
                print "\tTEMPLATEPAR( %s ) -> ( %s )" % (y, x)
        if len(temptypemap) == 0: return {}
        
        localcontext = {}
        localcontext['typemap'] = temptypemap
        localcontext['templatepar'] = ''
        localcontext['globaltypemap'] = {}
        localcontext['templateparlist'] = []
        localcontext['classname'] = classelement.attrib['name']
        tmap = {}
        for key, value in self.classDB[name].attrib['TypeMap'].items():
            if tmap.has_key(key): continue
            value = translateAllTypes(localcontext, value)
            tmap[key] = value
        return tmap
    #-----------------------------------------------------------------
    def __updatetypeMap(self, classelement):

        if SHOWTYPEMAP:
            print "UPDATE TYPEMAP( %s )" % classelement.attrib['fullname']
        
        typemap = classelement.attrib['TypeMap']
        
        # Inherit typemaps from children
        
        children = classelement.attrib['children']
        for name in children:
            if not self.classDB.has_key(name): continue
            for key, value in self.classDB[name].attrib['TypeMap'].items():
                if typemap.has_key(key): continue
                typemap[key] = value

        # Inherit typemap from base classes.

        basenames = classelement.attrib['basenames']
        for fullname in basenames:
            splitname = splitfields(fullname,'<',1)
            name = splitname[0]
            if not self.classDB.has_key(name):
                if SHOWTYPEMAP:
                    print "\tUNKNOWN BASECLASS( %s )" % name
                continue

            # Base class could be an instantiated template
            # If so, make sure we replace the template parameter of
            # its typemap values with actual parameters.

            if len(splitname) > 1 and splitname[1] <> '':
                splitname[1] = '<'+splitname[1]
                tmap = self.__instantiatetypemaps(classelement, splitname)
                for key, value in tmap.items():
                    if typemap.has_key(key): continue
                    typemap[key] = value
                    if SHOWTYPEMAP:
                        print "\tBASE TYPEMAP( %s ) -> ( %s )" % (key, value)
            else:
                for key, value in self.classDB[name].attrib['TypeMap'].items():
                    if typemap.has_key(key): continue
                    typemap[key] = value
                    if SHOWTYPEMAP:
                        print "\tBASE TYPEMAP( %s ) -> ( %s )" % (key, value)
                
            # Add typemaps of children of base classes
            children = []
            findClassElements(self.classDB[name], children)
            for child in children:
                updatetypeMap(typemap, child)    

        # Inherit typemap from parents

        parents = classelement.attrib['parents']
        for name in parents:
            if not self.classDB.has_key(name): continue
            for key, value in self.classDB[name].attrib['TypeMap'].items():
                if typemap.has_key(key): continue
                typemap[key] = value

       # Inherit typemap from siblings

        siblings = classelement.attrib['siblings']
        for name in siblings:
            if not self.classDB.has_key(name): continue
            for key, value in self.classDB[name].attrib['TypeMap'].items():
                if typemap.has_key(key): continue
                typemap[key] = value
    #-----------------------------------------------------------------
    # For each class, determine if it has iterators
    #-----------------------------------------------------------------
    def __checkforIterators(self, element): 
        nestclasses = findElements(element, 'class')
        for e in nestclasses:
            if e.attrib['name'] in ['iterator', 'const_iterator']:
                element.attrib['iterableBycontainment'] = 1
                element.attrib['iterableclass'] = element.attrib['fullname']
    #-----------------------------------------------------------------
    # Given a class name, return the header element to which it belongs.
    # IMPORTANT: If header element is already in database, make sure
    #            we return that instance of the header element.
    # If there is an ambiguity, then try to use the header element
    # associated with the current Python module. Otherwise return the
    # best match.
    #-----------------------------------------------------------------
    def headerElement(self, name):
        
        if self.namespaceDB.has_key(name): return None

        if self.classDB.has_key(name):
            return self.classDB[name].attrib['header']

        if not self.xmlmap.has_key(name):
            # Could be of the form name<...>
            name = splitfields(name, '<',1)[0]
            if not self.xmlmap.has_key(name):
                return None
        #------------------------------------------------------------
        # Get the full pathname of the XML file.
        # Note: the paths are stored in XmlMap.py relative to
        # PYTHON_PROJECTS.
        #
        # If we have ambiguities pick the class object
        # whose module matches the current one.
        #------------------------------------------------------------
        t = self.xmlmap[name]
        paths = []
        for index, p in enumerate(t):
            if os.path.exists(p):
                paths.append(p)
            else:
                p = '%s%s' % (BASE, p)
                if not os.path.exists(p): continue
                paths.append(p)

        #------------------------------------------------------------
        # Note: Even though the path is in the XmlMap, this doesn't
        # guarantee that the file actually exists, so paths could be
        # empty.
        #------------------------------------------------------------
        if len(paths) == 0:
            inform("Warning - Found entry for %s in XmlMap, "\
                   "but no XML file" % name)
            return None
        
        if len(paths) > 1:

            # More than one XML file matches given class.

            for xmlfile in paths:

                element = readXmlFile(xmlfile) # Read header element
                if element == None:
                    if SHOWHEADER:
                        print "\tXML file\n\t%s NOT FOUND" % xmlfile
                    continue
                else:
                    if SHOWHEADER:
                        print "\tFOUND XML file %s" % xmlfile

                headername = self.headername(element)
                if self.headerDB.has_key(headername):
                    if SHOWHEADER:
                        print "\t\tRETURN existing header ", headername
                    return self.headerDB[headername]

                classes = []
                findClassElements(element, classes)
                for classelement in classes:
                    module = classelement.attrib['module']
                    project= classelement.attrib['project']
                    
                    if SHOWHEADER:
                        print "\t\tMODULE CHECK(%s -> %s)" % (module, MODULE)
                    if module == MODULE:
                        warning("Database ambiguity - using %s in %s" %
                                (name, module))

                        # Remember to update header database
                        self.__updateheaderDB(element)
                        return element
                    elif project == PROJECT:
                        # Remember to update header database
                        self.__updateheaderDB(element)
                        if SHOWHEADER: print "\t\tKEEP header ", headername
                        return element


            # Update database and return our best guess!
            if element <> None: self.__updateheaderDB(element)
            return element
        else:

            # Found one-to-one map between XML file and given class
            
            xmlfile = paths[0]
            element = readXmlFile(xmlfile)
            if element == None:
                return None
                
            name = self.headername(element)
            if self.headerDB.has_key(name):
                return self.headerDB[name]
            else:
                # Remember to update header database
                
                self.__updateheaderDB(element)
                return element
    #-----------------------------------------------------------------
    # Given class name, get class element from given header
    # IMPORTANT: If class element is already in database, make sure
    #            we return that instance of the class element.
    #-----------------------------------------------------------------    
    def classElement(self, name, both=False):

        if self.namespaceDB.has_key(name): return None        
        if self.classDB.has_key(name): return self.classDB[name]

        h = self.headerElement(name)
        if h == None: return None

        c = self.__classElement(name, h)
        if both:
            return (c, h)
        else:
            return c

    #-----------------------------------------------------------------
    # Given name, get typedef element from given header
    #-----------------------------------------------------------------    
    def typedefElement(self, name, both=False):

        if self.namespaceDB.has_key(name): return None        
        if self.classDB.has_key(name): return None

        h = self.headerElement(name)
        if h == None: return None

        t = self.__typedefElement(name, h)
        if both:
            return (t, h)
        else:
            return t
        
    def __classElement(self, name, elem):
    
        if elem == None: return None
        if self.namespaceDB.has_key(name): return None
        if self.classDB.has_key(name): return self.classDB[name]

        classes = []
        findClassElements(elem, classes)
        for classelement in classes:
            
            shortname = classelement.attrib['name']
            if name == shortname: return classelement

            fullname = classelement.attrib['fullname']
            if name == fullname: return classelement

            fullname = split(fullname,'<')[0]
            if name == fullname: return classelement

        return None

    def __typedefElement(self, name, elem):
    
        if elem == None: return None
        if self.namespaceDB.has_key(name): return None
        if self.classDB.has_key(name): return None

        elements = []
        findTypedefElements(elem, elements)
        for element in elements:
            
            shortname = element.attrib['name']
            if name == shortname: return element

            fullname = element.attrib['fullname']
            if name == fullname: return element

        return None
        
#---------------------------------------------------------------------------
# Create XML Database instance
#---------------------------------------------------------------------------
DB = Database(CONTEXT)
#===========================================================================
# Edit commands
#
#  ExcludeClass(class name regular expression)
#
#  ExcludeMethod(class name regular expression,
#                method name regular expression)
#
#  IncludeMethod(class name regular expression,
#                method name regular expression)
#
#  ExcludeCallback(header name regular expression,
#                  method name regular expression)
#
#  ExcludeFunction(header name regular expression,
#                  function name regular expression)
#
#  Replace(old-name, new-name[, ARGS | RETURNS | VALUE])
#
#  EnableMethodOverride(class name regular expression,
#                       method name regular expression)
#
#  InsertCode(name regular expression, code, where=EXPORT_FUNCTION)
#
#  Insert(header name regular expression, record[, BEFORE | AFTER])
#
#  DeclareCallback(header name regular expression, record)
#
#  InstantiateCallback(method name regular expression, number)
#
#  Instantiate(class name regular expression, template types,
#              method name regular expression)
#
#  DefineLength(class name regular expression, sizeString)
#
#  UseNoargVector(regex type)
#
#  UseNoinitRange(regex type)
#
#  RenameMethod(class name regex, method name regex, newname)
#
#  MakeNoncopyable(class name regular expression)
#
#  ExcludeHolder(class name regular expression)
#===========================================================================
def fixescape(record):
    record = replace(record, '\b','\\b')
    record = replace(record, '\s','\\s')
    record = replace(record, '\w','\\w')
    record = replace(record, '\t','\\t')
    record = replace(record, '\n','\\n')
    return record
#---------------------------------------------------------------------------
# Usage: ExcludeClass(class name regular expression) 
#---------------------------------------------------------------------------
def Exclude(regex, record=None):
    if record == None:
        ExcludeClass(regex)
    else:
        ExcludeMethod(regex, record)
        ExcludeFunction(regex, record)
            
ExcludeClassEdits = []
def ExcludeClass(regex):
    global ExcludeClassEdits
    regex = fixescape(regex)
    ExcludeClassEdits.append(re.compile(regex))
    
def excludeClass(classname):
    global ExcludeClassEdits
    for nameregex in ExcludeClassEdits:
        if nameregex.search(classname) <> None: return 1
    return 0
#---------------------------------------------------------------------------
# Usage: ExcludeInclude(header name regular expression) 
#---------------------------------------------------------------------------
ExcludeIncludeEdits = []
def ExcludeInclude(regex, record):
    global ExcludeIncludeEdits
    regex  = fixescape(regex)
    record = fixescape(record)
    ExcludeIncludeEdits.append((re.compile(regex), re.compile(record)))
    
def excludeInclude(name, record):
    global ExcludeIncludeEdits
    for nameregex, recordregex in ExcludeIncludeEdits:
        if nameregex.search(name) <> None:
            if recordregex.search(record) <> None: return 1
    return 0
#---------------------------------------------------------------------------
# Usage: ExcludeMethod(class name regular expression,
#                      method name regular expression)
#---------------------------------------------------------------------------
ExcludeMethodEdits = []
def ExcludeMethod(regex, record):
    global ExcludeMethodEdits
    regex  = fixescape(regex)
    record = fixescape(record)
    ExcludeMethodEdits.append((re.compile(regex), re.compile(record)))

def ExcludeAllMethods():
    ExcludeMethod(".*",".*")
    
def excludeMethod(classname, record):
    global ExcludeMethodEdits
    for nameregex, recordregex in ExcludeMethodEdits:
        if nameregex.search(classname) <> None:
            if recordregex.search(record) <> None: return 1
    return 0
#---------------------------------------------------------------------------
# Usage: ExcludeMember(class name regular expression,
#                      member name regular expression)
#---------------------------------------------------------------------------
ExcludeMemberEdits = []
def ExcludeMember(regex, record):
    global ExcludeMemberEdits
    regex  = fixescape(regex)
    record = fixescape(record)
    ExcludeMemberEdits.append((re.compile(regex), re.compile(record)))

def ExcludeAllMembers():
    ExcludeMember(".*",".*")
    
def excludeMember(classname, record):
    global ExcludeMemberEdits
    for nameregex, recordregex in ExcludeMemberEdits:
        if nameregex.search(classname) <> None:
            if recordregex.search(record) <> None: return 1
    return 0
#---------------------------------------------------------------------------
# Usage: IncludeMethod(class name regular expression,
#                      method name regular expression)
#---------------------------------------------------------------------------
IncludeMethodEdits = []
def IncludeMethod(regex, record):
    global IncludeMethodEdits
    regex  = fixescape(regex)
    record = fixescape(record)
    IncludeMethodEdits.append((re.compile(regex), re.compile(record)))
    
def includeMethod(classname, record):
    global IncludeMethodEdits
    for nameregex, recordregex in IncludeMethodEdits:
        if nameregex.search(classname) <> None:
            if recordregex.search(record) <> None: return 1
    return 0
#---------------------------------------------------------------------------
# Usage: ExcludeCallback(header name regular expression,
#                        method name regular expression)
#---------------------------------------------------------------------------
ExcludeCallbackEdits = []
def ExcludeCallback(regex, record):
    global ExcludeCallbackEdits
    regex  = fixescape(regex)
    record = fixescape(record)
    ExcludeCallbackEdits.append((re.compile(regex), re.compile(record)))
    
def excludeCallback(headername, record):
    global ExcludeCallbackEdits
    for nameregex, recordregex in ExcludeCallbackEdits:
        if nameregex.search(headername) <> None:
            if recordregex.search(record) <> None: return 1
    return 0
#---------------------------------------------------------------------------
# Usage: MapVectorToList(function name regular expression)
#---------------------------------------------------------------------------
MapVectorToListEdits = []
def MapVectorToList(regex):
    global MapVectorToListEdits
    regex = re.compile(fixescape(regex))
    MapVectorToListEdits.append(regex)
    
def mapVectorToList(funcname):
    global MapVectorToListEdits
    for nameregex  in MapVectorToListEdits:
        if nameregex.search(funcname) <> None: return True
    return False
#---------------------------------------------------------------------------
# Usage: ExcludeFunction(header name regular expression,
#                        function name regular expression)
#---------------------------------------------------------------------------
ExcludeFunctionEdits = []
def ExcludeFunction(regex, record):
    global ExcludeFunctionEdits
    regex  = fixescape(regex)
    record = fixescape(record)
    ExcludeFunctionEdits.append((re.compile(regex), re.compile(record)))

def ExcludeAllFunctions():
    ExcludeFunction(".*",".*")
    
def excludeFunction(headername, record):
    global ExcludeFunctionEdits
    for nameregex, recordregex in ExcludeFunctionEdits:
        if nameregex.match(headername) <> None:
            if recordregex.search(record) <> None: return 1
    return 0
#---------------------------------------------------------------------------
# Usage: IncludeFunction(header name regular expression,
#                        function name regular expression)
#---------------------------------------------------------------------------
IncludeFunctionEdits = []
def IncludeFunction(regex, record):
    global IncludeFunctionEdits
    regex  = fixescape(regex)
    record = fixescape(record)
    IncludeFunctionEdits.append((re.compile(regex), re.compile(record)))
    
def includeFunction(headername, record):
    global IncludeFunctionEdits
    for nameregex, recordregex in IncludeFunctionEdits:
        if nameregex.match(headername) <> None:
            if recordregex.search(record) <> None: return 1
    return 0
#---------------------------------------------------------------------------
# Usage: IncludeDummyClass(class name regular expression, classname)
#---------------------------------------------------------------------------
IncludeDummyClassEdits = []
def IncludeDummyClass(regex, dummyclassname):
    global IncludeDummyClassEdits
    regex  = fixescape(regex)
    IncludeDummyClassEdits.append(re.compile(regex), dummyclassname)
    
def includeDummyClass(classname):
    global IncludeDummyClassEdits
    s = ''
    for nameregex, name in IncludeDummyClassEdits:
        if nameregex.search(classname) <> None:
            s = s + '  class_< %s, boost::noncopyable > ("%s", no_init);\n' \
                % (name, name)
    return s
#---------------------------------------------------------------------------
# Usage: EnableMethodOverride(class name regular expression,
#                             method name regular expression)
#---------------------------------------------------------------------------
MethodOverrideEdits = []
def EnableMethodOverride(regex, record='.+'):
    global MethodOverrideEdits
    regex  = fixescape(regex)
    record = fixescape(record)
    MethodOverrideEdits.append((re.compile(regex), re.compile(record)))
    
def insertMethodOverride(classname, record=None):
    global MethodOverrideEdits
    for nameregex, recordregex in MethodOverrideEdits:
        if nameregex.search(classname) <> None:
            if record == None: return 1
            if recordregex.search(record) <> None: return 1
    return 0
#---------------------------------------------------------------------------
# Usage: SkipSearch(name regular expression)
#---------------------------------------------------------------------------
SkipSearchEdits = []
def SkipSearch(regex):
    global SkipSearchEdits
    regex  = fixescape(regex)
    SkipSearchEdits.append(re.compile(regex))
    
def skipSearch(name):
    global SkipSearchEdits
    for nameregex in SkipSearchEdits:
        if nameregex.search(name) <> None:
            return 1
    return 0
#---------------------------------------------------------------------------
# For every constructor, method or function change the args string into a
# a list and optionally change argument type.
#
# Usage: Replace(old-name, new-name[, ARGS | RETURNS | VALUE])
#---------------------------------------------------------------------------
ARGS=0
RETURNS=1
VALUE=2

ReplaceArgTypeEdits = []
ReplaceReturnTypeEdits = []
ReplaceValueEdits = []

def Replace(oldname, newname, where=None):
    global ReplaceArgTypeEdits, ReplaceReturnTypeEdits
    oldname  = re.compile(fixescape(oldname))
    if where == None:
        ReplaceArgTypeEdits.append((oldname, newname))
        ReplaceReturnTypeEdits.append((oldname, newname))
    elif where == ARGS:
        ReplaceArgTypeEdits.append((oldname, newname))
    elif where == RETURNS:
        ReplaceReturnTypeEdits.append((oldname, newname))
    elif where == VALUE:
        ReplaceValueEdits.append((oldname, newname))
#---------------------------------------------------------------------------
def replaceValue(value):
    global ReplaceValueEdits
    for regex, newvalue in ReplaceValueEdits:
        if regex.match(value) <> None: return newvalue
    return value
#---------------------------------------------------------------------------
def replaceTypes(element):

    if type(element.attrib['args']) == type(""):
        # Convert to lists 
        element.attrib['args'] = eval(element.attrib['args'])
        element.attrib['vars'] = eval(element.attrib['vars'])

    # Replace arg types
    
    for row, oldType in enumerate(element.attrib['args']):
        arg = joinfields(split(element.attrib['args'][row]),' ')
        element.attrib['args'][row] = arg
        
        for regex, newType in ReplaceArgTypeEdits:
            if regex.search(oldType) <> None:
                str = element.attrib['args'][row]
                str = regex.sub(newType, str)
                str = replace(str,'const const','const') # HACK!
                str = regex.sub(newType, str)
                element.attrib['args'][row] = str
                break
        
    # Fix returns
            
    retype = joinfields(split(element.attrib['return']),' ')
    element.attrib['static'] = isStatic.search(retype) <> None
    element.attrib['virtual']= isVirtual.search(retype) <> None
    element.attrib['return'] = stripreturn.sub('',retype) 

    oldType = element.attrib['return']
    for regex, newType in ReplaceReturnTypeEdits:
        if regex.search(oldType) <> None:
            str = element.attrib['return']
            str = regex.sub(newType, str)
            str = replace(str,'const const','const') # HACK!
            element.attrib['return'] = str
            break
#---------------------------------------------------------------------------
# For a method or function that shadows another, rename as specified
# Usage: AutoRename(method regex, how)
#---------------------------------------------------------------------------
AutoRenameEdits = []
def AutoRename(classname, routine, how='append type 1'): 
    classname= re.compile(fixescape(classname))
    routine  = re.compile(fixescape(routine))
    AutoRenameEdits.append((classname, routine, how))
#---------------------------------------------------------------------------
def autoRename(classname, routine, args):
    global AutoRenameEdits
    for classregex, routineregex, how in AutoRenameEdits:
        if classregex.match(classname) == None: continue
        if routineregex.search(routine)== None: continue

        words = swords.findall(args[0])
        for word in words:
            routine = routine + capitalize(word)
        return routine
    return routine
#---------------------------------------------------------------------------
# To rename a method
# Usage: RenameMethod(classname, method regex, newname)
#---------------------------------------------------------------------------
RenameMethodEdits = []
def RenameMethod(classname, routine, newname): 
    classname= re.compile(fixescape(classname))
    routine  = re.compile(fixescape(routine))
    RenameMethodEdits.append((classname, routine, newname))
#---------------------------------------------------------------------------
def renameMethod(classname, routine, record):
    global RenameMethodEdits
    for classregex, routineregex, newname in RenameMethodEdits:
        if classregex.match(classname) == None: continue
        if routineregex.search(record)== None: continue
        return newname
    return routine
#---------------------------------------------------------------------------
# Usage: TrapNegativeReturn(class name regular expression,
#                           method name regular expression)
#---------------------------------------------------------------------------
TrapNegativeReturnEdits = []
def TrapNegativeReturn(regex, record):
    global TrapNegativeReturnEdits
    regex  = fixescape(regex)
    record = fixescape(record)
    TrapNegativeReturnEdits.append((re.compile(regex), re.compile(record)))
    
def trapNegativeReturn(classname, record):
    global TrapNegativeReturnEdits
    for nameregex, recordregex in TrapNegativeReturnEdits:
        if nameregex.search(classname) <> None:
            if recordregex.search(record) <> None: return 1
    return 0
#---------------------------------------------------------------------------
# Handle arguments in need of special processing
#---------------------------------------------------------------------------
def analyzeCallbackArguments(context, element):    

    for row, arg in enumerate(element.attrib['args']):

        name = split(stripName(arg),'::').pop()
    
        if DB.callbackDB.has_key(name):
            skip = 0
            DB.callbackDB[name][C_SKIP] = skip

        else:
            
            # Check if this is a callback argument, declared in-line

            m = callback2.match(arg)
            if m <> None:
                name = m.group('name')
                if DB.callbackDB.has_key(name):
                    skip = 0
                    DB.callbackDB[name][C_SKIP] = skip
                    continue

                # Argument has an in-line declared callback that has not
                # been defined in a typedef, so create a new callback type

                t = split(arg,'=') # May have a value
                arg = t[0]
                if len(t) == 2:
                    val = '=%s' % t[1]
                else:
                    val = ''

                n = len(DB.callbackDB)
                callb = callbackName.sub('CB%d' % n, arg)
                arg   = callback.sub('CB%d' % n, arg) + val
                element.attrib['args'][row] = arg

                retype, name, \
                        arglist, \
                        varlist, qual = decodeMethodName(callb)
                skip = 0
                DB.callbackDB[name] = [retype, arglist, INLINE, skip]
#---------------------------------------------------------------------------
# Usage: Map(class name regular expression,
#            method name regular expression,
#            old type, new type)
#---------------------------------------------------------------------------
MapTypeEdits = []
def Map(classname, routine, oldtype, newtype):
    global MapTypeEdits
    classname= re.compile(fixescape(classname))
    routine  = re.compile(fixescape(routine))
    oldtype  = re.compile(fixescape(oldtype))
    MapTypeEdits.append((classname, routine, oldtype, newtype))
    
def mapType(classname, routine, oldtype):
    global MapTypeEdits
    for classregex, routineregex, oldtyperegex, newtype in MapTypeEdits:
        if classregex.match(classname) <> None:
            if routineregex.search(routine) <> None:
                if oldtyperegex.match(oldtype) <> None:
                    return newtype
    return oldtype
#---------------------------------------------------------------------------
# Usage: UseNoargVector(regex type)
#--------------------------------------------------------------------------
NoargVectorEdits = []
def UseNoargVector(typeregex):
    global NoargVectorEdits
    regex = fixescape(typeregex)
    NoargVectorEdits.append(re.compile(regex))

def useNoargVector(ttype):
    for typeregex in NoargVectorEdits:
        if typeregex.match(ttype) <> None: return 1
    return 0
#---------------------------------------------------------------------------
# Usage: UseNoinitRange(regex type)
#--------------------------------------------------------------------------
NoinitRangeEdits = []
def UseNoinitRange(typeregex):
    global NoinitRangeEdits
    regex = fixescape(typeregex)
    NoinitRangeEdits.append(re.compile(regex))

def useNoinitRange(ttype):
    for typeregex in NoinitRangeEdits:
        if typeregex.match(ttype) <> None: return 1
    return 0
#---------------------------------------------------------------------------
# Usage: UseNoinitPair(regex type)
#--------------------------------------------------------------------------
NoinitPairEdits = []
def UseNoinitPair(typeregex):
    global NoinitPairEdits
    regex = fixescape(typeregex)
    NoinitPairEdits.append(re.compile(regex))

def useNoinitPair(ttype):
    for typeregex in NoinitPairEdits:
        if typeregex.match(ttype) <> None: return 1
    return 0
#---------------------------------------------------------------------------
# Usage: InsertCode(name regular expression, code, which=EXPORT_FUNCTION)
#---------------------------------------------------------------------------
InsertCodeEdits = []
def InsertCode(regex, record, which=EXPORT_FUNCTION):
    global InsertCodeEdits
    regex  = fixescape(regex)
    InsertCodeEdits.append((re.compile(regex), record))
    
def insertCode(headername, tab):
    global InsertCodeEdits
    for nameregex, record in InsertCodeEdits:
        if nameregex.search(headername) <> None:
            records = filter(lambda x: strip(x) <> "", split(record,'\n'))
            records = map(lambda x: rstrip(x), records)
            offset  = len(records[0]) - len(strip(records[0]))
            records = map(lambda x: '%s%s' % (tab,x[offset:]), records)
            record  = '\n%s// Added code\n' % tab + joinfields(records,'\n')
            return record
    return ""
#---------------------------------------------------------------------------
# Usage: Insert(header name regular expression, record[, BEFORE | AFTER])
#---------------------------------------------------------------------------
BEFORE=0
AFTER=1
InsertBeforeHeadersEdits = []
InsertAfterHeadersEdits  = []
def Insert(regex, record, where=BEFORE):
    global InsertBeforeHeadersEdits, InsertAfterHeadersEdits
    regex = fixescape(regex)
    if where == BEFORE:
        InsertBeforeHeadersEdits.append((re.compile(regex), record))
    else:
        InsertAfterHeadersEdits.append((re.compile(regex), record))
        
def insertBeforeHeaders(context, preamble):
    global InsertBeforeHeadersEdits
    for nameregex, record in InsertBeforeHeadersEdits:
        if nameregex.search(context['headername']) <> None:
            uniqueList(preamble, record)
            
def insertAfterHeaders(context, preamble):
    global InsertAfterHeadersEdits
    for nameregex, record in InsertAfterHeadersEdits:
        if nameregex.search(context['headername']) <> None:
            uniqueList(preamble, record)
#---------------------------------------------------------------------------
ExcludeHolderEdits = []
def ExcludeHolder(regex):
    global ExcludeHolderEdits
    regex = fixescape(regex)
    ExcludeHolderEdits.append(re.compile(regex))

def excludeHolder(classname):
    for nameregex in ExcludeHolderEdits:
        if nameregex.match(classname) <> None: return 1
    return 0
#---------------------------------------------------------------------------
# Usage: ExcludeInstantiationType(type regex)
#        In the automatic instantiation of vector and array types, the
#        specified type should be excluded.
#---------------------------------------------------------------------------
ExcludeInstantiationTypeEdits = []
def ExcludeInstantiationType(regex):
    global ExcludeInstantiationTypeEdits
    regex = fixescape(regex)
    ExcludeInstantiationTypeEdits.append(re.compile(regex))

def excludeInstantiationType(ttype):
    for nameregex in ExcludeInstantiationTypeEdits:
        if nameregex.match(ttype) <> None: return 1
    return 0
#---------------------------------------------------------------------------
NoncopyableEdits = []
def MakeNoncopyable(regex):
    Noncopyable(regex)
    
def Noncopyable(regex):
    global NoncopyableEdits
    regex = fixescape(regex)
    NoncopyableEdits.append(re.compile(regex))

def makeNoncopyable(classname):
    for nameregex in NoncopyableEdits:
        if nameregex.match(classname) <> None: return 1
    return 0

def isNoncopyable(classname):

    # Scan class
    
    element = classElement(classname)
    if element == None: return 0
    ctype = element.attrib['type']
    if find(ctype, 'copyctor') > -1: return 1

    # Scan base classes
    
    bases = DB.query('basenames', classname)
    
    for name in bases:
        element = classElement(name)
        if element == None: continue
        ctype = element.attrib['type']
        if find(ctype, 'copyctor') > -1: return 1

    # Scan nested classes
    
    children = DB.query('children', classname)
    for name in children:
        element = classElement(name)
        if element == None: continue
        ctype = element.attrib['type']
        if find(ctype, 'copyctor') > -1: return 1        
    return 0
#---------------------------------------------------------------------------
NonconstructableEdits = []
def MakeNonconstructable(regex):
    global NonconstructableEdits
    regex = fixescape(regex)
    NonconstructableEdits.append(re.compile(regex))

def makeNonconstructable(classname):
    for nameregex in NonconstructableEdits:
        if nameregex.match(classname) <> None: return 1
    return 0
#---------------------------------------------------------------------------
# Usage: DeclareCallback(header name regular expression, record)
#---------------------------------------------------------------------------
DeclareCallbackEdits = []
def DeclareCallback(regex, record):
    global DeclareCallbackEdits
    regex = fixescape(regex)
    record= standardizeName(record)
    DeclareCallbackEdits.append((re.compile(regex), record))

    updatecallbackDB(record)

    # Update typelist just in case we introduce a type that is not
    # explicitly in the header.
    
    DB.add(record)    
#---------------------------------------------------------------------------
def addCallbackToHeader(context):
    for nameregex, record in DeclareCallbackEdits:
        if nameregex.match(context['headername']):
            updatecallbackDB(record)
#---------------------------------------------------------------------------
def updatecallbackDB(record, code=not INLINE, skip=1):
    # Update callback map
    t = decodeCallback(record)
    if t <> None:
        retype, name, arglist, vars, qual = t
        DB.callbackDB[name] = [retype, arglist, code, skip]
#---------------------------------------------------------------------------
def translateCallbackTypes(context):
    for key in DB.callbackDB.keys():
        retype, arglist, code, skip = DB.callbackDB[key]
        retype = translateAllTypes(context, retype)

        for index, arg in enumerate(arglist):
            arg = translateAllTypes(context, arg)
            arglist[index] = arg
        DB.callbackDB[key] = [retype, arglist, code, skip]
#---------------------------------------------------------------------------
# Usage: DeclareNamespace()
#---------------------------------------------------------------------------
def DeclareNamespace(namespace):
    DB.namespaceDB[namespace]=1
#---------------------------------------------------------------------------
# Usage: InstantiateCallback(method name regular expression, number)
#---------------------------------------------------------------------------
CallbackCountEdits = []
def InstantiateCallback(regex, number):
    global CallbackCountEdits
    regex = fixescape(regex)
    CallbackCountEdits.append((re.compile(regex), number))

def callbackCount(routinename):
    for nameregex, number in CallbackCountEdits:
        if nameregex.search(routinename) <> None:
            return number
    return 2
#---------------------------------------------------------------------------
# Usage: Instantiate(class name regular expression, ttype, funregex=None)
#---------------------------------------------------------------------------
TemplateEdits = []
TemplateNames = []
def Instantiate(classregex, ttype, pyname='', funregex=None):
    global TemplateEdits, TemplateNames
    t = map(lambda x: strip(x), split(ttype,','))
    # Could just be a name
    unique(TemplateNames, (classregex, t, pyname, funregex==None)) 
    classregex = fixescape(classregex)
    if funregex <> None: funregex = re.compile(fixescape(funregex))
    TemplateEdits.append((re.compile(classregex), ttype, funregex))
#---------------------------------------------------------------------------
def templateTypes(classname, funname=None):
    ttypes = []
    if funname == None:
        for nameregex, ttype, funregex in TemplateEdits:
            if nameregex.match(classname) == None: continue
            ttypes.append(ttype)
    else:
        for nameregex, ttype, funregex in TemplateEdits:
            if funregex == None: continue
            if nameregex.match(classname) == None: continue
            if funregex.match(funname)    == None: continue
            ttypes.append(ttype)        
    return ttypes
#---------------------------------------------------------------------------
# Usage: Import(target class name regular expression,
#               class to be imported, shared library)
#---------------------------------------------------------------------------
ImportEdits = []
def Import(classregex, classname, sharedlib):
    global ImportEdits
    classregex = fixescape(classregex)
    ImportEdits.append((re.compile(classregex), classname, sharedlib))
#---------------------------------------------------------------------------
def importClass(classname):
    a = []
    for nameregex, name, sharedlib in ImportEdits:
        if nameregex.match(classname) == None: continue
        a.append((name, sharedlib))
    return a
#---------------------------------------------------------------------------
# Usage: ForceImport(class name regular expression)
#---------------------------------------------------------------------------
ForceImportEdits = []
def ForceImport(classregex):
    global ForceImportEdits
    classregex = fixescape(classregex)
    ForceImportEdits.append(re.compile(classregex))
#---------------------------------------------------------------------------
def forceImport(classname):
    for nameregex in ForceImportEdits:
        if nameregex.match(classname) != None: return True
    return False
#---------------------------------------------------------------------------
# Usage: DefineLength(class name regular expression, sizeString) 
#---------------------------------------------------------------------------
LengthEdits = []
def DefineLength(regex, record):
    global LengthEdits
    regex = fixescape(regex)
    LengthEdits.append((re.compile(regex), record))
    
def lenFunction(classname):
    global LengthEdits
    for nameregex, record in LengthEdits:
        if nameregex.match(classname) <> None:
            if find(record,'()') > -1: record = 'o.' + record
            return record 
    return ''
#---------------------------------------------------------------------------
# Usage: DefineArgType(class name|'', method, arg, argtype, size='1')
#---------------------------------------------------------------------------
ArgTypeEdits = []
def DefineArgType(classregex, methodregex, argregex, argtype, size='1'):
    global ArgTypeEdits
    if classregex != '': classregex  = re.compile(fixescape(classregex))
    methodregex = fixescape(methodregex)
    argregex    = fixescape(argregex)
    ArgTypeEdits.append((classregex,
                         re.compile(methodregex),
                         re.compile(argregex),
                         argtype,
                         size))
    
def ioType(classname, methodname, argname):
    global ArgTypeEdits
    for classregex, methodregex, argregex, argtype, size in ArgTypeEdits:
        if classregex == '':
            if methodregex.search(methodname) <> None:
                if argregex.search(argname) <> None:
                    return (argtype, size)
        elif classregex.search(classname) <> None:
            if methodregex.search(methodname) <> None:
                if argregex.search(argname) <> None:
                    return (argtype, size)
    return (INPUT,'1')
#===========================================================================
# Read edits files; first project-wide then local 
#===========================================================================
edfile = "%s/config/edits.py" % PROJECTPATH

try:
    if os.path.exists(edfile):
        execfile(edfile)
except:
    pass

try:
    if os.path.exists("edits.py"):
        execfile("edits.py")
except:
    fatal("Problem in file edits.py")

try:
    if os.path.exists(TEMPLATEFILE):
        execfile(TEMPLATEFILE)
except:
    fatal("Problem in file %s" % TEMPLATEFILE)    
#===========================================================================
# Given name, return class element from database
#---------------------------------------------------------------------------
def classElement(name):
    return DB.query('class', name)
#---------------------------------------------------------------------------
# Given a base class element, return name of base class, minus possible
# template decoration.
#---------------------------------------------------------------------------
getbasename = re.compile(r'(public |private |protected )? *'\
                         '(?P<name>[a-zA-Z].*)')

def extractbaseclassName(e):
    name = e.attrib['name']
    m = getbasename.match(name)
    if m == None: fatal("Can't get name from %s name" % name)
    name = m.group('name')
    return name
#---------------------------------------------------------------------------
def baseclassElements(element):
    bases = element.findall('baseclass')
    elist = []
    for e in bases:
        fullname = extractbaseclassName(e)
        name = strip(split(fullname,'<')[0])
        elem = classElement(name)
        if elem <> None: elist.append((elem, fullname))
    return elist
#---------------------------------------------------------------------------
def baseclassElementsAll(element, elist=[], depth=0):
    depth += 1
    if depth > 50: fatal('lost in trees: class %s' % element.atrib['name'])
    bases = element.findall('baseclass')
    for e in bases:
        name = extractbaseclassName(e)
        elem = classElement(name)
        if elem <> None:
            elist.append(elem)
            baseclassElementsAll(elem, elist, depth)
    return elist
#---------------------------------------------------------------------------
def findAllBaseElements(element, elist=[], depth=0):
    depth += 1
    if depth > 50: fatal('lost in trees: class %s' % element.atrib['name'])
    bases = element.findall('baseclass')
    for e in bases:
        name = extractbaseclassName(e)
        elem = classElement(name)
        if elem <> None:
            elist.append(elem)
            baseclassElementsAll(elem, elist, depth)
    return elist
#---------------------------------------------------------------------------
def baseclassNames(element):
    bases = element.findall('baseclass')
    nlist = []
    for e in bases:
        name = extractbaseclassName(e)
        nlist.append(name)
    return nlist
#---------------------------------------------------------------------------
# Create an XML object file to register an instantiated template.
#---------------------------------------------------------------------------
def writeXMLfile(names):
    str = '''
<?xml version="1.0"?>
<!--
==============================================================================
  XML File: %s.xml
  Created:  %s
==============================================================================
-->
<header name="%(headername)s"
        file="%(headerfile)s">

    <class name="%(classname)s"
               fullname="%(classname)s"
               module="%(module)s"
               project="%(project)s"
               type="instantiatedtemplate">

    <section name="public">
       <constructor name="%(classname)s" return="" args="[]" vars="[]" qual="">
       %(classname)s();
       </constructor>
    </section>
    </class>"
</header>
    ''' % names
    open('%(xmlfile)s' % names,'w').write(str)
#---------------------------------------------------------------------------
# A template class could define a hierarchy of typedefs. For example, the
# class could define something like
#
# typedef std::string String;
# typedef String Name;
#
# This procedure tries to extract them.
#---------------------------------------------------------------------------
def typedefTypenames(context, elem, typenames, depth=0):
    depth += 1
    if depth == 1:
        context['fname'] = elem.attrib['name'] + templateDecoration(elem)
        
    if depth > 50:
        fatal('lost in tree...! tag: %s' % elem.tag)

    for element in elem.getchildren():

        if element.tag == 'section':            
            typedefTypenames(context, element, typenames, depth)

        elif element.tag == 'class':
            tname = element.attrib['name']
            if tname in context['typelist']:
                context['tname'] = tname
                t = TAB1 + 'typedef typename '\
                    '%(fname)s::%(tname)s %(tname)s; // nested class' % context
                unique(typenames, t)
                    
        elif element.tag == 'typedef':
            record = strip(element.text)
            record = replace(record,'typedef','')
            record = replace(record,'typename','')
            record = replace(record,';','')
            tname = split(record).pop()
            if tname in context['typelist']:
                context['tname'] = tname
                t = TAB1 + 'typedef typename '\
                    '%(fname)s::%(tname)s %(tname)s;' % context
                unique(typenames, t)
    return
#---------------------------------------------------------------------------
def createMethod(method, retype, name, pyname, args, vars, text):
    
    # Create a new method element

    cname  = convert2html(name)
    cargs  = convert2html(args)
    cvars  = convert2html(vars)
    cretype= convert2html(retype)
    ctext  = convert2html(text)
    qual   = ''
    vars   = '[]'
    str = '''
    <%s name="%s"
             pyname="%s"
             return="%s"
             args="%s"
             vars="%s"
             qual="%s">
    %s
    </%s>
        ''' % (method, cname, pyname,
               cretype, cargs, cvars, qual, ctext, method)
    return XML(str)    
#---------------------------------------------------------------------------
def instantiateFunction(elem, element, context):
    
    classname= context['classname']
    retype   = element.attrib['return']    
    name     = element.attrib['name']
    
    template = joinfields(templateParList(retype),', ')

    inform("template method %s" % name)

    # Check for template instants. If none return
    ttypes = templateTypes(classname, name) 
    if len(ttypes) == 0:
        inform("nothing to instantiate!")
        return 

    # Remove template decoration from function declaration

    t = findTemplates(retype)
    end  = t[0][4] # End of "template<...>"
    retype = strip(retype[end:])

    text = element.text
    t = findTemplates(text)
    end  = t[0][4] # End of "template<...>"    
    text = strip(text[end:])
    
    # Get remaining function elements
    
    vars   = element.attrib['vars']
    args   = element.attrib['args']
    qual   = element.attrib['qual']

    # Get list of template parameter names
    
    tpars = map(lambda x: strip(x), split(template,','))
    tparnames = map(lambda x: re.compile(r'\b%s\b' % x,re.M), tpars)

    # Instantiate each type in types
    # Note: type may in fact be multiple types
    for ttype in ttypes:
        # We may have a multiple fields 
        subtypes = split(ttype)

        n = name
        pn= name
        a = args
        v = vars
        r = retype
        t = text
        nochange = 0
        delim = '< '        
        for index, s in enumerate(subtypes):            
            A = tparnames[index].sub(s, a)
            r = tparnames[index].sub(s, r)
            t = tparnames[index].sub(s, t)
            n = n + delim + fixbadstl(s)
            pn= pn + capitalize(renameIdentifier(s))
            delim = ', '
            if a == A:
                nochange += 1
            a = A
        n = n + ' >'

        # if arguments are unchanged, then assume we must
        # keep the function name unchanged.
        
        if nochange <> len(subtypes):
            inform("instantiate %s" % n)
            
            # Create a new method element
        
            newelement = createMethod('method2',r,n,pn,a,v,t)
        else:
            n = replace(n,'<','')
            n = replace(n,'>','') 
            inform("instantiate %s" % n)
            
            # Create a new method element
        
            newelement = createMethod('method2',r,name,pn,a,v,t)            

        elem.append(newelement) # Add to current element
    return
#---------------------------------------------------------------------------
# Deconstruct argument list. Find start of default arguments if present
# and determine number of wrappers to write.
#---------------------------------------------------------------------------
def getArgs(args, routinename=""):
    arglist = []
    
    for index, arg in enumerate(args):
        if arg <> 'void':
            arglist.append(arg)
        
    arguments = []
    nwrap = 1
    first = 1
    kdefault = len(arglist)
    for index, arg in enumerate(arglist):
        arguments.append(arg)
        tmp = split(arg,'=')
        if len(tmp) == 2:
            arg = tmp[0]
            arguments[index] = arg
            nwrap += 1
            if first:
                first = 0
                kdefault = index
            if len(arguments) == (MAXARGS-1): break
    return (nwrap, kdefault, arguments)
#===========================================================================
# Convert a struct into a class with a default constructor if one does
# not already exist and insert into a public section.
#---------------------------------------------------------------------------
def updateStruct(element):
    classname = element.attrib['name']
    sections = element.findall('section')
    if len(sections) == 0: return

    # Check for constructors. Also, if struct is un-named, assume it
    # is public
    for section in sections:
        if not section.attrib.has_key('name'): section.attrib['name']='public'
        
        ctors = section.findall('constructor')
        if len(ctors) > 0: return

    # No constructors found, so add one

    cname = convert2html(classname)
    str = '''
    <section name="public">
       <constructor name="%s" return="" args="[]" vars="[]" qual="">
       %s();
       </constructor>
    </section>
    ''' % (cname, cname)
    cstr = XML(str)
    element.append(cstr) # Add to struct
#---------------------------------------------------------------------------
# Add a default public constructor to class if one does not exist explicitly
# but only if the class contains no private or protected constructors or
# destructors. Under this circumstance the compiler will create a default
# constructor.
#---------------------------------------------------------------------------
def updateClass(element):

    # If abstract class then just return

    classtype = element.attrib['type']
    if find(classtype, 'abstract') > -1: return

    # Get public constructors

    ctors = findElements(element, 'constructor')
    if len(ctors) > 0: return

    # Class contains no explicitly declared public constructors.
    # Return if constructors or destructors are private or protected.

    if find(classtype, 'ctor') > -1: return
    if find(classtype, 'dtor') > -1: return
    
    # No constructors have been declared explicitly and none is private
    # or protected. Since the compiler will create a default constructor,
    # we'll add one to the class element.

    classname = element.attrib['name']
    cname = convert2html(classname)
    str = '''
    <section name="public">
       <constructor name="%s" return="" args="[]" vars="[]" qual="">
       %s();
       </constructor>
    </section>
    ''' % (cname, cname)
    cstr = XML(str)
    element.append(cstr) # Add to class
#---------------------------------------------------------------------------
# A template class could define a hierarchy of typedefs. For example, the
# class could define something like
#
# typedef std::string String;
# typedef String Name;
#
# This procedure tries to extract them.
#---------------------------------------------------------------------------
def typedefTypenames(context, elem, typenames, depth=0):
    depth += 1
    if depth == 1:
        context['fname'] = elem.attrib['name'] + templateDecoration(elem)
        
    if depth > 50:
        fatal('lost in tree...! tag: %s' % elem.tag)

    for element in elem.getchildren():

        if element.tag == 'section':            
            typedefTypenames(context, element, typenames, depth)

        elif element.tag == 'class':
            tname = element.attrib['name']
            if tname in context['typelist']:
                context['tname'] = tname
                t = TAB1 + 'typedef typename '\
                    '%(fname)s::%(tname)s %(tname)s; // nested class' % context
                unique(typenames, t)
                    
        elif element.tag == 'typedef':
            record = strip(element.text)
            record = replace(record,'typedef','')
            record = replace(record,'typename','')
            record = replace(record,';','')
            tname = split(record).pop()
            if tname in context['typelist']:
                context['tname'] = tname
                t = TAB1 + 'typedef typename '\
                    '%(fname)s::%(tname)s %(tname)s;' % context
                unique(typenames, t)
    return
#---------------------------------------------------------------------------
# Create fully qualified scopes and wrapper prefixes.
#---------------------------------------------------------------------------
def updateScopename(context):
    if context['scope'] <> []:
        context['fullname']  = joinfields(context['scope'],'::')
        context['scopename'] = context['fullname'] + '::'
    else:
        context['fullname']  = ''
        context['scopename'] = ''

    str = striptpar.sub('', context['scopename'])
    context['scopeprefix'] = replace(str,'::','_')

    # Check if we should prefix with "typename"
    
    if len(context['scope']) == 0: return
    
    if not context.has_key('classelement'): return

    element = context['classelement']
    if element == None: return
    
    name = context['scope'][-1]
    if name in ['iterator','const_iterator']:

        if element.attrib['iterableBycontainment']:
                    
            # If we have a type of the form
            # <name>< template pars >::<other name> then make sure
            # it is preceded by a "typename"
            
            if element.attrib.has_key('template') and \
                   element.attrib['template'] <> '':
                if context['fullname'][0:8] <> 'typename':
                    context['fullname'] = 'typename ' + context['fullname']
#---------------------------------------------------------------------------
def dumpclassDB(classname=''):
    if classname == '':
        print
        for element in DB.classelements:
            print "%s\nCLASSDB( %s )" % (element,
                                         element.attrib)
            print 79*'-'
            print ""
    elif DB.classDB.has_key(classname):
        print
        print "%s\nCLASSDB( %s )" % (DB.classDB[classname],
                                     DB.classDB[classname].attrib)
        print 79*'-'
        print ""
#---------------------------------------------------------------------------
# Find specified elements within given element
#--------------------------------------------------------------------------    
def findElems(elem, name, publicOnly, depth=0):
    depth += 1    
    if depth > 50:
        fatal("findElems - lost in trees")
    elements = []
    for element in elem.getchildren():
        if element.tag == name:
            elements.append(element)

        elif element.tag == 'section':
            if publicOnly:
                if element.attrib.has_key('name'):
                    if element.attrib['name'] == 'public':
                        elements = elements + \
                                   findElems(element, name,
                                             publicOnly, depth)
            else:
                elements = elements + \
                           findElems(element, name,
                                     publicOnly, depth)
    return elements
#---------------------------------------------------------------------------
def findElements(element, name, publicOnly=1):
    elements = findElems(element, name, publicOnly) 
    if len(elements) > 0:
        if elements[0].attrib.has_key('name'):
            elements.sort(cmpelement)
    return elements
#---------------------------------------------------------------------------
def findAllElements(elem, tag, depth=0):
    depth += 1
    if depth > 50: fatal("findAllElements - lost in trees")
    elements = []
    for element in elem.getchildren():
        if element.tag == tag: elements.append(element)
        elements = elements + findAllElements(element, tag, depth)
    return elements
#---------------------------------------------------------------------------
# A compare function for ordering elements
def cmpelement(x, y):
    if   x.attrib['name'] < y.attrib['name']:
        return -1
    elif x.attrib['name'] > y.attrib['name']:
        return 1
    else:
        return 0
#---------------------------------------------------------------------------
# Insert a blank line if number of elements is non-zero
#---------------------------------------------------------------------------
def addBlankline(context, elements):
    if len(elements) > 0:
        context['cpprecords'].append('')
#---------------------------------------------------------------------------
# Handle identifiers that are, in fact, the names of template classes, but
# which do not have the appropriate template decoration.
#---------------------------------------------------------------------------
def fixTemplateName(context, word, name):

    # If class is not a template class do nothing
    
    if context['templatepar'] == '': return name
    
    # We have a template class. If word is followed by a
    # template decoration do nothing.

    if re.search(r'\b%s\b(?=\<)' % word, name) <> None: return name
    
    # We have a template class and word has no template decoration.
    # But, if word is not in the full name of current class do nothing

    if re.search(r'\b%s\b' % word, context['fullname']) == None: return name

    # Word is in full name of a template class and it has no template
    # decoration, therefore, replace word with the full class name.

    name = re.sub(r'\b%s\b(?!=\<)' % word, context['fullname'], name)
    return name
#---------------------------------------------------------------------------
# Use the typemap to translate a type, recursively, for current class.
# 1. First split name into words.
# 2. If a word is the same as the current class name continue unless the
#    class is a template class and the word is *not* followed by the
#    template decoration, in which case replace the word by the full name of
#    the template class.
# 3. If a word pair is of the form classname::word then continue.
# 4. Check if word is in typemap. If so, translate. But do not translate
#    if the word is "iterator" and if it is bound either to an stl container
#    type or if the current class contains its own iterator class.
# 5. Do not translate if size_type, reference etc. are bound to stl
#    types list, vector, pair, map.
# 6. If a translation yields a simple C-type preceded by "<identifier>::"
#    then assume that all we really want is a simple C-type
#---------------------------------------------------------------------------
stripCtype = re.compile('(?P<name>\\b[a-zA-Z]\w+\\b::)'\
                        '(?=\\b(void|int|short|float)\\b)')
#---------------------------------------------------------------------------
# Find all templated identifiers within given record                        
def findTemplates(record):
    return findComponents(templates,record,'<','>')
#---------------------------------------------------------------------------
def findAndupdateTemplatemap(context, record):
    try:
        if context.has_key("templateparlist"):
            A = Set(context["templateparlist"])
        else:
            A = Set([])
    
        tids = findTemplates(record)
        for name, g, s, l, r, e in tids:
            name = strip(name)
            #B = Set(templateParList(name))
            B = Set(swords.findall(name))
            if A.intersection(B) != Set([]): continue

            newname = renameIdentifier(name)
            if newname <> name:
                if not context['templatemap'].has_key(newname):
                    context['templatemap'][newname] = name
                    debug(2,["\t%s --> %s" % (name,newname)],0)
    except:
        pass
#---------------------------------------------------------------------------
def updateTemplatemap(context, name):
    newname = renameIdentifier(name)
    if newname == name: return name
    tids = findTemplates(name)
    if len(tids) == 0: return name
    
    if context.has_key("templateparlist"):
        A = Set(context["templateparlist"])
    else:
        A = Set([])
    
    #B = Set(templateParList(name))
    B = Set(swords.findall(name))        
    if A.intersection(B) == Set([]):
        if newname <> name:
            if not context['templatemap'].has_key(newname):
                context['templatemap'][newname] = name

    return newname
#---------------------------------------------------------------------------
acstring = re.compile(r'\"[^"]*\"') 
def translateTypes(context, element):

    updateScopename(context)

    if SHOWTRANS:
        print "\tFUNCTION( %s )" % element.attrib['name']
        print "%s\n" % strip(element.text)
        
    retype = element.attrib['return']
    retype = translateAllTypes(context, retype)
    retype = translateAllTypes(context, retype)
    retype = stripCtype.sub("",retype)
    retype = fixbadstl(retype)
    element.attrib['return'] = retype # Translated
    findAndupdateTemplatemap(context, retype)

    for index, arg in enumerate(element.attrib['args']):
        arg = translateAllTypes(context, arg)
        arg = translateAllTypes(context, arg)
        arg = stripCtype.sub("", arg)
        arg = fixbadstl(arg)
        element.attrib['args'][index] = arg

        # Remove possible default values

        arg = stripDefaultValues(arg)
        findAndupdateTemplatemap(context, arg)    
#---------------------------------------------------------------------------
fixstl = re.compile(r'\w+::(?=std::)')
def fixbadstl(name):
    m = badstl.search(name)
    if m <> None:
        t = badstl.findall(name)
        for x in t:
            regex = '(?<!std::)\\b%s\\b' % x
            name = re.sub(regex, 'std::%s' % x, name)
    return name
def fixtemplate(name):
    #name = replace(name,'< ','<')
    #name = replace(name,' >','>')
    #name = fixstl.sub("",name)
    #name = fixbadstl(name)
    name = replace(name,'>>','> >')
    return name
#---------------------------------------------------------------------------
fixpairs = re.compile(r'\w+::\w+')
def fixscope(context, name):
    pairs = fixpairs.findall(name)
    if pairs == []: return name
    
    # if B is a class, then make sure that A is in B or in a base class
    # of B, that is, make sure that A::B is a valid identifier. If it is not,
    # remove A::

    typemap = context['typemap']
    for pair in pairs:

        if SHOWTRANS: print "\t\tCHECK_IDENTIFIER( %s )" % pair

        # If A::B is present in the values of the current typemap
        # then, presumably, A::B is a valid typemap.
        if pair in typemap.values(): continue

        # A::B is not present in typemap values. Assuming A is a class
        # check its map of identifiers.

        A, B = split(pair,'::')
        
        element = DB.query('class', A)
        if element == None: continue

        # A is a known class. Make sure identifier B is in A

        if B in element.attrib['identifiers'].keys(): continue

        if SHOWTRANS: print "\t\t\tCHECK FAILED"

        # B is not in A, so strip away A:: ... and hope for the best!

        name = replace(name, pair, B)

        if SHOWTRANS: print "\tFIXEDNAME( %s )" % name

    return name
#---------------------------------------------------------------------------
def translateAllTypes(context, name):

    oldname = name

    if context.has_key('classelement'):
        element = context['classelement']
    else:
        element = None
        
    # Make a unique set of words, so that we don't translate a word more
    # than once. Also, do not translate template parameters.

    if SHOWTRANS: print "CLASS( %s )TYPE( %s )" % (context['classname'],name)
    
    wds = swords2.findall(name)
    
    words = []
    for word in wds:

        # Ignore strings
        if word[0] == '"': continue
        
        # If word is of form (*word) then don't translate it
        if word[0] == '(': continue

        # Skip C++ keywords and native C types

        if isCkeywordNative.search(word) <> None: continue

        # If word is a template parameter of the current class then
        # don't translate it.
        if word in context['templateparlist']:
            if SHOWTRANS: print "\tTEMPLATEPAR( %s )" % word
            continue
        if word not in words: words.append(word)

        
    for index, word in enumerate(words):

        if index > 0:
            previousword = words[index-1]
        else:
            previousword = ''

        if SHOWTRANS: print "\tWORD( %s )" % word
        
        # If word is classname and is the name of a template class,
        # translate classname unless the result would be
        # classname<templatepar><templatepar>

        if word == context['classname']:
            name = fixTemplateName(context, word, name)
            
        # If previous word is same as classname and the current word
        # is ajoined to it with "::" then continue

        if index > 0:
            if previousword == context['classname']:
                fullname = '%s::%s' % (previousword,word)
                if find(name, fullname) > -1:
                    if SHOWTRANS: print "\t\tfullname( %s )" % fullname, name
                    continue
        
        # If previous word is a namespace known to DB.namespace
        # don't translate
        
        if index > 0:
            fullname = '%s::%s' % (previousword,word)
            if find(name, fullname) > -1:
                if DB.namespaceDB.has_key(previousword):
                    if SHOWTRANS: print "\t\tnamespace( %s )" % previousword
                    continue
        
        # Check for a fully expanded stl type. If it is already
        # fully expanded then do not translate!

        regex = stlattribtype % word
        m = re.search(regex, name)
        if m <> None:
            if SHOWTRANS: print "\tstl type( %s )" % name
            continue
        
        # First check typemap specific to class, then
        # the global typemap

        if context['typemap'].has_key(word):
            fullname = context['typemap'][word]
            if SHOWTRANS: print "\ttypemap: %s <-- %s" % (fullname, word)

        elif context['globaltypemap'].has_key(word):
            key = word
            count = 0
            while context['globaltypemap'].has_key(key) and count < 4:
                fullname = context['globaltypemap'][key]
                if SHOWTRANS: print "\tglobaltypemap: %s <-- %s" % \
                   (fullname, key)
                key = fullname
                count += 1
            
        else:
            if SHOWTRANS: print "\t\tword ( %s ) NOT found in typemaps" % word
            continue

        # If fullname is an enum, don't translate

        if enumtype.match(fullname) <> None:
            if SHOWTRANS:
                print "DO NOT TRANSLATE( %s )" % name
            continue

        # If word is iterator, just replace with full name.
        
        if word in ['iterator','const_iterator']:

            if element <> None:

                if element.attrib['iterableBycontainment']:

                    if SHOWTRANS: print "\tITERABALE BY CONTAINMENT"
                    
                    # If we have a type of the form
                    # <name>< template pars >::<other name> then make sure
                    # it is preceded by a "typename"
                    
                    if element.attrib.has_key('template') and \
                           element.attrib['template'] <> '':
                        if fullname[0:8] <> 'typename':
                            fullname = 'typename ' + fullname

            name = fullname 
            continue

        # Okay, maybe we can translate!

        name = re.sub(r'\b%s\b' % word, fullname, name)

        # Do misc. fixups
        
        name = re.sub(r'\bconst const\b', 'const', name)
        
        if SHOWTRANS: print "\tNEWNAME( %s )" % name


    if SHOWTRANS: print "\tbefore FIXSCOPE( %s )" % name
    # Do more misc. fixups
    if name <> oldname: name = fixscope(context, name)

    if SHOWTRANS: print "\tbefore FIXTEMPLATE( %s )" % name
    name = fixtemplate(name)
                     
    if SHOWTRANS:
        print "FINAL TRANSLATION( %s )" % name
        print MAXWIDTH*'-'

    # We call this to make sure we add correct headers
    # If a header is not in classDB try to get it from
    # headermap

    DB.addInclude(name, 'TRANS')
    
    return name
#---------------------------------------------------------------------------
# Try to perform a reasonably complete analysis of return and argument
# types. We do this so that we can make an educated guess about how the
# types should be handled.
#---------------------------------------------------------------------------
def analyzeReturnAndArguments(context, element):

    updateScopename(context)

    analyzeCallbackArguments(context, element)
    
    #-------------------------------------
    # Analyze return
    #-------------------------------------
    retype = element.attrib['return'] # Get translation
    lowercaseretype = lower(retype)
    element.attrib['oldreturn'] = retype
    element.attrib['newreturn'] = retype
    element.attrib['ttypereturn'] = ''
    element.attrib['simpleReturnType'] = 0
    element.attrib['returnByPointer']  = 0
    element.attrib['vectorReturnType'] = 0

    funcname = element.attrib['name']
    functext = element.text
    
    if   retype in ['void*', 'const void*', 'void* const']:
        element.attrib['newreturn'] = '%(opaque)s*' % context # opaque pointer
        context['addopaque'] = 1
    
    elif retype == 'FILE*':
        element.attrib['newreturn'] = 'PyObject*'

    elif retype == 'const char*':
        element.attrib['newreturn'] = retype

    elif retype == 'char*':
        element.attrib['newreturn'] = 'const char*'

    elif retype == 'char**':
        element.attrib['newreturn'] = 'boostutil::array< const char* >'
        element.attrib['ttypereturn'] = 'const char*'
        element.attrib['simpleReturnType']= 1
        element.attrib['returnByPointer'] = 1
        
    elif simpletype.match(lowercaseretype) <> None:

        # Simple type other than const char* or char*

        element.attrib['simpleReturnType']= 1

        match = simpletype.match(retype)
        if match != None:
            atype = strip(match.group('name'))
        else:
            atype = strip(retype)
                          
        element.attrib['returnByPointer'] = atype[-1] == "*"

        if element.attrib['returnByPointer']:
            ttype = atype[:-1]
            element.attrib['ttypereturn'] = ttype
            # check for const
            if ttype == 'const char*' or findconst.search(ttype) == None:
                element.attrib['newreturn'] = "boostutil::array< %s >" % ttype
            else:
                element.attrib['newreturn'] = "boostutil::"\
                                              "array_of_const_types< %s >" % \
                                              ttype
            if not ttype in context['arraystowrap']:
                context['arraystowrap'].append(ttype)

        elif atype[-1] == '&':
            element.attrib['newreturn'] = atype[:-1]
        else:
            element.attrib['ttypereturn'] = atype

    elif vectortype.search(retype) <> None:
        atype = strip(vectortype.search(retype).group('name'))
        ttype = strip(vectortype.search(retype).group('type'))
        if ttype == 'string': ttype = 'std::string' # HACK for stl string
        unique(context['vectorstowrap'], ttype)

        if mapVectorToList(functext):
            log(functext, "Allow vector to list conversions")
            #element.attrib['return'] = atype
            element.attrib['newreturn'] = "PyObject*"
            element.attrib['ttypereturn'] = ttype 
            element.attrib['vectorReturnType']= 1
            element.attrib['returnByPointer'] = atype[-1] == "*"
        
    elif vtype.search(retype) <> None:        
        atype = strip(vtype.search(retype).group('name'))
        ttype = strip(vtype.search(retype).group('type'))
        if ttype == 'string': ttype = 'std::string' # HACK for stl string
        unique(context['vectorstowrap'], ttype)

        if mapVectorToList(functext):            
            #element.attrib['return'] = atype
            element.attrib['newreturn'] = "PyObject*"
            element.attrib['ttypereturn'] = ttype 
            element.attrib['vectorReturnType']= 1
            element.attrib['returnByPointer'] = atype[-1] == "*"

    elif pointertype.search(element.attrib['return']):
        element.attrib['returnByPointer'] = 1
        
    #-------------------------------------
    # Analyze arguments
    #-------------------------------------    
    vars = element.attrib['vars']
    args = element.attrib['args']
    context['routinename'] = element.attrib['name']
    context['routinetext'] = element.text
    element.attrib['argsanalysis'] = []

    if len(vars) != len(args):
        print element.attrib
        print vars
        print args
        fatal("problem with args and vars attributes")
    
    for index, arg in enumerate(args):
        var = vars[index]

        # Append default and original names to args analysis object
        
        v = analyzeArgument(context, arg, var)
        v.append("a%d" % index)    # Default argument name
        if index < len(element.attrib['vars']):
            v.append(vars[index])  # Original argument name
        else:
            v.append("")
        element.attrib['argsanalysis'].append(v)
#---------------------------------------------------------------------------
# Analyze and possibly change argument type and return results of analysis.
#---------------------------------------------------------------------------
def analyzeArgValue(arg, checkForValue=1):

    # Do a very dumb check for time being: FIXME
    if checkForValue:
        tmp = splitfields(arg,'=',1) # Optional argument could be present
    else:
        tmp = [arg]    
    if len(tmp) == 1:
        arg = strip(tmp[0])
        val = ''
    else:
        arg, val = tmp
        val = replaceValue(val)      # Possibly need to replace value
        arg = strip(arg)
        val = '='+strip(val)         # NB: Prefix with "="

    oldarg = arg                     # We need to return the unadulterated type
    castarg = '(%s)' % oldarg        # Type cast operator
    return (oldarg, castarg, arg, val)
#---------------------------------------------------------------------------
# context  context
# arg    argument to be analyzed
# index  ordinal value of routine
#
# Returns
#   1. argtype         argument type (void*, simple*, vector, vtype, '')
#   2. oldarg          original argument (except in case of inline callback)
#   3. arg             new argument type if special processing warranted
#   4. castarg         type cast operator
#   5. val             '=<default value>' or ''
#   6. ttype           template type or simple type if argument is a pointer
#   7. callbackname    name of callback or ''
#   8. iotype
#   9. iosize          number of instances of given type
#  10. byvalue
#  11. var             variable name
#
# Needs:
#   1. context['callbacks'], context['classname'],
#      context['routinename'], context['routinetext']
#---------------------------------------------------------------------------
def analyzeArgument(context, arg, var):

    classname   = context['classname']
    routinename = context['routinename']
    routinetext = context['routinetext']
    
    prefix = lower(routinename)
    
    oldarg, castarg, arg, val = analyzeArgValue(arg)
    
    # Handle simple type mapping, e.g., const SoPath& --> SoPath*

    context['typemapped'] = 0
    if routinename <> '':
        arg = mapType(classname, routinename, arg)         
        if arg <> oldarg:
            context['typemapped'] = 1
            if oldarg[-1] == '&' and arg[-1] == '*':
                castarg = '(%s)*' % oldarg
            inform("map %s to %s in %s" % (oldarg, arg, routinename))
        
    namekey = split(stripName(arg),'::').pop()    

    if arg[0:5] == 'std::':
        ttype = arg
    else:
        ttype   = namekey
    callbackname = ''
    argtype = ''

    # INPUT, OUTPUT, INOUT [+ BYVALUE]
    argvar = arg+' '+var
    iotype, iosize = ioType(classname, routinename, argvar)
    byvalue= iotype / 4
    iotype = iotype - 4 * byvalue

    # Decide whether we have a getter or a setter
    
    isaGetter = prefix[0:3] == 'get' or \
                prefix[0:4] == 'read' or \
                prefix[0:6] == 'return'

    isaSetter = prefix[0:3] == 'set' or \
                prefix[0:5] == 'write'
    
    #-------------------------------------------------        
    # Determine argument type
    #-------------------------------------------------

    if DB.callbackDB.has_key(namekey):
        argtype = 'callback'
        callbackname = namekey
        arg = "PyObject*"

    elif arg == "...":
        argtype = 'varargs'
        context['addcstdarg'] = 1          # Need to include <cstdarg> header
        arg = "va_list"
        
    elif arg == "void":
        argtype = 'void'

    elif arg == "void*":
        argtype = 'void*'
        arg = "PyObject*"

    elif arg == "const void*":
        argtype = 'void*'
        arg = "PyObject*"

    elif arg == "void* const":
        argtype = 'void*'
        arg = "PyObject*"
        
    elif arg == "void**":
        argtype = 'unsupported'

    elif arg == "char***":
        argtype = 'unsupported'
        
    elif find(arg, 'void*&') > -1:
        argtype = 'unsupported'
        
    elif arg in ["const char*", "const char* const"]:
        pass

    elif arg == "char*":

        if iotype == OUTPUT:
            argtype = 'output'
            ttype = "char"
        
    elif arg == "FILE*":
        argtype = 'file*'
        arg = "object&"

    #elif arg == "std::ostream": # DON'T KNOW HOW TO HANDLE THIS YET
    #    argtype = 'file*'
    #    arg = "object&"
    #    var = 'a%d' % index        

    elif rfind(arg, '*&') > -1:  # Return items in a list
        argtype = 'output'
        ttype   = oldarg[:-2]
        iotype  = OUTPUT
        
    elif find(arg, '[') > -1 and rfind(arg, ']') > -1:

        if simpletype.match(lower(arg)) == None:
            argtype = 'unsupported'

        elif array2d.search(arg) <> None:
            argtype = 'unsupported'

        else:
            argtype = 'simple*'
            arg = 'object&'
            ttype = strip(split(oldarg,'[')[0])
            castarg = split(castarg,'[')[0]+'*)'

    elif simpletype.match(lower(arg)) <> None:

        argtype = 'simple'
        
        # Default for simple types is by value

        byvalue = 1

        if pointer.search(arg) <> None:
            arg = 'object&'

            if ppointer.search(oldarg) <> None:
                argtype = 'simple**'
                ttype = ppointer.sub('',oldarg)
            else:
                argtype = 'simple*'
                ttype = pointer.sub('',oldarg)
                
        elif arg[-1] == '&':

            # This is a simple type, by reference

            byvalue = 0
            ttype  = oldarg[:-1]
            arg = "object&"
            castarg = '(%s)' % ttype
            
            if isaGetter:
                iotype = OUTPUT # Set to output only
                
    elif vectortype.search(arg) <> None:
        argtype = 'vector'
        if mapVectorToList(routinetext):
            argtype = 'mapvector'
            arg = 'object&'
               
        ttype = strip(vectortype.search(oldarg).group('type'))
        unique(context['vectorstowrap'], ttype)
            
        if ttype in STL_TYPES:
            ttype = 'std::' + ttype
        
    elif vtype.search(arg) <> None:
        argtype = 'vtype'        
        if mapVectorToList(routinetext):
            argtype = 'mapvtype'
            arg = 'object&'

        ttype = strip(vtype.search(oldarg).group('type'))        
        unique(context['vectorstowrap'], ttype)            
            
        if ttype in STL_TYPES:
            ttype = 'std::' + ttype

    elif rfind(arg,'**') > -1:
        argtype = 'unsupported'
        
    # If arg is of type object& and val is 0 or NULL, replace with
    # the object "null"
    if arg == "object&" and (val == "=0" or val == "=NULL"):
        val = "=null"
        context['addnull'] = 1        

    return [context['typemapped'],
            argtype, oldarg, arg, castarg,
            val, ttype, callbackname, iotype, iosize, byvalue]
#---------------------------------------------------------------------------
findactualtypes = re.compile(r'[^,]+')
def findActualTypes(name):
    if name[0]  == '<': name = name[1:]
    if name[-1] == '>': name = name[:-1]
    tlist = findactualtypes.findall(name)
    for index, t in enumerate(tlist):
        tlist[index] = strip(t)
    return tlist
#---------------------------------------------------------------------------
def dissectTemplate(template):
    stemplate = strippedTemplate(template)
    tlist = templateParList(stemplate)
    for index, t in enumerate(tlist):
        tlist[index] = simplectype.sub("", t)
    return (tlist, stemplate)
#---------------------------------------------------------------------------
def analyzeTemplate(element):
    attrib = element.attrib

    template = attrib["template"]
    tlist, stemplate = dissectTemplate(template)

    # Get rid of possible simple C - types in template decor

    templatepar = fixtemplate(templateDecoration(element))
    templatepar = simplectype.sub("", templatepar)

    element.attrib['strippedtemplate']= stemplate
    element.attrib['templatepar']     = templatepar
    element.attrib['templateparlist'] = tlist


 
    

    
