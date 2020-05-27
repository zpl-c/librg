CC=gcc
CXX=g++
STDC=-std=gnu11

ifeq ($(OS),Windows_NT)
	CFLAGS += -DWIN32
else
	OSDEF := $(shell uname -s)
	ifeq ($(OSDEF),Linux)
		LDFLAGS += -pthread -ldl -lm
	endif
	ifeq ($(OSDEF),OpenBSD)
		STDC=-std=c11
		CC=clang
		CXX=clang++
		LDFLAGS += -pthread -lm
	endif
	ifeq ($(OSDEF),FreeBSD)
		STDC=-std=c11
		CC=clang
		CXX=clang++
		LDFLAGS+=-pthread -lm
	endif
endif

WARNS = -Wall -Wextra -Werror -Wno-missing-field-initializers -Wno-unused-value -Wno-unused-function -Wno-missing-braces
CFLAGS += -g $(STDC) -Icode $(WARNS)
CXXFLAGS += -g -std=c++11 -Icode $(WARNS)

APPS += $(patsubst %.c,%,$(wildcard code/apps/*.c))
APPS += $(patsubst %.cc,%,$(wildcard code/apps/*.cc))

BUILD_FILES = $(wildcard build/*)

.PHONY: all clean apps test

all: clean apps test

test: clean code/tests/unit
	@echo '> Building unit tests'
	build/unit

apps: $(APPS)
	@echo '> Building apps'

clean:
ifneq ($(BUILD_FILES),)
	@echo '> Cleaning up files'
	@rm -r $(BUILD_FILES)
endif

% : %.c
	@mkdir -p build
	@echo '=> Building $(@F)'
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o build/$(@F)

% : %.cc
	@mkdir -p build
	@echo '=> Building $(@F)'
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o build/$(@F)
