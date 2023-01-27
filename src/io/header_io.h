#include "file_io.h"
#include "../db/structs.h"

void write_header(File_header* header, FILE* ptr);
File_header* read_header(FILE* ptr);
size_t read_tree_head_offset(FILE* ptr);
size_t read_free_space_offset(FILE* ptr);
size_t read_end_of_last_node_offset(FILE* ptr);
void write_tree_head_offset(size_t offset, FILE* ptr);
void write_free_space_offset(size_t offset, FILE* ptr);
void write_end_of_last_node_offset(size_t offset, FILE* ptr);
void print_header(FILE* ptr);