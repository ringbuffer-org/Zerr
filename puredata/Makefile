# Makefile to build Zerr* library for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

CC  = g++

#############################################################
# ZERR CORE PATHS (centralized — change here if core moves)
#############################################################
ZERR_CORE_DIR  ?= ../core
ZERR_LIB_DIR   := $(ZERR_CORE_DIR)/lib
ZERR_INCLUDES  := -I$(ZERR_CORE_DIR)/include/utils \
                  -I$(ZERR_CORE_DIR)/include/modules \
                  -I$(ZERR_CORE_DIR)/include/features

#############################################################
# CONAN DEPENDENCIES
#############################################################
# Guard: ensure conan dependencies are available
ifeq ($(wildcard build/conandeps.mk),)
    $(error build/conandeps.mk not found — run 'conan install . --output-folder=build --build=missing' first)
endif

include build/conandeps.mk

#############################################################
# COMPILER & LINKER FLAGS
#############################################################
cflags += -std=c++17 -Wall -DYAML_CPP_STATIC_DEFINE
cflags += -I$(CONAN_INCLUDE_DIRS_YAML_CPP) -I$(CONAN_INCLUDE_DIRS_FFTW)

ldflags += -L$(ZERR_LIB_DIR) -L$(CONAN_LIB_DIRS_YAML_CPP)
ldlibs += -lzerr_core -l$(CONAN_LIBS_YAML_CPP)

# Library directory name: zerr
lib.name = zerr

# All class sources files
class.sources = src/zerr_combinator~.cpp \
                src/zerr_features~.cpp   \
                src/zerr_envelopes~.cpp  \
                src/zerr_disperser~.cpp

# Source files which must be statically linked to each class in the library.

# Class specific source files
zerr_combinator~.class.sources += src/zerr_combinator.cpp
zerr_disperser~.class.sources  += src/zerr_disperser.cpp
zerr_envelopes~.class.sources  += src/zerr_envelopes.cpp
zerr_features~.class.sources   += src/zerr_features.cpp

# class specific dependencies
zerr_features~.class.ldflags += -L$(CONAN_LIB_DIRS_FFTW)
zerr_features~.class.ldlibs += -lfftw3
zerr_envelopes~.class.ldlibs += -lyaml-cpp

# add library data files
datafiles += LICENSE
datafiles += README.md

datafiles += help/zerr_combinator~-help.pd
datafiles += help/zerr_envelopes~-help.pd
datafiles += help/zerr_features~-help.pd
datafiles += help/zerr_disperser~-help.pd
datafiles += help/circulation_8.yaml

# Add Zerr* core headers and PureData wrapper headers
CC  += $(ZERR_INCLUDES) -Iinclude -I$(CONAN_INCLUDE_DIRS_FFTW) -I$(CONAN_INCLUDE_DIRS_YAML_CPP)
CXX += $(ZERR_INCLUDES) -Iinclude -I$(CONAN_INCLUDE_DIRS_FFTW) -I$(CONAN_INCLUDE_DIRS_YAML_CPP)

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
include pd-lib-builder/Makefile.pdlibbuilder
