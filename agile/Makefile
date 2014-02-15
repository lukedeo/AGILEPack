# Makefile for AGILEPack
# Author: Luke de Oliveira (luke.deoliveira@yale.edu)


# --- set directories
BIN          := bin
SRC          := src
INC          := $(CURDIR)/../
LIB          := $(CURDIR)/lib

YAML_DIR     := $(CURDIR)/../yaml-cpp

# ...if not, build us some YAML

YAML_INC     := $(YAML_DIR)
YAML_LIB     := $(YAML_DIR)/lib

LIBS         += -L$(YAML_LIB) 
LIBS         += -lyamlc++

YAML         = yaml_proxy


#  set search path
vpath %.o    $(BIN)
vpath %.cxx  $(SRC) 
vpath %.hh   $(INC) 

# --- set compiler and flags (roll c options and include paths together)


# --  shit for eigen to be blazingly fast
DEBUG =  -O0 -g 
PRODUCTION = -O2 -DEIGEN_NO_DEBUG

ADDTL_FLG = $(PRODUCTION)
# ADDTL_FLG = $(DEBUG)

CXX          ?= g++
CXXFLAGS     := -Wall -fPIC -I$(INC) -g -std=c++11 $(ADDTL_FLG) -I$(YAML_DIR) -I./

ifeq ($(CXX),clang++)
CXXFLAGS += -stdlib=libc++
endif

# ---- define objects

LAYER_OBJ    := layer.o autoencoder.o architecture.o

UTIL_OBJ     := activation.o basedefs.o

# - command line interface
EXE_OBJ      := main.o

# EXECUTABLE   := AGILE

LIB_OBJ      := $(LAYER_OBJ) $(UTIL_OBJ)
ALLOBJ       := $(LAYER_OBJ) $(EXE_OBJ) $(UTIL_OBJ)
ALLOUTPUT    := $(EXECUTABLE) $(LIB)/libagile.a


all: $(YAML) $(ALLOUTPUT)
	@echo "AGILE base (libagile.a) build successful."

$(EXECUTABLE): $(LIB_OBJ:%=$(BIN)/%) $(EXE_OBJ:%=$(BIN)/%)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS)

$(LIB)/libagile.a: $(LIB_OBJ:%=$(BIN)/%)
	@mkdir -p $(LIB)
	@echo "linking $^ --> $@"
	@ar rc $@ $^ && ranlib $@

$(YAML):
	@$(MAKE) -C $(YAML_DIR)


# --------------------------------------------------

# compile rule
$(BIN)/%.o: %.cxx
	@echo compiling $<
	@mkdir -p $(BIN)
	@$(CXX) -c $(CXXFLAGS) $< -o $@

# use auto dependency generation
DEP = $(BIN)

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),rmdep)
ifneq ($(MAKECMDGOALS),purge)
include  $(ALLOBJ:%.o=$(DEP)/%.d)
endif
endif
endif

DEPTARGSTR = -MT $(BIN)/$*.o -MT $(DEP)/$*.d
$(DEP)/%.d: %.cxx
	@echo making dependencies for $<
	@mkdir -p $(DEP)
	@$(CXX) -MM -MP $(DEPTARGSTR) $(CXXFLAGS) $< -o $@ 

# clean
.PHONY : clean rmdep
CLEANLIST     = *~ *.o *.o~ *.d core 

clean:
	rm -fr $(CLEANLIST) $(CLEANLIST:%=$(BIN)/%) $(CLEANLIST:%=$(DEP)/%)
	rm -fr $(BIN) 
	@$(MAKE) -C $(YAML_DIR) clean

purge:
	rm -fr $(CLEANLIST) $(CLEANLIST:%=$(BIN)/%) $(CLEANLIST:%=$(DEP)/%)
	rm -fr $(BIN) 
	rm -fr $(EXECUTABLE) $(LIB)
	@$(MAKE) -C $(YAML_DIR) purge

rmdep: 
	rm -f $(DEP)/*.d
