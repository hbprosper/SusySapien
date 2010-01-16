#!/usr/bin/env python
#------------------------------------------------------------------------------
# File: wrap.py
# Description: Wrap a simple function
# Created:     October-2003 Harrison B. Prosper
# $Revision: 1.1.1.1 $
#------------------------------------------------------------------------------
import os, sys, re
from string     import *
from getopt     import getopt, GetoptError
from glob       import glob
from header2xml import \
     findFunctions, \
     parseFunctionBodies, \
     decodeMethodName, \
     whitespace
#------------------------------------------------------------------------------
# Constants
#------------------------------------------------------------------------------
include = re.compile(r"^# *\binclude\b")
using   = re.compile(r"^ *using namespace [a-zA-Z]\w*$")
comment = re.compile(r"(?!</)(?P<comment>//.*)$")
skipfun = re.compile(r"^(\binline\b|\bextern\b)")

MAXARGS = 15 # Maximum number of arguments supported by Boost Python
shortOptions = 'o:' 
longOptions  = ['help']

USAGE = '''
Usage:
 wrap.py [-o <module-name>] cpp-file
'''

COMPILE = '''
g++ -c -O2 %(name)s.cpp
'''

COMPILE_WRAPPER = '''
g++ -c -Os export_%(name)s.cpp \
 -DBOOST_PYTHON_DYNAMIC_LIB \
 -I. \
 -I%(boostinc)s \
 -I%(pythoninc)s
'''

LINK = '''
g++ -shared \
%(name)s.o \
export_%(name)s.o %(boostlib)s -o %(module)s.so
rm -rf *%(name)s.o
'''

OVERLOAD = 'BOOST_PYTHON_FUNCTION_OVERLOADS(%(name)s_OVL, '\
           '%(name)s, %(min)s, %(max)s)\n'

DEF_WITH_OVERLOAD = '  def("%(name)s", %(name)s, %(name)s_OVL());\n'
DEF = '  def("%(name)s", %(name)s);\n'

WRAPPER = \
'''//--------------------------------------------------------------------------
#include <boost/python.hpp>
%(includes)s
using namespace boost::python;
//--------------------------------------------------------------------------
%(declaration)s
//--------------------------------------------------------------------------
%(overloads)s
//--------------------------------------------------------------------------
BOOST_PYTHON_MODULE(%(module)s)
{
%(defs)s
}
'''


#------------------------------------------------------------------------------
# Functions
#------------------------------------------------------------------------------
def usage():
    print USAGE
    sys.exit(0)    
def quit(s):
    print "\n**error** %s" %s
    print "\tgoodbye!"
    sys.exit(1)
def nameonly(s):
    return os.path.splitext(os.path.split(s)[1])[0]

#------------------------------------------------------------------------------
def main():
    global COMPILE, COMPILE_WRAPPER, LINK, WRAPPER

    if not os.environ.has_key("BOOST_BASE"):
        print "BOOST_BASE must first be defined"
        usage()

    if not os.environ.has_key("PYTHON_BASE"):
        print "PYTHON_BASE must first be defined"
        usage()

    try:
        opts, infile = getopt(sys.argv[1:], shortOptions, longOptions)
    except GetoptError, m:
        print
        print m
        usage()

    if len(infile) == 0: usage()
        
    # Decode option, value pairs
    
    cppfile  = infile[0]
    if not os.path.exists(cppfile):
        cppfile += ".cpp"
        if not os.path.exists(cppfile):
            quit("Can't find %s" % cppfile)

    module   = nameonly(cppfile)
    name     = nameonly(cppfile)

    for option, value in opts:
        if option == "help":
            usage()

        elif option == '-o':
            module = nameonly(value)

    names = {}
    names['name']   = name
    names['module'] = module

    # Extract function signature from C++ file

    records = open(cppfile).readlines()
    increc   = ''
    usingrec = ''
    recs = []
    for record in records:
        if include.match(record):
            increc += record
            continue
        elif using.match(record):
            usingrec += record
            continue
        record = comment.sub("", record)
        if record == '': continue
        recs.append(record)

    names['includes'] = strip(increc + usingrec)
    
    record = joinfields(recs, '\n')

    # ---------------------------------------------------
    # Find function bodies and empty them
    # ---------------------------------------------------
    items = {}
    record = parseFunctionBodies(record, items)

    # Find functions

    declaration = ''
    overloads   = ''
    defs        = ''
    
    functions, record = findFunctions(record)
    for i, (str, group, start, left, right, end) in enumerate(functions):
        rec = whitespace.sub(' ', str)
        if skipfun.match(rec): continue

        retype, name, args, vars, qual = decodeMethodName(rec)
        names['name'] = name
        
        numargs = len(args)

        if numargs > MAXARGS: continue
        
        # Find minimum number of arguments

        minargs = numargs
        for arg in args:
            if find(arg,'=') > -1: minargs -= 1

        declaration += "%s\n" % rec

        if minargs < numargs:
            names['min'] = "%d" % minargs
            names['max'] = "%d" % numargs
            overloads += OVERLOAD % names
            defs += DEF_WITH_OVERLOAD % names
        else:
            defs += DEF % names           

    names['declaration'] = rstrip(declaration)
    names['overloads']   = rstrip(overloads)
    names['defs']        = rstrip(defs)
    
    binc = strip(os.
                 popen("find $BOOST_BASE/include/boost* -maxdepth 0").
                 readlines()[0])
    names['boostinc'] = binc
    
    blib = strip(os.
                 popen("find $BOOST_BASE/lib/libboost_python*.so -maxdepth 0").
                 readlines()[0])
    names['boostlib'] = blib
    
    pinc = strip(os.
                 popen("find $PYTHON_BASE/include/python* -maxdepth 0").
                 readlines()[-1])
    names['pythoninc']= pinc
    
    COMPILE = COMPILE % names
    COMPILE_WRAPPER = COMPILE_WRAPPER % names
    LINK    = LINK % names

    wrapper = WRAPPER % names
    
    open('export_%(name)s.cpp' % names, 'w').write(wrapper)

    print "compile %(name)s.cpp" % names
    if os.system(COMPILE) == 0:
        print "\tcompile export_%(name)s.cpp" % names
        if os.system(COMPILE_WRAPPER) == 0:
            print "\t\tlink %(module)s.so" % names
            os.system(LINK)
#------------------------------------------------------------------------------
main()
