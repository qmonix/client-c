#
# This makefile builds static library for qmonix C client.
# O3 optimization is used.
#
# To customize build you can preset CC or USER_C_FLAGS variables.
# E.g. CC=gcc-4.5 USER_C_FLAGS=-m32 make
# CC - sets compiler command.
# USER_C_FLAGS - sets compiler flags.
#

CC ?= gcc
USER_CFLAGS ?=

PROJECT_BASE_DIR = .
INCLUDES = \
	-iquote$(PROJECT_BASE_DIR)/include \
	-iquote$(PROJECT_BASE_DIR)/src \
	-I$(PROJECT_BASE_DIR)/include \
	-I$(PROJECT_BASE_DIR)/lib

CFLAGS ?= -O3 -W -Wall -Wextra -Werror -ggdb -std=gnu99 $(INCLUDES) \
	$(USER_CFLAGS)
LDFLAGS ?= -pthread -lcurl

AR = ar rcu
RANLIB = ranlib


SRC_DIR = src
LIB_DIR = lib
BUILD_DIR = build

QMONIX_CLIENT_A = libqmonix_client.a

# Collect all *.o files to build.
OBJS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(OBJS:.c=.o) json.o
OBJS := $(OBJS:$(SRC_DIR)/%=%)
OBJS := $(addprefix $(BUILD_DIR)/, $(OBJS))

all: static-lib
.PHONY: all


static-lib: $(BUILD_DIR) $(BUILD_DIR)/$(QMONIX_CLIENT_A)
.PHONY: static-lib


$(BUILD_DIR)/$(QMONIX_CLIENT_A): $(OBJS)
	$(AR) $@ $^
	$(RANLIB) $@


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/json.o: lib/json/json.c lib/json/json.h
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


clean:
	rm -rf $(BUILD_DIR)
.PHONY: clean
