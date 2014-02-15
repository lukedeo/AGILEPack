# Makefile for yaml-cpp
# Author: Luke de Oliveira (luke.deoliveira@yale.edu)

# --- set dirs
BIN          := bin
SRC          := src
INC          := ../
LIB          := $(CURDIR)/lib

#  set search path
vpath %.o    $(BIN)
vpath %.cpp  $(SRC) 
vpath %.h    $(INC) 

# --- set compiler and flags 

DEBUG        := -g

CXX          ?= g++
CXXFLAGS     := -O2 -Wall -fPIC -I$(INC) -I./ $(DEBUG) -std=c++11 

ifeq ($(CXX),clang++)
CXXFLAGS += -stdlib=libc++
endif

# ---- define objects

LIBRARY      = $(LIB)/libyamlc++.a

SOURCES      = $(shell ls src/*.cpp | xargs -n 1 basename)

OBJ          = $(SOURCES:%.cpp=%.o)

all: $(LIB)/libyamlc++.a
	@echo "libyamlc++.a build successful."

$(LIB)/libyamlc++.a: $(OBJ:%=$(BIN)/%)
	@mkdir -p $(LIB)
	@echo "linking objects to --> $@"
	@ar rc $@ $^ && ranlib $@

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