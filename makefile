UNAME := $(shell uname)
SHELL := /bin/bash

CC   := clang
ifeq (${CC}, gcc)
  GCOV := gcov
else ifeq ($(shell command -v llvm-cov-19 2> /dev/null),)
  GCOV := llvm-cov gcov
else
  GCOV := llvm-cov-19 gcov
endif

LIBS = src
TEST = tests/main.c

INCLUDES     = $(addprefix -I, $(LIBS))
SOURCES      = $(foreach dir, $(LIBS), $(wildcard $(dir)/*.c))
SRC_FILES    = $(foreach dir, $(LIBS), $(wildcard $(dir)/*.c)) $(foreach dir, $(LIBS), $(wildcard $(dir)/*.h))
CFLAGS       = -std=c99 -Wall -g -Os -flto ${INCLUDES} ${SOURCES}
TEST_OUTPUT  = build/test

.PHONY: clean
clean:
	rm -rf build/*

.PHONY: test
test: clean
	mkdir -p build
	${CC} ${CFLAGS} ${TEST} -o ${TEST_OUTPUT}
	chmod +x ${TEST_OUTPUT}
	${TEST_OUTPUT}

.PHONY: coverage
coverage: clean
	mkdir -p build
	${CC} ${CFLAGS} -fprofile-arcs -ftest-coverage ${TEST} -o ${TEST_OUTPUT}
	chmod +x ${TEST_OUTPUT}
	${TEST_OUTPUT} > build/test.out || { cat build/test.out; exit 1; }
	${GCOV} src/microcuts.c -o ${TEST_OUTPUT}-microcuts.gcda > /dev/null
	python3 tools/coverage.py | grep -v string | tee build/coverage.out