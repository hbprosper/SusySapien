#-----------------------------------------------------------------------
# File: library.mk
# Purpose: Makefile fragment to build shared libraries
# created: 08-Jul-2005	Harrison B. Prosper
# Updated: 08-Apr-2006  HBP, allow for extensions .for and .FOR
#
#                       External macros:
#
#				library		library name
#						[lib + name of parent dir]
#
#				libdir		library area
#						[$(topdir)/lib OR .]
#
#                               cppflags	C pre-compiler flags
#                               cxxflags	C++ compiler flags
#                               cflags		C compiler flags
#                               fflags		Fortran compiler flags
#                               ldflags		Loader flags
#                               libs		Libraries
#
#                               verbose         [no]
#
#                               withmodule      <project>:module[,module[,]]
#                               withcern	[no]
#                               withboost	[no]
#                               withpython	[no]
#                               withroot	[no]
#                               withcint	[no]
#
#                               linkdef         [_linkdef]
#                               hppdir		[../include]
#                               exclude		sources to exclude
#                                               [export_*.cpp]
#
#			Requires:
#				g++
#				gcc
#				g77
#                               boost
#-----------------------------------------------------------------------
# 	Make sure we have a defined environment
ifndef PYTHON_PROJECTS
$(error PYTHON_PROJECTS is undefined)
endif
area	:= $(subst $(PYTHON_PROJECTS)/,,$(shell pwd))
say	:= $(shell echo -e "\n\t\tLibrary - $(area)" >& 2)
#-----------------------------------------------------------------------
include $(PYTHON_PROJECTS)/config/checkenv.mk
#-----------------------------------------------------------------------

#			LIBRARY

ifndef library
	blddir	:= $(shell pwd)
	path	:= $(subst /, ,$(blddir))
	pwd	:= $(word $(words $(path)),$(path))

	ifeq "$(pwd)" "src"
		blddir	:= $(shell cd ..; pwd)
		path	:= $(subst /, ,$(blddir))
		library	:= lib$(word $(words $(path)),$(path))
	else
		library	:= lib$(pwd)
	endif
endif
#-----------------------------------------------------------------------
include $(PYTHON_PROJECTS)/boostutil/config/common.mk
#-----------------------------------------------------------------------
say 	:= $(shell echo "Library:      $(library)" >& 2)
say 	:= $(shell echo "Library Area: $(libdir)"  >& 2)

ifdef libdir
	ifneq "$(libdir)" "."
		say 	:= $(shell mkdir -p $(libdir))
	endif
endif

#-----------------------------------------------------------------------
sharedlib:= $(libdir)/$(library).so
LDSHARE	= -shared -Wl,-E -Wl,-soname,$(library).so $(debugflag)
#-----------------------------------------------------------------------
# SOURCES and OBJECTS
#-----------------------------------------------------------------------

#		EXCLUDE SOURCES

ifdef exclude
	excsrcs	:= $(wildcard $(srcdir)/$(exclude))
	cppsrcs	:= $(filter-out $(excsrcs), $(wildcard $(srcdir)/*.cpp))
else
	excsrcs	:= $(wildcard $(srcdir)/export_*.cpp)
	cppsrcs	:= $(filter-out $(excsrcs), $(wildcard $(srcdir)/*.cpp))
endif

#		OTHER SOURCES

cxxsrcs	:= $(wildcard $(srcdir)/*.cxx)
ccsrcs	:= $(wildcard $(srcdir)/*.cc)
csrcs	:= $(wildcard $(srcdir)/*.c)
fsrcs	:= $(wildcard $(srcdir)/*.f)
Fsrcs	:= $(wildcard $(srcdir)/*.F)
forsrcs	:= $(wildcard $(srcdir)/*.for)
FORsrcs	:= $(wildcard $(srcdir)/*.FOR)

#		OBJECTS

cppobjs	:= $(subst $(srcdir)/,$(tmpdir)/,$(cppsrcs:.cpp=.o))
cxxobjs	:= $(subst $(srcdir)/,$(tmpdir)/,$(cxxsrcs:.cxx=.o))
ccobjs	:= $(subst $(srcdir)/,$(tmpdir)/,$(ccsrcs:.cc=.o))
cobjs	:= $(subst $(srcdir)/,$(tmpdir)/,$(csrcs:.c=.o))
fobjs	:= $(subst $(srcdir)/,$(tmpdir)/,$(fsrcs:.f=.o))
Fobjs	:= $(subst $(srcdir)/,$(tmpdir)/,$(Fsrcs:.F=.o))
forobjs	:= $(subst $(srcdir)/,$(tmpdir)/,$(forsrcs:.for=.o))
FORobjs	:= $(subst $(srcdir)/,$(tmpdir)/,$(FORsrcs:.FOR=.o))

cintobjs:= $(subst $(srcdir)/,$(tmpdir)/,$(cintsrcs:.cpp=.o))

# Create list of other objects

objects	:= $(cppobjs) $(cxxobjs) $(cintobjs) $(ccobjs) $(cobjs) \
		$(fobjs) $(Fobjs) $(forobjs) $(FORobjs)

programs:= $(objects:.o=) 

#say	:= $(shell echo -e "$(programs)" >& 2)

#-----------------------------------------------------------------------
# TARGETS AND RULES
#-----------------------------------------------------------------------
lib: 	$(cintsrcs) $(sharedlib) 

check: 	$(programs)

$(programs)	: $(tmpdir)/%	: $(tmpdir)/%.o
	@echo "---> Checking  $*.o for undefined references"
	$(AT)echo 'int main(){return 0;}' > $@_main.cpp
	$(AT)$(C++) -c $@_main.cpp -o $@_main.o
	$(AT)$(C++) $(EXEFLAGS) $@_main.o $< $(EXELIBS) -o $@ >& $*.FAILED
	$(AT)rm -rf $*.FAILED
	@echo "               $*.o is fully bound"

$(sharedlib)	: $(objects)
	@echo "---> Linking $(libname)"
	$(AT)$(LDSHARED) $(LDFLAGS) $(LDSHARE) $(objects) $(LIBS) -o $@

$(cppobjs)	: $(tmpdir)/%.o	: $(srcdir)/%.cpp $(headers) 
	@echo "---> Compiling `basename $<`"
	$(AT)$(C++) $(CXXFLAGS) $(CPPFLAGS) -o $@ $< >& $*.FAILED
	$(AT)rm -rf $*.FAILED

$(cxxobjs)	: $(tmpdir)/%.o : $(srcdir)/%.cxx
	@echo "---> Compiling `basename $<`"
	$(AT)$(C++) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< >& $*.FAILED
	$(AT)rm -rf $*.FAILED

$(cintobjs)	: $(tmpdir)/%.o : $(tmpdir)/%.cpp
	@echo "---> Compiling `basename $<`"
	$(AT)$(C++) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< >& $*.FAILED
	$(AT)rm -rf $*.FAILED

$(cintsrcs): $(tmpdir)/%_dict.cpp : $(hppdir)/%.hpp $(srcdir)/%$(linkdef).h
	@echo "---> Generating dictionary `basename $@`"
	$(AT)$(CINT) -f $@ -c $(CPPFLAGS) $+ >& $*.FAILED
	$(AT)rm -rf $*.FAILED

$(ccobjs)	: $(tmpdir)/%.o : $(srcdir)/%.cc
	@echo "---> Compiling `basename $<`"
	$(AT)$(C++) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< >& $*.FAILED
	$(AT)rm -rf $*.FAILED

$(cobjs)	: $(tmpdir)/%.o : $(srcdir)/%.c 
	@echo "---> Compiling `basename $<`"
	$(AT)$(CC)  $(CPPFLAGS) $(CFLAGS) -o $@ $< >& $*.FAILED
	$(AT)rm -rf $*.FAILED

$(fobjs)	: $(tmpdir)/%.o : $(srcdir)/%.f  
	@echo "---> Compiling `basename $<`"
	$(AT)$(F77) $(CPPFLAGS) $(F77FLAGS) -o $@ $< >& $*.FAILED
	$(AT)rm -rf $*.FAILED

$(Fobjs)	: $(tmpdir)/%.o : $(srcdir)/%.F 
	@echo "---> Compiling `basename $<`"
	$(AT)$(F77) $(CPPFLAGS) $(F77FLAGS) -o $@ $< >& $*.FAILED
	$(AT)rm -rf $*.FAILED

$(forobjs)	: $(tmpdir)/%.o : $(srcdir)/%.for 
	@echo "---> Compiling `basename $<`"
	$(AT)$(F77) $(CPPFLAGS) $(F77FLAGS) -o $@ $< >& $*.FAILED
	$(AT)rm -rf $*.FAILED

$(FORobjs)	: $(tmpdir)/%.o : $(srcdir)/%.FOR 
	@echo "---> Compiling `basename $<`"
	$(AT)$(F77) $(CPPFLAGS) $(F77FLAGS) -o $@ $< >& $*.FAILED
	$(AT)rm -rf $*.FAILED

clean	:
		$(AT)rm -rf $(sharedlib)
		$(AT)rm -rf $(tmpdir)/*.o*
		$(AT)rm -rf $(tmpdir)/*_dict.*
		@echo
