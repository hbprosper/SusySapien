#!/usr/bin/env python
#----------------------------------------------------------------------------
# File: cvstag.py
# Description: For each specified module perform a cvs rtag
# Created: 13-Jun-2006, Harrison B. Prosper
#$Revision: 1.2 $
#----------------------------------------------------------------------------
import sys, os, re
from string import *
#----------------------------------------------------------------------------
if not os.environ.has_key('PYTHON_PROJECTS'):
    print "PYTHON_PROJECTS not defined"
    sys.exit(1)

names = {}
names['project'] = split(os.environ['PWD'],'/').pop()
names['base']    = os.environ['PYTHON_PROJECTS']
names['config']  = 'config/' % names

# Make sure we're in the right directory

if not os.path.exists('%(config)s' % names):
    print "\ncvstag must be run from a project directory!\n"
    sys.exit(1)
    
#----------------------------------------------------------------------------
def newversion(n):
    a = 100*n[0]+10*n[1]+n[2] + 1
    i = a / 100
    j = (a - i * 100) / 10
    k = (a - i * 100 - j * 10)
    return (i,j,k)

curversion = re.compile(r'(?<=\[)[\w_]+(?=:A\])')
number = re.compile('[0-9]+')
command = "cvs history -m%(project)s" % names
recs = os.popen(command).readlines()
if recs[0][0:6] == 'No rec':
    version = '[v1_0_0:A]'
else:
    version = strip(recs[-2])
cver = curversion.findall(version)[0]
n = map(atoi, number.findall(cver))
m = newversion(n)
names['version'] = 'v%d_%d_%d' % m
    
command = '''
cvs commit -m"tag with %(version)s"
''' % names
if find(os.popen(command).read(),'failed') > -1:
    print "cvs commit failed - please fix errors!"
    sys.exit(0)

command = '''
cvs rtag %(version)s %(project)s;
''' % names
os.system(command)
print "%(project)s - new tag: %(version)s" % names
open("%(base)s/%(project)s/VERSION" % names, 'w').write('%(version)s\n'% names)

    
