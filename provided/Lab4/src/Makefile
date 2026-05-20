# Makefile for CS-202 course
# Authors: Lucie Mermod, Jean-Cédric Chappelier & Édouard Bugnion
# Date: March 2026

# main targets
# 	all: build the u6fs (early unix filesystem) program
# 	style: astyle formatting
# 	doc: create documentation
#       check: local unit tests

#TEST_DIR = $(PWD)/../provided/tests
TEST_DIR = ../tests

#########################################################################
# DO NOT EDIT BELOW THIS LINE
#

# Note: builds with address sanitizer by default
TARGETS += u6fs

# ======================================================================
# Compiler and Link

export CC = clang

CFLAGS += -g
CFLAGS += -pedantic -Wall
CFLAGS += -Wextra -Wfloat-equal -Wshadow -Wpointer-arith -Wbad-function-cast -Wwrite-strings \
-Wconversion -Wunreachable-code -Wcast-qual -W -Wformat=2 -Winit-self -Wuninitialized

CFLAGS += -Wcast-align

## may require: export ASAN_OPTIONS=allocator_may_return_null=1
#               export ASAN_OPTIONS=verify_asan_link_order=0
# different -fsanitize options are available, including -fsanitize=memory
CPPFLAGS += -fsanitize=address
LDFLAGS  += -fsanitize=address

ifdef DEBUG
CPPFLAGS += -DDEBUG
endif

.PHONY: all all-deferred

SRCS = u6fs.c error.c u6fs_utils.c

# START (provided as an example)
SRCS += mount.c
SRCS += sector.c inode.c
SRCS += filev6.c
SRCS += direntv6.c

LDLIBS += -lm -lssl -lcrypto

OBJS=$(subst .c,.o,$(SRCS))

all:: all-deferred

u6fs: $(OBJS)
	$(LINK.o) -o $@ $(OBJS) $(LDLIBS)

all-deferred:: $(TARGETS)

.PHONY: depend clean new static-check check release doc

# automatically generate the dependencies
# including .h dependencies !

depend: .depend

.depend: $(SRCS)
	bash -c "cat $(foreach SRC,$(SRCS),<($(CC) $(CFLAGS) -MM -MT $(SRC:.c=.o) $(SRC))) > ./.depend"

ifneq ($(MAKECMDGOALS),clean)
-include .depend
endif

clean::
	-@/bin/rm -f *.o *~  .depend $(TARGETS)
	$(MAKE) -C $(TEST_DIR)/unit clean

new: clean all

static-check:
	CCC_CC=$(CC) scan-build -analyze-headers --status-bugs -maxloop 64 make -j1 new

style:
	astyle -n -o -A8 -xt0 *.[ch]

unit-tests:
	make SRC_DIR=$${PWD} -B -C $(TEST_DIR)/unit

test-%:
	make SRC_DIR=$${PWD} -B -C $(TEST_DIR)/unit $*

define e2e_test
robot -v srcdir:. -v datadir:$(TEST_DIR)/data $(TEST_DIR)/end-to-end/$1 ;
endef
end2end-tests: $(TARGETS)
	$(call e2e_test,week04.robot)

check: end2end-tests unit-tests

## --------------------------------------------------
# target to do all checks before releasing a new version by staff
release: new check style static-check clean

## --------------------------------------------------
# target to make documentation
doc: Doxyfile
	doxygen Doxyfile

## --------------------------------------------------
