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

lib.name = zerr_audio_disperser~

# input source file (class name == source file basename)
class.sources = zerr_audio_disperser~.cpp

# zerr_audio_disperser pd wrapper
zerr_audio_disperser~.class.sources += zerr_audio_disperser.cpp

# zerr_audio_disperser modules
zerr_audio_disperser~.class.sources += ../../src/modules/audiodisperser.cpp

# zerr_audio_disperser utils
zerr_audio_disperser~.class.sources += ../../src/utils/logger.cpp
zerr_audio_disperser~.class.sources += ../../src/utils/utils.cpp
zerr_audio_disperser~.class.sources += ../../src/utils/ringbuffer.cpp            #TODO: remove if not used 
zerr_audio_disperser~.class.sources += ../../src/utils/linearinterpolator.cpp    #TODO: remove if not used

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



