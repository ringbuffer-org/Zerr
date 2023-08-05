# Makefile to build object 'zerr_envelope_combinator~' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# add folder shortcut examples:
# INCLUDES = -I../includes
# TESTING = testing

CC  = g++
cflags = -std=c++11
ldflags += -L/usr/local/lib -lyaml-cpp
ldflags += -L/opt/homebrew/Cellar/fftw/3.3.10_1/lib -lfftw3

lib.name = zerr_envelope_combinator~

# input source file (class name == source file basename)
class.sources = zerr_envelope_combinator~.cpp

# zerr_envelope_combinator pd wrapper
zerr_envelope_combinator~.class.sources += zerr_envelope_combinator.cpp

# zerr_envelope_combinator modules
zerr_envelope_combinator~.class.sources += ../../src/modules/envelopecombinator.cpp

# zerr_envelope_combinator utils
zerr_envelope_combinator~.class.sources += ../../src/utils/logger.cpp
zerr_envelope_combinator~.class.sources += ../../src/utils/utils.cpp
zerr_envelope_combinator~.class.sources += ../../src/utils/ringbuffer.cpp            #TODO: remove if not used 
zerr_envelope_combinator~.class.sources += ../../src/utils/linearinterpolator.cpp    #TODO: remove if not used

# all extra files to be included in binary distribution of the library

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
PDLIBBUILDER_DIR=pd-lib-builder/

CC  += -I../../include
CC  += -I../../include/utils
CC  += -I../../include/modules
CC  += -I../../include/features
CC  += -I../../dependencies/include/fftw3   #TODO: remove if not used 

CXX += -I../../include
CXX += -I../../include/utils
CXX += -I../../include/modules
CXX += -I../../include/features
CXX += -I/usr/local/include/yaml-cpp       #TODO: remove if not used 
CXX += -I../../dependencies/include/fftw3  #TODO: remove if not used 

# use pd-lib-builder
include pd-lib-builder/Makefile.pdlibbuilder



