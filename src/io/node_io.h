#include "../db/structs.h"
#include "file_io.h"


size_t write_value_to_file(Value_c* c, Value_t t, size_t offset, FILE* ptr);
void write_node(Node* node, FILE* ptr);

size_t read_value_from_file (Value_c* c, int8_t type, size_t size, size_t offset, FILE* ptr);
Node* read_node_header(size_t offset, FILE* ptr);
Node* read_node(size_t offset, FILE* ptr);

size_t read_node_len(size_t node, FILE* ptr);
size_t read_first_child_offset(size_t cur_offset, FILE* ptr);
size_t read_next_sibling_offset(size_t cur_offset, FILE* ptr);
void change_next_sibling_offset(size_t node_offset, size_t next_sibling_offset, FILE* ptr);
void change_prev_sibling_offset(size_t node_offset, size_t prev_sibling_offset, FILE* ptr);

/// @brief Change node's first child
/// @param node_offset parent node
/// @param first_child_offset parent's new child offset
void change_first_child_offset(size_t node_offset, size_t first_child_offset, FILE* ptr);
void change_parent_offset(size_t cur_offset, size_t parent_offset, FILE* ptr);
void write_node_value(Node* node, FILE* ptr);
void print_tree(size_t tree_offset, FILE* ptr, size_t spaces);

void update_node(Node* node, FILE* ptr);