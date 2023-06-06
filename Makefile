# Makefile to build class 'zerr' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# library name

# add folder shortcut examples:
# INCLUDES = -I../includes
# TESTING = testing

CC  = g++
cflags = -std=c++11
ldflags += -L/usr/local/lib -lyaml-cpp

lib.name = zerr~

# input source file (class name == source file basename)
class.sources = zerr~.cpp

# zerr pd wrapper
zerr~.class.sources += zerr.cpp

# common.sources = $(filter-out $(zerr~.class.sources), $(shell find "src" -name "*.c"))
# $(info common.sources: $(common.sources))

# # zerr modules
zerr~.class.sources += src/modules/audiorouter.cpp
zerr~.class.sources += src/modules/mapper.cpp
zerr~.class.sources += src/modules/featurebank.cpp
zerr~.class.sources += src/modules/speakerarray.cpp
zerr~.class.sources += src/modules/featureextractor.cpp
zerr~.class.sources += src/modules/trajectorygenerator.cpp

# # zerr features
zerr~.class.sources += src/features/centroid.cpp
zerr~.class.sources += src/features/zerocrossing.cpp

# # zerr utils
zerr~.class.sources += src/utils/utils.cpp

# all extra files to be included in binary distribution of the library
datafiles =

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
PDLIBBUILDER_DIR=pd-lib-builder/

CC  += -Iinclude
CC  += -Iinclude/utils
CC  += -Iinclude/modules
CC  += -Iinclude/features
CXX += -Iinclude
CXX += -Iinclude/utils
CXX += -Iinclude/modules
CXX += -Iinclude/features
CXX += -I/usr/local/include/yaml-cpp

# use pd-lib-builder
include pd-lib-builder/Makefile.pdlibbuilder

# disable optimizations for debugging
# alldebug: c.flags += -O0 -DDEBUG
# alldebug: cxx.flags += -O0 -DDEBUG

# lib.name = goat~
# goat~.class.sources = src/goat_tilde.c
# common.sources = $(filter-out $(goat~.class.sources), $(shell find "src" -name "*.c"))
# $(info common.sources: $(common.sources))
# datafiles = $(wildcard goat_tilde.pd *.wav preset_*.txt)


# # disable optimizations for debugging
# alldebug: c.flags += -O0 -DDEBUG
# alldebug: cxx.flags += -O0 -DDEBUG

# # create the documentation
# DOXYGEN=doxygen
# DOXYGEN_DIR=docs
# .PHONY: docs docs.clean

# docs:
# 	mkdir -p $(DOXYGEN_DIR)
# 	$(DOXYGEN)

# # clean the documentation directory
# clean: docs.clean
# docs.clean:
# 	rm -rf $(DOXYGEN_DIR)

# # valgrind command for leak checks
# VG=valgrind
# VG_LOG="valgrind.log"
# VG_FLAGS=--log-file="$(VG_LOG)" --leak-check=full --show-reachable=yes --track-origins=yes -s
# .PHONY: valgrind valgrind.clean

# valgrind: alldebug
# 	$(VG) $(VG_FLAGS) pd -noaudio *.pd

# clean: valgrind.clean
# valgrind.clean:
# 	rm -f $(VG_LOG)





