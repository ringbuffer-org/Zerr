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
# Dependencies are resolved once at the repo root (see /conanfile.txt) so that
# this wrapper and zerr_core are built against one identical profile. Override
# ZERR_CONAN_DIR if the conan output folder lives elsewhere.
ZERR_CONAN_DIR ?= ../build

# Guard: ensure conan dependencies are available before building.
#
# $(error) fires while the makefile is being parsed, i.e. for *every* goal, so an
# unconditional guard makes `make clean` fail too whenever the dependencies are
# absent -- which is exactly the state you are in after removing them. Cleaning
# must not require them, so the guard is skipped for clean-only invocations.
#
# "Clean-only" means every named goal needs no dependencies -- not merely that one
# of them does. A plain `$(filter clean,...)` also matches `make clean all`, which
# would then build with the CONAN_* variables empty: `-I$(CONAN_INCLUDE_DIRS_YAML_CPP)`
# collapses to a bare `-I` that swallows the flag behind it, and `-l$(CONAN_LIBS_YAML_CPP)`
# to a bare `-l`, trading this message for a cryptic compiler error. An empty goal
# list means the default target, which does need them.
NODEPS_GOALS := clean
CLEAN_ONLY   := $(if $(MAKECMDGOALS),$(if $(filter-out $(NODEPS_GOALS),$(MAKECMDGOALS)),,yes))

ifneq ($(wildcard $(ZERR_CONAN_DIR)/conandeps.mk),)
    include $(ZERR_CONAN_DIR)/conandeps.mk
else ifeq ($(CLEAN_ONLY),)
    $(error $(ZERR_CONAN_DIR)/conandeps.mk not found — from the repo root run: conan install . --output-folder=build --build=missing -pr:h=profiles/<macos|linux|mingw> -pr:b=profiles/<macos|linux|mingw>)
endif

#############################################################
# COMPILER & LINKER FLAGS
#############################################################
cflags += -std=c++17 -Wall -DYAML_CPP_STATIC_DEFINE
cflags += -I$(CONAN_INCLUDE_DIRS_YAML_CPP) -I$(CONAN_INCLUDE_DIRS_FFTW)

ldflags += -L$(ZERR_LIB_DIR) -L$(CONAN_LIB_DIRS_YAML_CPP)
ldlibs += -lzerr_core -l$(CONAN_LIBS_YAML_CPP)

# Library directory name: zerr
lib.name = zerr

# Version: zerr-meta.pd is the single source for the Pd package (the Pd libdir /
# deken convention). pd-lib-builder reads the same line into lib.version, but only
# after it has folded cflags into cxx.flags (Makefile.pdlibbuilder:682 vs :699), so
# it is read here as well. build.sh pd_version() and ci.yml use the same sed line;
# keep the three copies identical.
ZERR_PD_VERSION := $(shell sed -n 's|^\#X text [0-9][0-9]* [0-9][0-9]* VERSION \(.*\);|\1|p' $(lib.name)-meta.pd)
ifeq ($(ZERR_PD_VERSION),)
  $(error $(lib.name)-meta.pd has no "VERSION x.y.z" line)
endif
cflags += -DZERR_PD_VERSION=\"$(ZERR_PD_VERSION)\"

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
# yaml-cpp is already in the library-wide ldlibs above; repeating it here made the
# linker report "ignoring duplicate libraries: '-lyaml-cpp'".

# add library data files (pd-lib-builder does not install the meta file by itself)
datafiles += zerr-meta.pd
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
