# Makefile for CS-202 course - spring 2026
# Authors: Lucie Mermod, Jean-Cédric Chappelier & Édouard Bugnion
# Date: March 2026

# main targets

# 	style: astyle formatting
# 	doc: create documentation

# Note: builds with address sanitizer by default

TEST_DIR = $(PWD)/../provided/tests

## You can change the tag to run week07, week08, etc.
IMAGE := chappeli/cs202-feedback:latest

#########################################################################
# DO NOT EDIT BELOW THIS LINE
#

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
# add the debug flag, may need to comment this line when doing make feedback
#TODO : Make feedback should build with -UDEBUG
CPPFLAGS += -DDEBUG
endif

.PHONY: all all-deferred

# list of C files the .o of which shall not be automatically added to OBJS list
EXCLUDE_SRCS = udp-test-client.c udp-test-server.c tcp-test-client.c tcp-test-server.c

SRCS = $(filter-out $(EXCLUDE_SRCS), $(wildcard *.c))

LDLIBS += -lm -lssl -lcrypto

OBJS=$(subst .c,.o,$(SRCS))

all:: all-deferred

udp: udp-test-client udp-test-server
udp-test-client: udp-test-client.o socket_layer.o
udp-test-server: udp-test-server.o socket_layer.o

tcp: tcp-test-client tcp-test-server
tcp-test-client: tcp-test-client.o socket_layer.o util.o
tcp-test-server: tcp-test-server.o socket_layer.o util.o

ifneq (,$(wildcard ./udp-test-*.c))
TARGETS += udp
endif
ifneq (,$(wildcard ./tcp-test-*.c))
TARGETS += tcp
endif

all-deferred:: $(TARGETS)

.PHONY: depend clean new static-check doc

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

new: clean all

static-check:
	CCC_CC=$(CC) scan-build -analyze-headers --status-bugs -maxloop 64 make -j1 new

style:
	astyle -n -o -A8 -xt0 *.[ch]

## --------------------------------------------------
# target to make documentation
doc: Doxyfile
	doxygen Doxyfile
