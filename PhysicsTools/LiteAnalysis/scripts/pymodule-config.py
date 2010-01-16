#!/usr/bin/env python
#----------------------------------------------------------------------------
# File: pymodule-config.py
# Description: Return compiler and linker configuration for the specified
#              Python module.
#
# Created: 30-Jan-2006 Harrison B. Prosper
#$Revision: 1.1.1.1 $
#----------------------------------------------------------------------------
import os, sys
from glob   import glob
from getopt import *
from string import *
#------------------------------------------------------------------------------
# Constants
#------------------------------------------------------------------------------
USAGE = '''
Usage: pymodule-config [options] <Python module>

  --help                  Print this
  --cflags                Print compiler flags and header path
  --libs                  Print libraries
''' 
LONGOPTIONS=['help','cflags','libs']
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
#----------------------------------------------------------------------------
# Get location of PYTHON_PROJECTS
#----------------------------------------------------------------------------
def getbasedir():
    if not os.environ.has_key("PYTHON_PROJECTS"):
        quit("PYTHON_PROJECTS undefined - please define it!")
    return os.environ["PYTHON_PROJECTS"]
#----------------------------------------------------------------------------
# Get command line arguments
#----------------------------------------------------------------------------
def getoptions():
    try:
        opts, modules = getopt(sys.argv[1:], None, LONGOPTIONS)

    except GetoptError, message:
        quit(message)
        
    return (opts, modules)
#----------------------------------------------------------------------------
def cflags(modules, names):
    records = []
    for modstr in modules:
        
        t = split(modstr,':')
        project = t[0]
        if len(t) > 1:
            paths = split(t[1],',')
        else:
            paths = [project]

        names['project'] = project
        
        for path in paths:

            names['path'] = path
            
            p = "%(base)s/%(project)s/src/%(path)s/include" % names
            if os.path.exists( p ): records.append("-I%s" % p)
            
            p = "%(base)s/%(project)s/src/%(path)s/interface" % names
            if os.path.exists( p ): records.append("-I%s" % p)

            p = "%(base)s/%(project)s/src/%(path)s/python" % names
            if os.path.exists( p ): records.append("-I%s" % p)            
            
    if len(records) > 0: print joinfields(records,' ')
#----------------------------------------------------------------------------
def libs(modules, names):
    records = []
    for modstr in modules:

        t = split(modstr,':')
        project = t[0]
        if len(t) > 1:
            libs = split(t[1],',')
        else:
            libs = [project]

        names['project'] = project
        libarea = "%(base)s/%(project)s/lib" % names
        if not os.path.exists(libarea): continue
        
        names['libarea'] = libarea

        records.append("-L%(libarea)s" % names)
        
        # Get list of shared libraries

        for lib in libs:
            
            names['lib'] = lib

            path = "%(libarea)s/*%(lib)s.so" % names
            g = glob(path)
            
            for x in g:
                lib = os.path.split(x)[-1] # tuple, so can't use pop()
                lib = os.path.splitext(lib)[0]
                if lib[:3] == 'lib':
                    lib = lib[3:]
                    records.append( "-l%s" % lib )
                else:
                    records.append(libarea + "/%s.so" % lib)
                
    if len(records) > 0: print joinfields(records,' ')
#----------------------------------------------------------------------------
def main():

    names = {}
    names['base'] = getbasedir()
    
    options, modules = getoptions()
    
    # We need at least one module
    
    if len(modules) == 0: usage()
    
    # Decode option, value pairs and
    # act accordingly

    for option, value in options:

        if   option == "--help":
            usage()
            
        elif option == "--cflags":
            cflags(modules, names)

        elif option == "--libs":
            libs(modules, names)
#----------------------------------------------------------------------------
main()
