# CFLAGS=--std=c17 -Wall -pedantic -Isrc/ -ggdb -Wextra -Werror -DDEBUG
BUILDDIR=build
SRCDIR=src
CC=g++
ADRESSFLAGS=-fsanitize=address
# ADRESSFLAGS=

# When wildcard % appears in the dependency list, it is replaced with 
# the same string that was used to perform substitution in the target.

# Inside actions we can use:  
#     $@ to represent the full target name of the current target  
#     $? returns the dependencies that are newer than the current target  
#     $* returns the text that corresponds to % in the target     
#     $< returns the name of the first dependency 
#     $^ returns the names of all the dependencies with space as the delimiter


all: main

build:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/file_io.o: $(SRCDIR)/io/file_io.cc build
	$(CC) $< -c -o $@

$(BUILDDIR)/node_io.o: $(SRCDIR)/io/node_io.cc build
	$(CC) $< -c -o $@

$(BUILDDIR)/structs.o: $(SRCDIR)/db/structs.cc build
	$(CC) $< -c -o $@

$(BUILDDIR)/main.o: $(SRCDIR)/main.cc build
	$(CC) $< -c -o $@

main: build
	$(CC) $(ADRESSFLAGS) $(SRCDIR)/main.cc $(SRCDIR)/db/structs.cc $(SRCDIR)/io/file_io.cc $(SRCDIR)/io/block_io.cc $(SRCDIR)/io/node_io.cc $(SRCDIR)/db/db_node.cc $(SRCDIR)/db/free_space.cc $(SRCDIR)/test/tests.cc -o./build/main

clean:
	rm -rf $(BUILDDIR)

# g++ ./src/structs.cc ./src/util.cc ./src/main.cc -Iinclude/ -o ./build/main