# Makefile for yaml-cpp
# Author: Luke de Oliveira (luke.deoliveira@yale.edu)

# --- set dirs
BIN          := bin
SRC          := src
INC          := include
# PYTHON       := python
LIB          := $(CURDIR)/lib

HACKED_BOOST := boost_subset


#  set search path
vpath %.o    $(BIN)
vpath %.cpp  $(SRC) 
vpath %.h    $(INC) 

# --- set compiler and flags (roll c options and include paths together)

CXX          ?= g++
# CXX          := clang++
CXXFLAGS     := -O2 -Wall -fPIC -I$(INC) -I$(HACKED_BOOST) -g -std=c++11 
# LDFLAGS      := -Wl,--no-undefined 

# ---- define objects
LIBRARY = $(LIB)/libYAMLCPP_INTERNAL.so

SOURCES = $(shell ls src/*.cpp | xargs -n 1 basename)

OBJ = $(SOURCES:%.cpp=%.o)

all: $(LIB)/libYAMLCPP_INTERNAL.so
	@echo "+++++++++++++++++++++++++++++++++++++++++++"
	@echo "+++ Complete YAML C++ build sucessful.  +++"
	@echo "+++++++++++++++++++++++++++++++++++++++++++"

$(LIB)/libYAMLCPP_INTERNAL.so: $(OBJ:%=$(BIN)/%)
	@mkdir -p $(LIB)
	@echo "linking objects to --> $@"
	@$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS) -shared

# --------------------------------------------------

# compile rule
$(BIN)/%.o: %.cpp
	@echo compiling $<
	@mkdir -p $(BIN)
	@$(CXX) -c $(CXXFLAGS) $< -o $@

# use auto dependency generation
DEP = $(BIN)

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),rmdep)
ifneq ($(MAKECMDGOALS),purge)
include  $(OBJ:%.o=$(DEP)/%.d)
endif
endif
endif

DEPTARGSTR = -MT $(BIN)/$*.o -MT $(DEP)/$*.d
$(DEP)/%.d: %.cpp
	@echo making dependencies for $<
	@mkdir -p $(DEP)
	@$(CXX) -MM -MP $(DEPTARGSTR) $(CXXFLAGS) $< -o $@ 

# clean
.PHONY : clean rmdep purge

CLEANLIST     = *~ *.o *.o~ *.d core 
clean:
	rm -fr $(CLEANLIST) $(CLEANLIST:%=$(BIN)/%) $(CLEANLIST:%=$(DEP)/%)
	rm -fr $(BIN)

purge:
	rm -fr $(CLEANLIST) $(CLEANLIST:%=$(BIN)/%) $(CLEANLIST:%=$(DEP)/%)
	rm -fr $(BIN)
	rm -fr $(LIB)

rmdep: 
	rm -f $(DEP)/*.d