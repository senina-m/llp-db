#include "structs.h"
#include "../io/block_io.h"
#include "../io/node_io.h"
#include "../io/header_io.h"
#include "free_space.h"

Node* add_node_db(Node* parent, Value* value);
void edit_node_db(Node* node, Value* value);
void delete_node_db(Node* node);
void close_file_db();
void open_file_db(const char* fn);
size_t get_node_real_len(Node* node);

Node* read_offset_node(size_t offset);
Node* read_first_node();
bool check_not_head(size_t offset);
void replace_node_db(size_t new_offset, Node* node);
size_t get_tree_offset();
