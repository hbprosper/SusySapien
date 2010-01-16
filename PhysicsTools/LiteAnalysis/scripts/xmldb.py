#!/usr/bin/env python
#------------------------------------------------------------------------------
# File:        xmldb.py
# Description: Manage XmlMap.py
# Created:     23-Nov-2004, Harrison B. Prosper
#$Revison: $
#------------------------------------------------------------------------------
import os, sys, re
from time   import *
from string import *
from getopt     import getopt, GetoptError
from pprint import PrettyPrinter
from header2xml import nameonly, readXmlFile, findClassElements
#------------------------------------------------------------------------------
version = '1'
SHORTOPTIONS = 'hPp:d:a:'
#-----------------------------------------------------------------------------
if not os.environ.has_key('PYTHON_PROJECTS'):
    print 'PYTHON_PROJECTS not defined!\n'
    
BASE = os.environ['PYTHON_PROJECTS']
XMLMAP = '%s/boostutil/config/XmlMap.py'  % BASE
names={'base':   BASE,
       'version':version,
       'date':   ctime(time()),
       'xmlmap': XMLMAP}
#-----------------------------------------------------------------------------
def usage():
    print '''
Usage:
    xmldb.py -p <keyname>   print key 
             -d <keyname>   delete key
             -P             prune database
    '''
def printKey(key, XmlMap):
    if XmlMap.has_key(key):
        PP = PrettyPrinter()
        print "%s:" % key
        PP.pprint(XmlMap[key])
    else:
        print "entry %s not found" % key
#-----------------------------------------------------------------------------
def writeXmlMap(XmlMap):
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
#-----------------------------------------------------------------------------
def main():
    try:
        from XmlMap import XmlMap
    except:
        print "Unable to import XmlMap.py"
        sys.exit(0)
        
    try:
        opts, infiles = getopt(sys.argv[1:],SHORTOPTIONS)
    except GetoptError, m:
        print
        print m
        usage()

    if len(opts) == 0: usage()
    
    # Decode option, value pairs

    for option, value in opts:
        if option == "-h":
            usage()

        elif option == "-p":
            printKey(value, XmlMap)
            
        elif option == "-P":
            update = False
            for (key, value) in XmlMap.items():
                for x in value:
                    names['name']=x
                    path = "%(base)s/%(name)s" % names
                    if not os.path.exists(path):
                        update = True
                        print "delete path:", x
                        value.remove(x)
                if len(value) == 0:
                    print "\t** delete key:", key
                    del XmlMap[key]
                else:
                    XmlMap[key] = value
            if update:
                print "Update XmlMap.py"
                writeXmlMap(XmlMap)            
#-----------------------------------------------------------------------------
main()

