ROOT_DIR=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
CC=cc_args.py clang
CFLAGS=-I../include -std=c99 -g -pthread -lm -ldl

all:
	$(CC) quick.c -o quick $(CFLAGS)

test:
	./quick

debug:
	hbg $(ROOT_DIR)/quick $(ROOT_DIR)
