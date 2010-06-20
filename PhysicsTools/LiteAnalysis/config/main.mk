# From SoftRelTools
#------------------------------------------------------------------------------
# 	Make sure we have a defined environment
ifndef PYTHON_PROJECTS
$(error PYTHON_PROJECTS is undefined)
endif
include $(PYTHON_PROJECTS)/config/checkenv.mk
#------------------------------------------------------------------------------
ifdef verbose
	VERBOSE:= "verbose=1"
else
	VERBOSE:= --no-print-directory -s
endif

#	Get top directory

pwd	:= $(shell /bin/pwd)
dirlist := $(subst /, ,$(PYTHON_PROJECTS))
prjdir	:= $(word $(words $(dirlist)), $(dirlist))
tmp	:= $(word 2, $(subst $(prjdir)/, ,$(pwd)))
project := $(word 1,$(subst /, ,$(tmp)))
topdir	:= $(PYTHON_PROJECTS)/$(project)
#------------------------------------------------------------------------------

#	Get list of packages to process
# 	Use SUBDIRS file if it exists

ifneq ($(wildcard SUBDIRS),)
PYDIRS	:= $(shell grep "^src/.*/python" SUBDIRS)
XMLDIRS	:= $(shell grep "^src/.*/xml"    SUBDIRS)
SRCDIRS	:= $(shell grep "^src/.*/src"    SUBDIRS)
else
PYDIRS	:= $(shell find src/* -name "python")
XMLDIRS	:= $(shell find src/* -name "xml")
SRCDIRS	:= $(shell find src/* -name "src")
endif
PACKAGES:= $(subst /python,,$(subst src/,,$(PYDIRS)))

# Tell make that these targets are not real files

.PHONY:	xmlveryclean xmlclean clean lib

all:	lib h2xml xml2b template build


ifeq ($(pwd),$(topdir))
setup:
	@$(PYTHON_PROJECTS)/bin/pyproject.py setup

config:
	@$(PYTHON_PROJECTS)/bin/pyproject.py config

shell:
	@$(PYTHON_PROJECTS)/bin/pyproject.py shell $(shell)
endif

lib:
	@$(foreach p,$(SRCDIRS), make $(VERBOSE) -C $(p) lib ;)

h2xml:
	@$(foreach p,$(XMLDIRS), make $(VERBOSE) -C $(p) h2xml ;)

xmlclean:
	@$(foreach p,$(XMLDIRS), make $(VERBOSE) -C $(p) xmlclean ;)

xmlcleanall:
	@$(foreach p,$(XMLDIRS), make $(VERBOSE) -C $(p) xmlcleanall ;)

build:
	@$(foreach p,$(PYDIRS),  make $(VERBOSE) -C $(p) build ;)

check:
	@$(foreach p,$(PYDIRS),  make $(VERBOSE) -C $(p) check ;)

xml2b:
	@$(foreach p,$(PYDIRS),  make $(VERBOSE) -C $(p) xml2b ;)

template:
	@$(foreach p,$(PYDIRS),  make $(VERBOSE) -C $(p) template ;)

clean:
	@$(foreach p,$(SRCDIRS), make $(VERBOSE) -C $(p) clean ;)
	@$(foreach p,$(XMLDIRS), make $(VERBOSE) -C $(p) clean ;)
	@$(foreach p,$(PYDIRS),  make $(VERBOSE) -C $(p) clean ;)

define subtargets
$(1).all:	$(1).lib $(1).h2xml $(1).xml2b $(1).template $(1).build

$(1).lib:
	@make $(VERBOSE) -C src/$(1)/src lib

$(1).h2xml:
	@make $(VERBOSE) -C src/$(1)/xml h2xml

$(1).xmlclean:
	@make $(VERBOSE) -C src/$(1)/xml xmlclean

$(1).xmlveryclean:
	@make $(VERBOSE) -C src/$(1)/xml xmlcleanall

$(1).build:
	@make $(VERBOSE) -C src/$(1)/python build 

$(1).check:
	@make $(VERBOSE) -C src/$(1)/python check

$(1).xml2b:
	@make $(VERBOSE) -C src/$(1)/python xml2b

$(1).template:
	@make $(VERBOSE) -C src/$(1)/python template
endef

$(foreach target,$(PACKAGES),$(eval $(call subtargets,$(target))))

debug:
	@echo "Packages: $(PACKAGES)"
