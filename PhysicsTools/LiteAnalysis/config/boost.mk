#-----------------------------------------------------------------------
# FILE
#	boost.mk
#
# DESCRIPTION
# 	Makefile to build python modules using Boost and GNU compilers
#	Python module name is either the package or the subpackage
#  
# CREATED
#       24-Oct-2003	Harrison B. Prosper
#
#                       External macros:
#
#                               h2xmlflags	h2xml flags
#                               xml2bflags	xml2b flags
#                               cppflags	C pre-compiler flags
#                               cxxflags	C++ compiler flags
#                               cflags		C compiler flags
#                               fflags		Fortran compiler flags
#                               ldflags		Loader flags
#                               libs		Libraries
#
#                               verbose         
#                               nocheck         suppress check for undefined 
#                                               references
#
#			Requires:
#				g77
#				g++
#                               boost
#                               xml2boost.py
#                                    
#
#  Updated: 06-May-2004 HBP
#           16-Sep-2004 HBP Alter project structure
#           05-Dec-2004 HBP Structure conforms with CMS default!
#           17-Oct-2005 HBP Alter project structure, yet again!
#           20-Dec-2005 HBP Remove reference to CERNLIB
#           26-Jan-2006 HBP fix TOPDIR bug
#-----------------------------------------------------------------------
#$Revision: 1.13 $
#-----------------------------------------------------------------------
ifndef PYTHON_PROJECTS
$(error PYTHON_PROJECTS is undefined)
endif

area	:= $(subst $(PYTHON_PROJECTS)/,,$(shell pwd))
say	:= $(shell echo -e "\n\t\tWrappers - $(area)" >& 2)

include $(PYTHON_PROJECTS)/config/checkenv.mk
#-----------------------------------------------------------------------
optflag		:= -Os
withboost	:= yes
withpython	:= yes
#-----------------------------------------------------------------------
include $(PYTHON_PROJECTS)config/common.mk
#-----------------------------------------------------------------------
say 	:= $(shell echo "Package:     $(package)"    >& 2)
say 	:= $(shell echo "Sub-package: $(subpackage)" >& 2)
say 	:= $(shell echo "Module:      $(module)"     >& 2)
#-----------------------------------------------------------------------
# Definition of module deferred
PYLDSHARE	= -shared -Wl,-E -Wl,-soname,libPy$*.so $(debugflag)
#-----------------------------------------------------------------------
# SOURCES and OBJECTS
#-----------------------------------------------------------------------
ifdef doc
	document := $(doc)
else
	document := $(module).doc 
endif
#-----------------------------------------------------------------------
# Create list of sources
#-----------------------------------------------------------------------
init	:= $(wildcard $(configdir)/init.py)
ttypes	:= $(wildcard $(srcdir)/ttypes.py)
srcs	:= $(wildcard $(srcdir)/export_*.cpp)
keeps	:= $(wildcard $(srcdir)/export_*.keep)
keepsrcs:= $(keeps:.keep=.cpp)
killsrcs:= $(filter-out $(keepsrcs),$(srcs))

xmlfiles:= $(wildcard $(xmldir)/*.xml)
ifeq "$(xmlfiles)" ""
	sources	:= $(srcs)
else
	names   := $(notdir $(basename $(xmlfiles)))
	sources	:= $(foreach s,$(names),$(srcdir)/export_$s.cpp)
# 	Select words in srcs *not* in sources
	osources:= $(filter-out $(sources),$(srcs))
endif

# Get list of sources to be compiled but not linked

ifdef skipfailed
failed		:= $(wildcard $(srcdir)/*.FAILED)
failedsrcs	:= $(failed:.FAILED=.cpp)
else
failedsrcs	:=
endif
nolink		:= $(wildcard $(srcdir)/*.NOLINK)
nolinksrcs	:= $(nolink:.NOLINK=.cpp) 
skipsrcs	:= $(nolinksrcs) $(failedsrcs)

# Get list of sources to be compiled

srcs	:= $(filter-out $(failedsrcs),$(osources) $(sources))
srcnames:= $(notdir $(srcs:.cpp=))
objects	:= $(foreach s,$(srcnames),$(tmpdir)/$s.o)

# Get sources to be linked

linksrcs	:= $(notdir $(basename $(filter-out $(skipsrcs),$(srcs))))
linknames	:= $(linksrcs:.cpp=)
linknames	:= $(subst export_,,$(linknames))
sharedlibs	:= $(foreach s,$(linknames),$(libpydir)/libPy$s.so)
programs	:= $(foreach s,$(linknames),$(tmpdir)/export_$s)

#say 	:= $(shell echo -e "\nlinknames:  $(linknames)"  >& 2)
#say 	:= $(shell echo -e "srcnames:   $(srcnames)\n"  >& 2)
#$(error bye)
#-----------------------------------------------------------------------
# TARGETS AND RULES
#-----------------------------------------------------------------------
build:	$(sharedlibs) $(objects)
	@echo "---> Building $(module).py"
	$(AT)$(XML2B) -M $(subpackagedir)
	$(AT)cat $(subpackagedir)/$(module).py $(ttypes) > \
	$(subpackagedir)/.tmp
	$(AT)cp $(subpackagedir)/.tmp $(subpackagedir)/$(module).py
	$(AT)echo -e "# `date`" >  $(packagedir)/__init__.py
	$(AT)echo -e "# `date`" >  $(subpackagedir)/.tmp
	$(AT)cat $(subpackagedir)/.tmp $(init) > $(subpackagedir)/__init__.py

check: 	$(programs)

xml2b:	$(sources)

template: $(sources)
	$(AT)$(XML2B) $(XML2BFLAGS) -t
	@echo ""

ifdef nocheck
$(sharedlibs)	: $(libpydir)/libPy%.so	: $(tmpdir)/export_%.o
	@echo "---> Linking   libPy$*.so"	
	$(AT)$(LDSHARED) $(LDFLAGS) $(PYLDSHARE) \
	$(tmpdir)/export_$*.o $(SHRLIB) $(LIBS) -o $@
	@echo ""
else
$(sharedlibs)	: $(libpydir)/libPy%.so	: $(tmpdir)/export_%
	@echo "---> Linking   libPy$*.so"	
	$(AT)$(LDSHARED) $(LDFLAGS) $(PYLDSHARE) \
	$(tmpdir)/export_$*.o $(SHRLIB) $(LIBS) -o $@
	@echo ""

$(programs)	: $(tmpdir)/%	: $(tmpdir)/%.o
	@echo "---> Checking  $*.o for undefined references"
	$(AT)echo 'int main(){return 0;}' > $@_main.cpp
	$(AT)$(C++) -c $@_main.cpp -o $@_main.o
	$(AT)$(C++) $(EXEFLAGS) $@_main.o $< $(EXELIBS) -o $@ >& $*.FAILED
	$(AT)rm -rf $*.FAILED
	@echo "               $*.o is fully bound"
endif

$(objects)	: $(tmpdir)/%.o	: $(srcdir)/%.cpp
	@echo "---> Compiling $<"
	$(AT)$(C++) $(CXXFLAGS) $(CPPFLAGS) -o $@ $< >& $*.FAILED
	$(AT)rm -rf $*.FAILED

ifneq "$(xmlfiles)" ""
$(sources)	: $(srcdir)/export_%.cpp : $(xmldir)/%.xml
	@echo "---> Building  $@"
	$(AT)$(XML2B) $(XML2BFLAGS) $< 
endif

clean	:
		$(AT)rm -rf $(shrlib) $(sharedlibs)
		$(AT)rm -rf $(killsrcs)
		$(AT)rm -rf $(srcdir)/inst*.py*
		$(AT)rm -rf $(srcdir)/ttypes*.py*
		$(AT)rm -rf $(tmpdir)/*
		$(AT)rm -rf $(srcdir)/export*.hpp*
		$(AT)rm -rf $(srcdir)/export*.NOLINK*
		$(AT)rm -rf $(srcdir)/*.FAILED*
		$(AT)rm -rf $(srcdir)/main.*
		$(AT)rm -rf $(srcdir)/undefined.txt
		@echo

