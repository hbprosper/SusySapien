#!/usr/bin/env python
#------------------------------------------------------------------------------
# File: mktrain.py
# Description: Create files <BNN>.sh and <BNN>.var to run FBM code
# Created: 06-Dec-2005 Harrison B. Prosper
# Updated: 20-Oct-2006 HBP & Daekwang Kau
#          02-Apr-2008 HBP some minor fixes for Serban
#------------------------------------------------------------------------------
import os, sys
from string import *
from time import time, ctime
from random import shuffle
from getopt import getopt, GetoptError
from math import *
#------------------------------------------------------------------------------
USAGE = '''
Usage:
   mktrain <options> <BNN-name>

   options:
         -h   print this
         -t   training file  [train.dat]
         -v   variables text file [use all variables in training file]
         -N   number of events per sig/bkg sample [min(5000, all)]
         -H   number of hidden nodes [20]
         -I   number of iterations [300]
'''
SHORTOPTIONS = 'hs:b:v:N:H:I:'
COUNT = 5000
ITERATIONS = 300
HIDDEN= 20

template = '''
# Created: %(time)s
#------------------------------------------------------------------------------
%(varlist)s
#------------------------------------------------------------------------------
echo "File: %(name)s"

net-spec	%(name)s.bin %(I)d %(H)d 1 / - 0.05:0.5 0.05:0.5 - x0.05:0.5 - 100

model-spec	%(name)s.bin binary

data-spec	%(name)s.bin %(I)s 1 2 / \\
%(datafile)s@2:%(count)d%(cols)s %(datafile)s@2:%(count)d,%(target)s \\
%(datafile)s@2:%(count)d%(cols)s %(datafile)s@2:%(count)d,%(target)s

net-gen		%(name)s.bin fix 0.5

mc-spec		%(name)s.bin repeat 20 heatbath hybrid 100:10 0.2

net-mc		%(name)s.bin 1

mc-spec %(name)s.bin \\
repeat 20 sample-sigmas heatbath 0.95 hybrid 100:10 0.2 negate

echo "Start chain"
echo "Use"
echo "   net-display -h %(name)s.bin"
echo "periodically to check the progress of the chain"

time net-mc	%(name)s.bin %(iter)s

echo ""
echo "Use"
echo "   netwrite -n 100 %(name)s.bin"
echo "to create the BNN function %(name)s.cpp"
'''

#------------------------------------------------------------------------------
def error(message):
    print "** %s" % message
    sys.exit(0)

def usage():
    print USAGE
    sys.exit(0)
    
def nameonly(x):
    return os.path.splitext(os.path.basename(x))[0]
#------------------------------------------------------------------------------
def readData(datafile, varfile, count):

    print "\nReading training file: %s" % datafile
    records= map(lambda x: rstrip(x), open(datafile).readlines())
    header = records[0]
    records= records[1:]

    # Make sure we don't ask for more than the number of
    # events per file
    
    count = min(count, len(records))
    records = records[:count]
    #---------------------------------------------------
    # Convert data to float
    #---------------------------------------------------
    data = map(lambda row:
               map(atof, split(row)), records)

    #---------------------------------------------------
    # Get column names from header and create name to
    # index map
    #---------------------------------------------------
    colnames = split(header)
    colmap = {}
    for index in range(len(colnames)):
        name = colnames[index]
        colmap[name] = index + 1

    # Get variable names, means and sigmas
    
    vars = map(split, open(varfile).readlines())
    mean = []
    sigma= []
    for index, (name, m, s) in enumerate(vars):
        if name != colnames[index]:
            error("The order of variables in %s does not match\n\tthat in %s"\
                  % (datafile, varfile))
        mean.append(atof(m))
        sigma.append(atof(s))
    return (colnames, colmap, data, mean, sigma)
#------------------------------------------------------------------------------
# MAIN Program
#------------------------------------------------------------------------------
def main():
    
    #---------------------------------------------------
    # Decode command line using getopt module
    #---------------------------------------------------
    try:
        options, inputs = getopt(sys.argv[1:], SHORTOPTIONS)
    except GetoptError, m:
        print
        print m
        usage()

    # Make sure we have a network name

    if len(inputs) == 0: usage()

    # Name of BNN
    
    bnnname = inputs[0]
    
    # Set defaults, then parse input line

    trainfile = 'train.dat'
    varfile = ''
    count   = COUNT
    hidden  = HIDDEN
    iterations = ITERATIONS
    
    for option, value in options:
        if option == "-h":
            usage()

        elif option == "-t":
            trainfile = value
            
        elif option == "-v":
            varfile = value
            
        elif option == "-N":
            count = atoi(value)

        elif option == "-H":
            hidden = atoi(value)            

        elif option == "-I":
            iterations = atoi(value)

    #---------------------------------------------------
    # Check that input files exist
    #---------------------------------------------------
    trainvarfile = nameonly(trainfile)+".var"
    if not os.path.exists(trainfile): error("Can't find %s" % trainfile)
    if not os.path.exists(trainvarfile): error("Can't find %s" % trainvarfile)

    #---------------------------------------------------
    # Mix signal and background events
    #---------------------------------------------------
    colnames, colmap, data, mean, sigma = readData(trainfile,
                                                   trainvarfile,
                                                   count)
    ndata = len(data)
    
    #---------------------------------------------------
    # Get list of BNN input variables
    #---------------------------------------------------
    if varfile == '':
        # Use all variables in input files
        var = colnames
    else:
        # Use variables given in variables file
        if not os.path.exists(varfile): error("Can't find %s" % varfile)
        var = map(strip, open(varfile).readlines())
    # Skip any blank lines
    var = filter(lambda x: x != "", var)
    nvar = len(var)
    
    #---------------------------------------------------
    # Write <BNN>.sh file
    #---------------------------------------------------
    varstr = '#\t' + joinfields(var,'\n#\t')

    names = {}
    names['name'] = bnnname
    names['datafile'] = trainfile
    names['time'] = ctime(time())
    names['count']= count+1
    names['I'] = nvar
    names['H'] = hidden
    names['iter'] = iterations
    names['varlist']  = varstr

    print "\nInput variables for BNN %s" % bnnname
    colstr = ""
    for name in var:
        if not colmap.has_key(name): error("Can't find variable %s" % name)
        print "\t%s" % name
        col = colmap[name]
        colstr += ",%d" % col
    names['cols'] = colstr
    
    if not colmap.has_key("target"):
        error("Can't find variable target")
    names['target'] = "%d" % colmap["target"]

    record = template % names

    shfile = "%(name)s.sh" % names
    print "Write %s" % shfile
    open(shfile, "w").write(record)

    #---------------------------------------------------
    # Write means and sigmas of input variables
    # to variables file
    #---------------------------------------------------
    vfile = "%(name)s.var" % names
    print "Write %s" % vfile
    out = open(vfile,"w")
        
    for name in var:
        index = colmap[name]-1
        if sigma[index] == 0: error("variable %s has zero variance" % name)
        
        record = "%-16s\t%10.3e\t%10.3e\n" % (name, mean[index], sigma[index])
        out.write(record)
    out.close()
#------------------------------------------------------------------------------
main()
