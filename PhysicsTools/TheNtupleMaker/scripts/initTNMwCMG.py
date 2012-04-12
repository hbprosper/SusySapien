#!/usr/bin/env python
#------------------------------------------------------------------------------
# initialize TheNtupleMaker with CMGTools, if it is installed in the
# local release. This scripts must be excecuted before scram build
# Created 2 Apr 2012 Harrison B. Prosper
#------------------------------------------------------------------------------
import os, sys
from string import *
pwd = split(os.environ["PWD"],"/")[-1]
if pwd != "TheNtupleMaker":
        print "\n\t* Please run this script from the TheNtupleMaker directory:"
        
        print "\t    python scripts/initTNMwCMG.py"
        print
        sys.exit(0)

# Make scripts executable

os.system("chmod +x scripts/*.py")

if not os.environ.has_key("CMSSW_BASE"):
        print "\t* Please remember \n\t\tcmsenv\n"
        sys.exit(0)

        
# Add link to TheNtupleMaker

base = os.environ["CMSSW_BASE"]
os.system('''
rm -rf %s/python/PhysicsTools/TheNtupleMaker
ln -s %s/src/PhysicsTools/TheNtupleMaker/python %s/python/PhysicsTools/TheNtupleMaker
''' % (base, base, base))

# Check if CMGTools installed

cmgtools = "%s/src/AnalysisDataFormats/CMGTools" % base
if not os.path.exists(cmgtools):
        print "\t* CMGTools was not found in local release"
        print "\t* Please install the CMGTools and re-run this script\n"
        sys.exit(0)
        
print "\n\t* Initialize TheNtupleMaker with CMGTools *\n"

# Now execute initialization commands

cmd = "scripts/mkclassmap.py -u$CMSSW_BASE/src/AnalysisDataFormats/CMGTools"
print cmd
os.system(cmd)

cmd = "scripts/mkclasslist.py"
print cmd
os.system(cmd)

cmd = "scripts/mkplugins.py"
print cmd
os.system(cmd)



