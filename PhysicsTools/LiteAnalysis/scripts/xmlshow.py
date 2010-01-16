#!/usr/bin/env python
#-----------------------------------------------------------------------------
# File:        xmlshow.py
# Description: Show an XML file with HTML entities, such as &amp;, displayed
#              correctly.
# Created:     02-Dec-2003 Harrison B. Prosper, CERN
#-----------------------------------------------------------------------------
from boostlib import convertFromHtml
#-----------------------------------------------------------------------------
# Constants
#-----------------------------------------------------------------------------
VERSION='1.0'
USAGE='''
Usage:
   more | xmlshow 
'''
#---------------------------------------------------------------------------
# Main function to be called when this module is run as a script
#---------------------------------------------------------------------------
def main():
    TheSunShinesBrightly = 1
    while TheSunShinesBrightly:
        try:
            str = convertFromHtml(raw_input())
            print str
        except EOFError:
            break
#---------------------------------------------------------------------------
if __name__ == "__main__":
    main()

