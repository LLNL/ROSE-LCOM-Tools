-include env.mk

# Note: This Makefile is no longer maintained. Please use the cmake build system to build this tool instead. This Makefile is kept here for reference.

# Makefile for LCOM metrics tool using the ROSE compiler framework.
#
# ROSE has a number of configuration details that must be present when
# compiling and linking a user program with ROSE, and some of these
# details are difficult to get right.  The most foolproof way to get
# these details into your own makefile is to use the "rose-config"
# tool.
#
#
# This makefile assumes:
#   1. The ROSE library has been properly installed (refer to the
#      documentation for configuring, building, and installing ROSE).
#
#   2. The top of the installation directory is $(ROSE_HOME). This
#      is the same directory you specified for the "--prefix" argument
#      of the "configure" script, or the "CMAKE_INSTALL_PREFIX" if using
#      cmake. E.g., "/usr/local".
#
# The "rose-config" tool currently only works for ROSE configured with
# GNU auto tools (e.g., you ran "configure" when you built and
# installed ROSE). The "cmake" configuration is not currently
# supported by "rose-config" [September 2015].
##############################################################################

# Get the path of the current Makefile
MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
# Get the directory of the Makefile
MAKEFILE_DIR := $(dir $(MAKEFILE_PATH))

#If the ROSE bin directory is in your path, rose-config can be found automatically
ifndef ROSE_HOME
ROSE_HOME = $(shell rose-config prefix)
endif

include $(ROSE_HOME)/lib/rose-config.cfg

# Standard C++ compiler stuff (see rose-config --help)
ROSE_CXX         = $(shell $(ROSE_HOME)/bin/rose-config ROSE_CXX)
ROSE_CPPFLAGS    = $(shell $(ROSE_HOME)/bin/rose-config ROSE_CPPFLAGS) -lstdc++fs
ROSE_CXXFLAGS    = $(shell $(ROSE_HOME)/bin/rose-config ROSE_CXXFLAGS) -DROOT_DIR=$(MAKEFILE_DIR)
ROSE_LDFLAGS     = $(shell $(ROSE_HOME)/bin/rose-config ROSE_LDFLAGS) -lstdc++fs
ROSE_LIBDIRS     = $(shell $(ROSE_HOME)/bin/rose-config ROSE_LIBDIRS)
ROSE_RPATHS      = $(shell $(ROSE_HOME)/bin/rose-config ROSE_RPATHS)
ROSE_LINK_RPATHS = $(shell $(ROSE_HOME)/bin/rose-config ROSE_LINK_RPATHS)

MOSTLYCLEANFILES =

SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
BIN_DIR := bin

SRC_NAME := lcom

EXE := $(BIN_DIR)/$(SRC_NAME)
SRC := $(wildcard $(SRC_DIR)/*.cpp)
INC := $(wildcard $(INC_DIR)/*.hpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

##############################################################################
# Assuming your source code is "SRC_NAME.cpp" to build an executable named "SRC_NAME".

all: $(SRC_NAME).out $(SRC_NAME)-dot.out

# Generate the object file associated with the main source file.
$(SRC_NAME).o: $(SRC_DIR)/$(SRC_NAME).cpp
	$(ROSE_CXX) $(ROSE_CPPFLAGS) $(ROSE_CXXFLAGS) -I$(INC_DIR) -o $@ -c $<
# Generate the executable file using the object file.
$(SRC_NAME).out: $(SRC_NAME).o
	$(ROSE_CXX) $(ROSE_CXXFLAGS) -I$(INC_DIR) -o $@ $^ $(ROSE_LDFLAGS) $(ROSE_LINK_RPATHS) -Wl,-rpath=$(ROSE_HOME)/lib

$(SRC_NAME)-dot.o: $(SRC_DIR)/$(SRC_NAME)-dot.cpp
	$(ROSE_CXX) $(ROSE_CPPFLAGS) $(ROSE_CXXFLAGS) -I$(INC_DIR) -o $@ -c $<
$(SRC_NAME)-dot.out: $(SRC_NAME)-dot.o
	$(ROSE_CXX) $(ROSE_CXXFLAGS) -I$(INC_DIR) -o $@ $^ $(ROSE_LDFLAGS) $(ROSE_LINK_RPATHS) -Wl,-rpath=$(ROSE_HOME)/lib

MOSTLYCLEANFILES += $(SRC_NAME).out $(SRC_NAME)-dot.out $(SRC_NAME).o $(SRC_NAME)-dot.o gmon.out

##############################################################################
# Standard boilerplate

.PHONY: clean
clean:
	rm -f $(MOSTLYCLEANFILES)
