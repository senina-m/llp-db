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

# $(BUILDDIR)/commands.o: $(SRCDIR)/api/commands.cc $(BUILDDIR)/db_node.o
#       $(CC) $< -c -o $@

# $(BUILDDIR)/db_node.o: $(SRCDIR)/db/db_node.o $(BUILDDIR)/structs.o $(BUILDDIR)/block_io.o $(BUILDDIR)/header_io.o $(BUILDDIR)/free_space.o
#       $(CC) $< -c -o $@

# $(BUILDDIR)/free_space.o: $(SRCDIR)/db/free_space.cc $(BUILDDIR)/structs.o $(BUILDDIR)/block_io.o 
#       $(CC) $< -c -o $@

# $(BUILDDIR)/header_io.o: $(SRCDIR)/io/header_io.cc $(BUILDDIR)/file_io.o $(BUILDDIR)/structs.o
#       $(CC) $< -c -o $@

# $(BUILDDIR)/block_io.o: $(SRCDIR)/io/block_io.cc $(BUILDDIR)/file_io.o $(BUILDDIR)/structs.o
#       $(CC) $< -c -o $@

# $(BUILDDIR)/node_io.o: $(SRCDIR)/io/node_io.cc $(BUILDDIR)/file_io.o $(BUILDDIR)/structs.o
#       $(CC) $< -c -o $@

# $(BUILDDIR)/file_io.o: $(SRCDIR)/io/file_io.cc build
#       $(CC) $< -c -o $@

# $(BUILDDIR)/structs.o: $(SRCDIR)/db/structs.cc build
#       $(CC) $< -c -o $@

# $(BUILDDIR)/node_generator.o: $(SRCDIR)/test/generated_tests/node_generator.cc $(BUILDDIR)/commands.o
#       $(CC) $< -c -o $@

# $(BUILDDIR)/test_generator.o: $(SRCDIR)/test/generated_tests/test_generator.cc $(BUILDDIR)/node_generator.o
#       $(CC) $< -c -o $@

# $(BUILDDIR)/tests.o: $(SRCDIR)/test/hand_tests/tests.cc $(BUILDDIR)/commands.o
#       $(CC) $< -c -o $@


# main: $(SRCDIR)/main.cc $(BUILDDIR)/test_generator.o $(BUILDDIR)/tests.o
#       $(CC) $(ADRESSFLAGS) $^ -g -o $(BUILDDIR)/main
