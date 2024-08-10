export CC=gcc

# Current directory
export CURRENT= $(PWD)

# Workspace directories
export SRC_DIR= $(CURRENT)/src
export INC_DIR = $(CURRENT)/includes
export LIB_DIR = $(CURRENT)/lib
export TESTS_DIR = $(CURRENT)/tests
export BUILD_DIR = $(CURRENT)/build
export PROTO_DIR = $(CURRENT)/proto


#include src/Makefile
#include tests/Makefile
#include proto/Makefile


# Compile only the src program
all: project


# Call "make project" in ./src
project:
	$(MAKE) -C src project


# Clean all
clean:
	@$(MAKE) -C src clean_project


.PHONY: all project clean clean_project
