# Makefile to build object 'zerr_combinator~' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# add folder shortcut examples:
# INCLUDES = -I../includes
# TESTING = testing

CC  = g++
cflags = -std=c++11
# ldflags += -L../../dependencies/lib/yaml-cpp -lyaml-cpp    # keep it for future

lib.name = zerr_combinator~

# input source file (class name == source file basename)
class.sources = zerr_combinator~.cpp

# zerr_combinator pd wrapper
zerr_combinator~.class.sources += zerr_combinator.cpp

# zerr_combinator modules
zerr_combinator~.class.sources += ../../src/modules/envelopecombinator.cpp

# zerr_combinator utils
zerr_combinator~.class.sources += ../../src/utils/logger.cpp

# zerr_combinator headers
CC  += -I../../include/utils
CC  += -I../../include/modules

CXX += -I../../include/utils
CXX += -I../../include/modules

# CC  += -I../../dependencies/include/yaml-cpp    # keep it for future
# CXX += -I../../dependencies/include/yaml-cpp    # keep it for future

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
include pd-lib-builder/Makefile.pdlibbuilder



