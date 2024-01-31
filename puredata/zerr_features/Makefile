# Makefile to build class 'zerr_features' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# library name

# add folder shortcut examples:
# INCLUDES = -I../includes
# TESTING = testing

CC  = g++
cflags = -std=c++11
# ldflags += -L../../dependencies/lib/yaml-cpp -lyaml-cpp    # keep it for future
ldflags += -L../../dependencies/lib/fftw3      
ldlibs += -lfftw3

lib.name = zerr_features~

# input source file (class name == source file basename)
class.sources = zerr_features~.cpp

# zerr_features pd wrapper
zerr_features~.class.sources += zerr_features.cpp

# zerr_features modules
zerr_features~.class.sources += ../../src/modules/featurebank.cpp

# zerr_features features
zerr_features~.class.sources += ../../src/features/centroid.cpp
zerr_features~.class.sources += ../../src/features/crestfactor.cpp
zerr_features~.class.sources += ../../src/features/flatness.cpp
zerr_features~.class.sources += ../../src/features/flux.cpp
zerr_features~.class.sources += ../../src/features/rolloff.cpp
zerr_features~.class.sources += ../../src/features/rootmeansquare.cpp
zerr_features~.class.sources += ../../src/features/zerocrossingrate.cpp
zerr_features~.class.sources += ../../src/features/zerocrossings.cpp

# zerr_features utils
zerr_features~.class.sources += ../../src/utils/utils.cpp
zerr_features~.class.sources += ../../src/utils/ringbuffer.cpp
zerr_features~.class.sources += ../../src/utils/frequencytransformer.cpp
zerr_features~.class.sources += ../../src/utils/linearinterpolator.cpp

# include all headers
CC  += -I../../include/utils
CC  += -I../../include/modules
CC  += -I../../include/features

CXX += -I../../include/utils
CXX += -I../../include/modules
CXX += -I../../include/features

# CC  += -I../../dependencies/include/yaml-cpp    # keep it for future
# CXX += -I../../dependencies/include/yaml-cpp    # keep it for future

CC  += -I../../dependencies/include/fftw3
CXX += -I../../dependencies/include/fftw3

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
include pd-lib-builder/Makefile.pdlibbuilder






