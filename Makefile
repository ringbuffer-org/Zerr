# Makefile to build class 'zerr' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# library name

# add folder shortcut examples:
# INCLUDES = -I../includes
# TESTING = testing

lib.name = rtap_delay6~

# input source file (class name == source file basename)
class.sources = rtap_delay6~.c

rtap_delay6~.class.sources += vas_delay.c
rtap_delay6~.class.sources += vas_mem.c


# all extra files to be included in binary distribution of the library
datafiles =

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
PDLIBBUILDER_DIR=pd-lib-builder/

CC += $(INCLUDES)
# CC +=  -mavx -DVAS_USE_AVX

include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder




cflags += -Iinclude
cflags += -std=gnu11
cflags += -Wall -Wextra

lib.name = goat~
goat~.class.sources = src/goat_tilde.c
common.sources = $(filter-out $(goat~.class.sources), $(shell find "src" -name "*.c"))
$(info common.sources: $(common.sources))
datafiles = $(wildcard goat_tilde.pd *.wav preset_*.txt)

# use pd-lib-builder
include pd-lib-builder/Makefile.pdlibbuilder

# disable optimizations for debugging
alldebug: c.flags += -O0 -DDEBUG
alldebug: cxx.flags += -O0 -DDEBUG

# create the documentation
DOXYGEN=doxygen
DOXYGEN_DIR=docs
.PHONY: docs docs.clean

docs:
	mkdir -p $(DOXYGEN_DIR)
	$(DOXYGEN)

# clean the documentation directory
clean: docs.clean
docs.clean:
	rm -rf $(DOXYGEN_DIR)

# valgrind command for leak checks
VG=valgrind
VG_LOG="valgrind.log"
VG_FLAGS=--log-file="$(VG_LOG)" --leak-check=full --show-reachable=yes --track-origins=yes -s
.PHONY: valgrind valgrind.clean

valgrind: alldebug
	$(VG) $(VG_FLAGS) pd -noaudio *.pd

clean: valgrind.clean
valgrind.clean:
	rm -f $(VG_LOG)





