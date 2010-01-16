# Make sure we have a defined environment

ifndef BOOST_ROOT
ifdef BOOST_BASE
BOOST_ROOT	:= $(BOOST_BASE)
else
$(error BOOST_BASE is undefined)
endif
endif

ifndef PYTHON_ROOT
ifdef PYTHON_BASE
PYTHON_ROOT	:= $(PYTHON_BASE)
else
$(error PYTHON_BASE is undefined)
endif
endif
