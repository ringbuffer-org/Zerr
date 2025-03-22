# Makefile to build Zerr* library for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

CC  = g++
cflags = -std=c++11 -Wall

cflags += -fPIC

include build/conandeps.mk

ldflags = -L$(CONAN_LIB_DIRS_YAML_CPP)  # Add library directories
ldlibs = -l$(CONAN_LIBS_YAML_CPP)       # Link the yaml-cpp library
# $(info    CONAN_LIB_DIRS_YAML_CPP is $(CONAN_LIB_DIRS_YAML_CPP))
# $(info    CONAN_LIBS_YAML_CPP is $(CONAN_LIBS_YAML_CPP))

ldflags += -L../core/lib
ldlibs += -lzerr_core

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

# class specific depencencies
zerr_features~.class.ldflags += -L$(CONAN_LIB_DIRS_FFTW)
zerr_features~.class.ldlibs += -lfftw3

# add library data files
datafiles += LICENSE
datafiles += README.md
# datadirs  += img

datafiles += help/zerr_combinator~-help.pd
datafiles += help/zerr_envelopes~-help.pd
datafiles += help/zerr_features~-help.pd
datafiles += help/zerr_disperser~-help.pd
datafiles += help/circulation_8.yaml

# Add Zerr* core headers
CC  += -I../core/include/utils
CC  += -I../core/include/modules
CC  += -I../core/include/features

CXX  += -I../core/include/utils
CXX  += -I../core/include/modules
CXX  += -I../core/include/features

# Add Puredata wraper headers
CC  += -Iinclude
CXX += -Iinclude


# Add fftw3 dependency headers
CC  += -I$(CONAN_INCLUDE_DIRS_FFTW)
CXX += -I$(CONAN_INCLUDE_DIRS_FFTW)

# Add yaml-cpp dependency headers
CC  += -I$(CONAN_INCLUDE_DIRS_YAML_CPP)
CXX += -I$(CONAN_INCLUDE_DIRS_YAML_CPP)


# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
include pd-lib-builder/Makefile.pdlibbuilder
