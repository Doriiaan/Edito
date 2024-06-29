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


# Call compilation of project and "make tests" in ./tests
tests: project 
	$(MAKE) -C tests tests

proto: project 
	$(MAKE) -C proto proto

# Clean all
clean:
	@$(MAKE) -C src clean_project
	@$(MAKE) -C tests clean_tests
	@$(MAKE) -C proto clean_proto


.PHONY: all project tests proto clean clean_project clean_tests clean_proto
