# Makefile to build class 'zerr_feature_tracker' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# library name

# add folder shortcut examples:
# INCLUDES = -I../includes
# TESTING = testing

CC  = g++
cflags = -std=c++11
ldflags += -L/usr/local/lib -lyaml-cpp
ldflags += -L/usr/local/lib -lfftw3


lib.name = zerr_feature_tracker~

# input source file (class name == source file basename)
class.sources = zerr_feature_tracker~.cpp

# zerr_feature_tracker pd wrapper
zerr_feature_tracker~.class.sources += zerr_feature_tracker.cpp

# zerr_feature_tracker modules
zerr_feature_tracker~.class.sources += ../../src/modules/featurebank.cpp

# # zerr_feature_tracker features
zerr_feature_tracker~.class.sources += ../../src/features/root_mean_square.cpp
zerr_feature_tracker~.class.sources += ../../src/features/centroid.cpp
zerr_feature_tracker~.class.sources += ../../src/features/zero_crossing_rate.cpp
zerr_feature_tracker~.class.sources += ../../src/features/flux.cpp

# # zerr_feature_tracker utils
zerr_feature_tracker~.class.sources += ../../src/utils/utils.cpp
zerr_feature_tracker~.class.sources += ../../src/utils/ringbuffer.cpp
zerr_feature_tracker~.class.sources += ../../src/utils/frequencytransformer.cpp
zerr_feature_tracker~.class.sources += ../../src/utils/linearinterpolator.cpp


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
CXX += -I/usr/local/include/fftw3

# use pd-lib-builder
include pd-lib-builder/Makefile.pdlibbuilder






