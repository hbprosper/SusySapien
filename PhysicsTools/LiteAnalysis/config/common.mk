#-----------------------------------------------------------------------
# File: common.mk
# Purpose: Makefile fragment to define compilers and flags
# created: 08-Jul-2005	Harrison B. Prosper
#
#                       External macros:
#                               cppflags	C pre-compiler flags
#                               cxxflags	C++ compiler flags
#                               cflags		C compiler flags
#                               fflags		Fortran compiler flags
#                               ldflags		Loader flags
#                               libs		Libraries
#
#                               verbose         [no]
#
#                               withmodule	project:module[,module[,]...]
#
#                               withcern	[no]
#                               withboost	[no]
#                               withboostutil	[no]
#                               withpython	[no]
#                               withroot	[no]
#                               withcint	[no]
#                               linkdef         [_linkdef]
#                               hppdir		[../include]
#
#			Requires:
#				g++
#				gcc
#				g77
#                               boost
#$Revision: 1.11 $
#-----------------------------------------------------------------------

# Make sure we have a defined environment

ifndef PYTHON_PROJECTS
$(error PYTHON_PROJECTS must point to the PythonProjects directory)
endif

ifndef BOOST_BASE
$(error BOOST_BASE must point to the location containing the Boost lib)
endif

ifndef PYTHON_BASE
$(error PYTHON_BASE must point to the location containing the lib/pythonX.Y)
endif
#-----------------------------------------------------------------------
# COMPILERS/LINKER
#-----------------------------------------------------------------------
ifdef GCC_DIR
GCC_BIN_PREFIX	:= $(GCC_DIR)/bin/
else
GCC_BIN_PREFIX	:= /usr/bin/
endif
C++	:= $(GCC_BIN_PREFIX)g++
CC	:= $(GCC_BIN_PREFIX)gcc
F77	:= $(GCC_BIN_PREFIX)g77
LDSHARED:= $(GCC_BIN_PREFIX)g++
H2XML	:= $(PYTHON_PROJECTS)/bin/header2xml.py
XML2B	:= $(PYTHON_PROJECTS)/bin/xml2boost.py
PYCONFIG:= $(PYTHON_PROJECTS)/bin/pymodule-config.py
CINT	:= $(ROOTSYS)/bin/rootcint

C++VER	:= $(shell $(C++) --version)
COMP	:= $(word 1, $(C++VER))
CTYPE	:= $(word 2, $(C++VER))
CVER	:= $(word 3, $(C++VER))
say 	:= $(shell echo "$(COMP) $(CTYPE) $(CVER)" >& 2)
#-----------------------------------------------------------------------
dirlist := $(subst /, ,$(PYTHON_PROJECTS))
prjdir	:= $(word $(words $(dirlist)), $(dirlist))
pwd		:= $(shell pwd)
tmp		:= $(word 2, $(subst $(prjdir)/, ,$(pwd)))
project := $(word 1,$(subst /, ,$(tmp)))
topdir	:= $(PYTHON_PROJECTS)/$(project)
#-----------------------------------------------------------------------
# Define various directories. 
#-----------------------------------------------------------------------
# Get package, sub-package, module

path	:= $(subst /, ,$(topdir))
package	:= $(word $(words $(path)), $(path))

path	:= $(subst /, ,$(shell cd ../..; pwd))
subpkg	:= $(word $(words $(path)),$(path))

path	:= $(subst /, ,$(shell cd ..; pwd))
module	:= $(word $(words $(path)),$(path))

libdir	:= $(topdir)/lib
libpydir:= $(libdir)/python

ifeq "$(subpkg)" "src"
subpackage	:= 
packagedir	:= $(libpydir)/$(package)
subpackagedir	:= $(libpydir)/$(package)

do		:= $(shell mkdir -p $(packagedir))
else 
ifeq "$(subpkg)" "$(prjdir)"
package		:=
packagedir	:= $(libpydir)
subpackage	:= 
subpackagedir	:= $(libpydir)
do		:= $(shell mkdir -p $(packagedir))

else
packagedir	:= $(libpydir)/$(package)
subpackage	:= $(subpkg)
subpackagedir	:= $(libpydir)/$(package)/$(subpackage)

do		:= $(shell mkdir -p $(packagedir))
do		:= $(shell mkdir -p $(subpackagedir))
endif
endif

tmpdir	    	:= $(topdir)/tmp/$(module)
do		:= $(shell mkdir -p $(tmpdir))

configdir  	:= $(topdir)/config
main		:= main
xmldir		:= ../xml
srcdir		:= .
pythondir	:= ../python
incdir		:= ../include
docdir		:= ../doc
#-----------------------------------------------------------------------
ifdef verbose
	AT =
else
	AT = @
endif
#-----------------------------------------------------------------------
# Products to compile/link against
#-----------------------------------------------------------------------
ifdef withcern
	ifndef CERN_LIB
		ifdef CERN_DIR
			CERN_LIB:= $(CERN_DIR)/lib
		else
			say:=$(error CERN_LIB must point to CERN lib directory)
		endif
	endif
	cernlib	:= -L$(CERN_LIB) -lpacklib -lmathlib -lkernlib
endif
#-----------------------------------------------------------------------
ifdef withboost
	boostutilcpp	:= -I$(PYTHON_PROJECTS)/boostutil/include

#	Boost include path

	boostcpp:= -I$(BOOST_BASE)/include

#	Boost lib path

	boostlib:= -L$(BOOST_BASE)/lib -lboost_python		
	version:= $(subst /, ,$(BOOST_BASE))
	version:= $(word $(words $(version)),$(version))
	boostcxx:= -ftemplate-depth-1000 -DBOOST_PYTHON_DYNAMIC_LIB 
	say 	:= $(shell echo Boost $(version)   >& 2)
endif 

ifdef withboostutil
	boostutilcpp	:= -I$(PYTHON_PROJECTS)/boostutil/include
	boostutillib	:= -L$(PYTHON_PROJECTS)/boostutil/lib -lboostutil
endif 
#-----------------------------------------------------------------------
ifdef withpython

	ifndef PYTHON_BASE
		say:= $(error PYTHON_BASE undefined)
	endif

#	Python include path

	pythoncpp:= $(shell find $(PYTHON_BASE)/include/python* -maxdepth 0)
	ifeq "$(pythoncpp)" ""
		say:= $(shell echo -e "\nCheck Python installation\n" >& 2)
		say:= $(error )
	endif
	pythoncpp:= $(word $(words $(pythoncpp)), $(pythoncpp))
	pythoncpp:= -I$(pythoncpp)

#	Python lib path

	version:= $(subst /, ,$(pythoncpp))
	version:= $(word $(words $(version)),$(version))

	pythonlib := -L$(PYTHON_BASE)/lib/$(version)/config -l$(version)

	version	:= $(strip $(shell $(PYTHON_BASE)/bin/python -V))
	say 	:= $(shell echo $(version)   >& 2)
endif
#-----------------------------------------------------------------------
ifdef withcint
	withroot:= yes
	cintcpp	:= -D__WITH_CINT__

	ifndef linkdef
		linkdef	:= _linkdef
	endif

	ifndef hppdir
		hppdir	:= $(incdir)
	endif

	ldefsrcs:= $(wildcard $(srcdir)/*$(linkdef).h)
	hnames	:= \
	$(patsubst $(srcdir)/%,$(hppdir)/%.hpp,$(ldefsrcs:$(linkdef).h=))
	cintsrcs:= $(patsubst $(hppdir)/%.hpp,$(tmpdir)/%_dict.cpp,$(hnames))
endif
#-----------------------------------------------------------------------
ifdef withroot
	rootcpp	:= $(shell root-config --cflags)
	rootlib	:= $(shell root-config --glibs) -lTreePlayer
endif
#-----------------------------------------------------------------------
ifdef withmodule
	modulecpp	:= $(shell $(PYCONFIG) --cflags "$(withmodule)")
	modulelib	:= $(shell $(PYCONFIG) --libs   "$(withmodule)")
endif
#-----------------------------------------------------------------------
# SWITCHES/INCLUDES
#-----------------------------------------------------------------------
ifndef optflag
	optflag:=-O2
endif

CPPFLAGS:= 	-I. -I$(incdir) -I$(docdir) -I$(PYTHON_PROJECTS) \
			$(cppflags) \
			$(modulecpp) \
			$(boostutilcpp) \
			$(boostcpp) \
			$(pythoncpp) \
			$(rootcpp) \
			$(cintcpp)

otherflags	:= -rdynamic -pthread
cppfiltered	:= $(filter-out $(otherflags),$(CPPFLAGS))
H2XMLFLAGS 	:= -S  $(h2xmlflags) $(DEBUG)
XML2BFLAGS	:= $(xml2bflags) $(DEBUG)

# Use -fno-threadsafe-statics to prevent symbols __cxa_guard_acquire etc
# from being generated by the compiler. HBP Mon May 29, 2006

CXXFLAGS:= -c -pipe $(optflag) -fPIC -Wall \
			$(boostcxx) $(cxxflags) $(debugflag)

CFLAGS	:= -c $(optflag) $(cflags) $(debugflag)

F77FLAGS:= -c $(optflag) \
		-fno-automatic -fno-globals -fno-second-underscore \
			$(f77flags) $(debugflag)
#-----------------------------------------------------------------------
# LIBRARIES
#-----------------------------------------------------------------------
ifdef libdir
	ldflags += -L$(libdir)
endif

#LDFLAGS	:= -L/usr/lib -L/lib $(ldflags) $(debugflag)
LDFLAGS	:= $(ldflags) $(debugflag)
EXEFLAGS:= $(LDFLAGS) 
SYSLIB	:=  -lg2c -lpthread -lm -ldl -lutil -lc
rootlib	:= $(filter-out $(SYSLIB),$(rootlib))
LIBS	:= $(libs) $(modulelib) $(boostutillib) $(rootlib) \
		$(cernlib) $(boostlib) $(pythonlib) $(SYSLIB)
EXELIBS	:=  $(boostlib) $(libs) $(modulelib) $(boostutillib) $(rootlib) \
		$(cernlib)  $(pythonlib) $(SYSLIB)
#-----------------------------------------------------------------------
