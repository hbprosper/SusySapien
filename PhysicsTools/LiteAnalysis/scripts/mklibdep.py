#!/usr/bin/env python
#------------------------------------------------------------------------------
# File:        mklibdep.py
# Description: Create dependency files for all libraries in Symbols
# Created:     23-Apr-2005, Harrison B. Prosper
#$Revison: $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time   import *
from pprint import PrettyPrinter
from header2xml import nameonly
#------------------------------------------------------------------------------
if not os.environ.has_key('PYTHON_PROJECTS_BASE'):
    print 'PYTHON_PROJECTS_BASE not defined!\n'

if os.environ.has_key('CMS_SYS'):
    CMS_SYS = os.environ['CMS_SYS']
else:
    CMS_SYS = ''

if os.environ.has_key('CMS_PATH'):
    CMS_PATH = os.environ['CMS_PATH']
else:
    CMS_PATH = ''
    
base = os.environ['PYTHON_PROJECTS_BASE']
symmapfile   = '%s/config/Symbols' % base
libdepbase   = '%s/config/libdep' % base
symmap       = '%s/python/SymMap.py' % base

names={'base':   base,
       'version':'v1.0',
       'date':   ctime(time()),
       'symmap': symmap}
#-----------------------------------------------------------------------------
libraryToNames = {}
nameToLibrary  = {}
#-----------------------------------------------------------------------------
def buildLibraryList(namelist):
    librarylist = []
    for name in namelist:
        if nameToLibrary.has_key(name):
            library = nameToLibrary[name]
            if library in librarylist: continue
            librarylist.append(library)
    return librarylist
#---------------------------------------------------------------------------
def main():

    lastlib = ''
    # Take a walk!
    print "loading symbol list"
    records = open(symmapfile).readlines()

    print "building dependency map"
    count = 0
    libcount = 0
    for index, record in enumerate(records):
        
        record = strip(record)
        if record == '': continue
        t = split(record)
        path    = t[0]
        library = "%s/%s" % (path,t[1])
        vtype   = t[2]
        name = joinfields(t[3:])
            
        if not libraryToNames.has_key(library): libraryToNames[library]=[]

        if library <> lastlib:
            libcount += 1
            print "%8d\t" % (libcount, library)
        lastlib = library
        
        if vtype == 'I':
            
            nameToLibrary[name] = library

        elif not name in libraryToNames[library]:
            libraryToNames[library].append(name)

    print "building dependency files"
    
    libraries = libraryToNames.keys()
    libraries.sort()
    
    for index, library in enumerate(libraries):
        name = split(library,'/').pop()
        #print "%6d\t%s" % (index+1,name)
        libdepfile = "%s/%s.d" % (libdepbase, name)
        librarylist = buildLibraryList(libraryToNames[library])
        librarylist.sort()
        open(libdepfile,'w').write(joinfields(librarylist,'\n')+'\n')

    # Write out symbol map

    print "Saving SymMap..."

    out = open(symmap,'w')
    out.write(79*"#"+'\n')
    out.write("# File:    SymMap\n" % names)
    out.write("# Created: %(date)s, mklibdep.py"
              " %(version)s\n" % names)
    out.write("#$Revision: 1.2 $\n")
    out.write(79*"#"+'\n')
    out.write("SymMap =\\\n")
    PP = PrettyPrinter(stream=out)
    PP.pprint(nameToLibrary)
    out.write("\n")
    os.system('wc -l %s' % symmap)
#-----------------------------------------------------------------------------
main()
