#!/usr/bin/env python
#------------------------------------------------------------------------------
# File: bwrap.py
# Description: Wrap simple functions using header2xml and xml2boost
# Created:     13-May-2006 Harrison B. Prosper
# $Revision: 1.4 $
#------------------------------------------------------------------------------
import os, sys, re
from string     import *
from getopt     import getopt, GetoptError
from glob       import glob
from boostlib   import renameIdentifier
#------------------------------------------------------------------------------
# Constants
#------------------------------------------------------------------------------
shortOptions = 'I:L:D:l:V' 
longOptions  = ['help',
                'defineargtype=']

USAGE = '''
Usage:
 bwrap.py          [-I<include-path> [-I<include-path]]
                   [-D<macro> [-D<macro>]
                   [-L<libdir> [-L<libdir>]] [-l<lib> [-l<lib>]
                   [--defineargtype="class|'', function, arg, argtype"]

                   <cpp-file|"template<type>">
'''
BIN   = '$PYTHON_PROJECTS/boostutil/bin/'
H2XML = '%(bin)sheader2xml.py -s %(cppflags)s %(name)s.cpp'
XML2B = '%(bin)sxml2boost.py  -s --mapvectortolist=".*"'\
        '%(cppflags)s %(name)s.xml'
XML2BT= '%(bin)sxml2boost.py  -s -t %(cppflags)s'

COMPILE = '''
g++ -c -Os export_%(name)s.cpp \
 -DBOOST_PYTHON_DYNAMIC_LIB \
 -I. \
 -I$PYTHON_PROJECTS/boostutil/include \
 -I%(boostinc)s \
 -I%(pythoninc)s %(cppflags)s
'''

LINK = '''
g++ -shared export_%(name)s.o \
%(boostlib)s %(ldflags)s %(libs)s -o %(name)s.so
rm -rf export_%(name)s.o %(name)s.xml instantiate.py
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
    global COMPILE, LINK

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
    isTemplate = find(cppfile, '<') > -1

    if isTemplate:
        a, b = splitfields(cppfile, '<', 1)
        template = strip(a)
        types = strip(b)[:-1]
        str = "Instantiate('%s', '%s')\n" % (template, types)
        open("templates.py", "w").write(str)
        name = renameIdentifier(cppfile)
    else:
        name = nameonly(cppfile)
        if not os.path.exists(cppfile):
            cppfile += ".cpp"
            if not os.path.exists(cppfile):
                quit("Can't find %s" % cppfile)

    cppflags = ''
    ldflags  = ''
    libs     = ''
    verbose  = False

    for option, value in opts:
        
        if option == "--help":
            usage()

        elif option == '-I':
            cppflags += ' -I%s' % value

        elif option == '-D':
            cppflags += ' -D%s' % value            

        elif option == '-L':
            ldflags += ' -L%s' % value

        elif option == '-l':
            libs += ' -l%s' % value            

        elif option == '-V':
            verbose = True
            
    names = {}
    names['bin']     = BIN
    names['name']    = name
    names['cppflags']= cppflags
    names['ldflags'] = ldflags
    names['libs']    = libs

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

    # Execute commands

    if isTemplate:
        command = XML2BT % names
        if verbose: print command
        os.system(command)
        
    else:
        command = H2XML % names
        if verbose: print command    
        os.system(command)
        if not os.path.exists("%(name)s.xml" % names):
            quit("Can't find %(name)s.xml" % names)

        command = XML2B % names
        if verbose: print command
        os.system(command)
        if not os.path.exists("export_%(name)s.cpp" % names):
            quit("Can't find export_%(name)s.cpp" % names)

    # Now compile and link
    
    compile = strip(COMPILE % names)
    link    = strip(LINK % names)
    
    print "compile export_%(name)s.cpp" % names
    if verbose: print "%s\n" % compile
    if os.system(compile) == 0:
        print "\tlink %(name)s.so" % names
        if verbose: print "%s\n" % link
        os.system(link)
#------------------------------------------------------------------------------
main()
