# Makefile for yaml-cpp
# Author: Luke de Oliveira (luke.deoliveira@yale.edu)

# --- set dirs
BIN          := bin
SRC          := src
INC          := include
# PYTHON       := python
LIB          := $(CURDIR)/lib

# HACKED_BOOST := boost_subset


#  set search path
vpath %.o    $(BIN)
vpath %.cpp  $(SRC) 
vpath %.h   $(INC) 

# --- set compiler and flags (roll c options and include paths together)

CXX          := g++
CXXFLAGS     := -O2 -Wall -fPIC -I$(INC) -I$(HACKED_BOOST) -g -std=c++11 
# LDFLAGS      := -Wl,--no-undefined 

# ---- define objects
LIBRARY = $(LIB)/libYAMLCPP_INTERNAL.so

SOURCES = $(shell ls src/*.cpp | xargs -n 1 basename)

OBJ = $(SOURCES:%.cpp=%.o)


# OBJECTS = $(@ls $(SRC)/*.cpp | xargs -n 1 basename)

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
include  $(OBJ:%.o=$(DEP)/%.d)
endif
endif

DEPTARGSTR = -MT $(BIN)/$*.o -MT $(DEP)/$*.d
$(DEP)/%.d: %.cpp
	@echo making dependencies for $<
	@mkdir -p $(DEP)
	@$(CXX) -MM -MP $(DEPTARGSTR) $(CXXFLAGS) $< -o $@ 

# clean
.PHONY : clean rmdep
CLEANLIST     = *~ *.o *.o~ *.d core 
clean:
	rm -fr $(CLEANLIST) $(CLEANLIST:%=$(BIN)/%) $(CLEANLIST:%=$(DEP)/%)
	rm -fr $(BIN) $(ALLOUTPUT)

purge: clean
	rm -fr $(EXECUTABLE) $(LIB)

rmdep: 
	rm -f $(DEP)/*.d
# CXX = g++
# AR = ar
# LIBRARY = libyaml-cpp.a
# TEST    = yaml-cpp-test

# OBJECTS = $(SOURCES:.cpp=.o)
# CPPFLAGS += -Iinclude/ -Itest/
# CXXFLAGS += -Wall

# SOURCES = $(wildcard src/*.cpp)
# TEST_SOURCES = $(wildcard test/*.cpp test/old-api/*.cpp)
# TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

# all: $(LIBRARY)

# test: $(TEST)

# .cpp.o:
# 	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

# $(LIBRARY): $(OBJECTS)
# 	$(AR) -cq $(LIBRARY) $(OBJECTS)
# 	ranlib $(LIBRARY)

# $(TEST): $(LIBRARY) $(TEST_OBJECTS)
# 	$(CXX) -o $(TEST) $(TEST_OBJECTS) $(LIBRARY)

# clean: clean-test
# 	rm -f $(LIBRARY) $(OBJECTS)

# clean-test:
# 	rm -f $(TEST_OBJECTS) $(TEST)
