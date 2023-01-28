# CFLAGS=--std=c17 -Wall -pedantic -Isrc/ -ggdb -Wextra -Werror -DDEBUG
BUILDDIR=build
SRCDIR=src
CC=g++

main: build
	$(CC) $(SRCDIR)/main.cc $(SRCDIR)/db/structs.cc $(SRCDIR)/io/file_io.cc $(SRCDIR)/io/block_io.cc $(SRCDIR)/io/node_io.cc $(SRCDIR)/io/header_io.cc $(SRCDIR)/db/db_node.cc $(SRCDIR)/db/free_space.cc $(SRCDIR)/api/commands.cc $(SRCDIR)/test/generated_tests/node_generator.cc $(SRCDIR)/test/generated_tests/test_generator.cc  $(SRCDIR)/test/hand_tests/tests.cc -g -o ./build/main


build:
	mkdir -p $(BUILDDIR)

all: main

clean:
	rm -rf $(BUILDDIR)

# g++ ./src/structs.cc ./src/util.cc ./src/main.cc -Iinclude/ -o ./build/main