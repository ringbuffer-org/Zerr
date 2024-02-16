# Makefile to build Zerr* library for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.
CC  = g++
cflags = -std=c++11
ldflags += -static # to ensure static linking(didn't test if really works)

ldflags += -L../dependencies/lib/yaml-cpp 
ldlibs += -lyaml-cpp

# Library directory name: zerr
lib.name = zerr

# All class sources files
# class name == source file basename.
class.sources += src/zerr_combinator~.cpp
class.sources += src/zerr_features~.cpp
class.sources += src/zerr_envelopes~.cpp 
class.sources += src/zerr_disperser~.cpp

# Source files which must be statically linked to each class in the library.
common.sources += ../core/src/utils/logger.cpp
common.sources += ../core/src/utils/utils.cpp

# Class specific source files
zerr_combinator~.class.sources += src/zerr_combinator.cpp 
zerr_combinator~.class.sources += ../core/src/modules/envelopecombinator.cpp 

zerr_disperser~.class.sources += src/zerr_disperser.cpp
zerr_disperser~.class.sources += ../core/src/modules/audiodisperser.cpp

zerr_envelopes~.class.sources += src/zerr_envelopes.cpp
zerr_envelopes~.class.sources += ../core/src/modules/envelopegenerator.cpp
zerr_envelopes~.class.sources += ../core/src/modules/speakermanager.cpp
zerr_envelopes~.class.sources += ../core/src/utils/onsetdetector.cpp


zerr_features~.class.sources += src/zerr_features.cpp
zerr_features~.class.sources += ../core/src/modules/featurebank.cpp
zerr_features~.class.sources += ../core/src/features/centroid.cpp
zerr_features~.class.sources += ../core/src/features/crestfactor.cpp
zerr_features~.class.sources += ../core/src/features/flatness.cpp
zerr_features~.class.sources += ../core/src/features/flux.cpp
zerr_features~.class.sources += ../core/src/features/rolloff.cpp
zerr_features~.class.sources += ../core/src/features/rootmeansquare.cpp
zerr_features~.class.sources += ../core/src/features/zerocrossingrate.cpp
zerr_features~.class.sources += ../core/src/features/zerocrossings.cpp
zerr_features~.class.sources += ../core/src/utils/ringbuffer.cpp
zerr_features~.class.sources += ../core/src/utils/frequencytransformer.cpp
zerr_features~.class.sources += ../core/src/utils/linearinterpolator.cpp

# class specific depencencies
zerr_features~.class.ldflags += -L../dependencies/lib/fftw3
zerr_features~.class.ldlibs += -lfftw3

# add library data files
datafiles += LICENSE
datafiles += README.md
datadirs  += img

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
CC  += -I../dependencies/include/fftw3
CXX += -I../dependencies/include/fftw3

# Add yaml-cpp dependency headers
CC  += -I../dependencies/include/yaml-cpp
CXX += -I../dependencies/include/yaml-cpp

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
include pd-lib-builder/Makefile.pdlibbuilder
