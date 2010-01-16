#!/usr/bin/env python
#------------------------------------------------------------------------------
# File: newproject.py
# Description: Set up bare bones of a new project
# Create:      26-May-2006 Harrison B. Prosper
#------------------------------------------------------------------------------
import os
from time import time, ctime
from string import *
#------------------------------------------------------------------------------
names = {'project':  split(os.environ['PWD'],'/').pop(),
         'recursemakefile': '$(PYTHON_PROJECTS)/boostutil/config/main.mk',
         'file'   : 'GNUmakefile',
         'time'   :   ctime(time())}

outfile = "GNUmakefile"
out = open(outfile,"w")
out.write("#" + 78*'-' + '\n')
out.write("# %(file)s:\n" % names)
out.write("# Project: %(project)s\n" % names)
out.write("# Created: %(time)s with newproject.py\n" % names)
out.write("#" + 78*'-' + '\n')
out.write('include %(recursemakefile)s\n' % names)
out.close()

os.system("mkdir -p config/work")

prjpy = """
# project definition for Python project %(project)s

project  = '%(project)s'                   

version  = '1.0.0'

# Include paths to be searched for headers

incpaths = '''
$PYTHON_PROJECTS/%(project)s/src
$ROOTSYS/include
$CLHEP_BASE/include
'''

# Library paths to be searched for shared libraries

libpaths = '''         
$PYTHON_PROJECTS/%(project)s/lib
$ROOTSYS/lib
$CLHEP_BASE/lib
'''

# Pre-compiler and compiler flags

cppflags = '''
'''

cxxflags = '''
	-Wall
	-Wunused
	-Wparentheses
'''
""" % names

open("config/work/Project.py", "w").write(prjpy)
readme = '''
Notes:
    The file Project.xml describes the structure of the Python project. A
preliminary version of Project.xml can be created as follows.

(1) Place list of headers to be processed in the file "Headers", one header
path per line.

(2) Invoke

    mkproject.py

to create a preliminary version of Project.xml. Copy Project.xml to the config
directory and modify it as needed.
'''
open("config/work/README", "w").write(readme)
