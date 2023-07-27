# Makefile to build object 'zerr_envelope_generator' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# add folder shortcut examples:
# INCLUDES = -I../includes
# TESTING = testing

CC  = g++
cflags = -std=c++11
ldflags += -L/usr/local/lib -lyaml-cpp
ldflags += -L/opt/homebrew/Cellar/fftw/3.3.10_1/lib -lfftw3

lib.name = zerr_envelope_generator~

# input source file (class name == source file basename)
class.sources = zerr_envelope_generator~.cpp

# zerr_envelope_generator pd wrapper
zerr_envelope_generator~.class.sources += zerr_envelope_generator.cpp

# zerr_envelope_generator modules
zerr_envelope_generator~.class.sources += ../../src/modules/envelopegenerator.cpp
zerr_envelope_generator~.class.sources += ../../src/modules/speakerarray.cpp

# zerr_envelope_generator utils
zerr_envelope_generator~.class.sources += ../../src/utils/utils.cpp
zerr_envelope_generator~.class.sources += ../../src/utils/ringbuffer.cpp
zerr_envelope_generator~.class.sources += ../../src/utils/linearinterpolator.cpp

# all extra files to be included in binary distribution of the library
# datafiles = configs/spkr_configs/circulation_8.yaml
# datadirs = configs

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
PDLIBBUILDER_DIR=pd-lib-builder/

CC  += -I../../include
CC  += -I../../include/utils
CC  += -I../../include/modules
CC  += -I../../include/features
CXX += -I../../include
CXX += -I../../include/utils
CXX += -I../../include/modules
CXX += -I../../include/features
CXX += -I/usr/local/include/yaml-cpp

CC += -I../../dependencies/include/fftw3
CXX += -I../../dependencies/include/fftw3

# use pd-lib-builder
include pd-lib-builder/Makefile.pdlibbuilder



