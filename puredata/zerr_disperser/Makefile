# Makefile to build object 'zerr_disperser~' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# add folder shortcut examples:
# INCLUDES = -I../includes
# TESTING = testing

CC  = g++
cflags = -std=c++11
# ldflags += -L../../dependencies/lib/yaml-cpp -lyaml-cpp    # keep it for future

lib.name = zerr_disperser~

# input source file (class name == source file basename)
class.sources = zerr_disperser~.cpp

# zerr_disperser pd wrapper
zerr_disperser~.class.sources += zerr_disperser.cpp

# zerr_disperser modules
zerr_disperser~.class.sources += ../../src/modules/audiodisperser.cpp

# zerr_disperser utils
zerr_disperser~.class.sources += ../../src/utils/logger.cpp
zerr_disperser~.class.sources += ../../src/utils/utils.cpp

# zerr_disperser headers
CC  += -I../../include/utils
CC  += -I../../include/modules

CXX += -I../../include/utils
CXX += -I../../include/modules

# CC  += -I../../dependencies/include/yaml-cpp    # keep it for future
# CXX += -I../../dependencies/include/yaml-cpp    # keep it for future

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
include pd-lib-builder/Makefile.pdlibbuilder



