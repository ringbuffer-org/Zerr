# Makefile to build object 'zerr_envelope_generator' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# add folder shortcut examples:
# INCLUDES = -I../includes
# TESTING = testing

CC  = g++
cflags = -std=c++11
ldflags += -L../../dependencies/lib/yaml-cpp 
ldlibs += -lyaml-cpp

lib.name = zerr_envelope_generator~

# input source file (class name == source file basename)
class.sources = zerr_envelope_generator~.cpp

# zerr_envelope_generator pd wrapper
zerr_envelope_generator~.class.sources += zerr_envelope_generator.cpp

# zerr_envelope_generator modules
zerr_envelope_generator~.class.sources += ../../src/modules/envelopegenerator.cpp
zerr_envelope_generator~.class.sources += ../../src/modules/speakermanager.cpp

# zerr_envelope_generator utils
zerr_envelope_generator~.class.sources += ../../src/utils/logger.cpp
zerr_envelope_generator~.class.sources += ../../src/utils/utils.cpp
# zerr_envelope_generator~.class.sources += ../../src/utils/ringbuffer.cpp            # keep it for future
# zerr_envelope_generator~.class.sources += ../../src/utils/linearinterpolator.cpp    # keep it for future

# zerr_envelope_combinator headers
CC  += -I../../include/utils
CC  += -I../../include/modules

CXX += -I../../include/utils
CXX += -I../../include/modules

CC  += -I../../dependencies/include/yaml-cpp
CXX += -I../../dependencies/include/yaml-cpp

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
include pd-lib-builder/Makefile.pdlibbuilder



