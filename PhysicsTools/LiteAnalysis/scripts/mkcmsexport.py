#!/usr/bin/env python
#------------------------------------------------------------------------------
# File: mkcmsexport.py
# Description: Run scramv1 runtime -sh -dump <filename> and scan
#              $CMS_PATH/Releases/CMSSW/CMSSW_VVV/config/site/tools-*.conf
#              to create cmsexport.sh, which when
#              executed sets up standard CMS variables.
# Created: 18-Nov-2004 Harrison B. Prosper
# Updated: 22-Jan-2006 HBP Adapt to CMSSW
# $Revision: 1.4 $
#------------------------------------------------------------------------------
import os, sys, re, posixpath
from string import *
from glob   import glob
#------------------------------------------------------------------------------
def nameonly(s):
    return posixpath.splitext(posixpath.split(s)[1])[0]
#------------------------------------------------------------------------------

# Make sure we have a sane CMS environment
    
if os.environ.has_key('CMS_PATH'):
    CMS_PATH = os.environ['CMS_PATH']
else:
    print "**error** CMS_PATH not defined"
    sys.exit(1)

if os.environ.has_key('SCRAM_ARCH'):
    SCRAM_ARCH = os.environ['SCRAM_ARCH']
else:
    print "**error** SCRAM_ARCH not defined"
    sys.exit(1)

PATH = os.environ['PATH']

# Check that CMSSW exists

path = glob('%s/%s/cms/cmssw/CMSSW_*' % (CMS_PATH, SCRAM_ARCH))
if len(path) == 0:
    print "No CMSSW release found"
    sys.exit(1)
path = path.pop()
version = split(path, '/').pop()
#------------------------------------------------------------------------------
# execute scramv1 runtime -sh
#------------------------------------------------------------------------------

name = {}
name['path'] = path
name['arch'] = SCRAM_ARCH
name['cmspath'] = CMS_PATH
name['HOME'] = os.environ['HOME']

command = '''
scramv1 runtime -sh | \
sed -e "s/SCRAMRT_PATH/PATH/g" | \
sed -e "s/%(arch)s/\$SCRAM_ARCH/g" \
> $HOME/cmsruntime.sh
''' % name
os.system(command)

filename = '%(path)s/config/site/tools-STANDALONE.conf' % name 
filename = glob(filename)[0]
records  = map(lambda x: strip(x),
               os.popen('egrep "BASE:\$SCRAM_BASEPATH/lcg/external.*%s" %s'\
                        % (SCRAM_ARCH, filename).readlines())

#out = open("%(HOME)s/cmstools.sh" % name,"w")
#for record in records:
#    a, b = split(record, ":")
#    var  = a[1:]
#    path = replace(b, "SCRAM_BASEPATH/lcg/external", "CMS_PATH/external")
#    path = replace(path, SCRAM_ARCH, "$SCRAM_ARCH")
#    record = "export %s=%s\n" % (var, path)
#    out.write(record)
#out.close()





    
