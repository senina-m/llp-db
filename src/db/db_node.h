#include "structs.h"
#include "../io/block_io.h"
#include "../io/node_io.h"
#include "free_space.h"

Node* add_node_db(Node* parent, Value* value, size_t free_space_offset, size_t* end_of_nodes, FILE* ptr);
Node* edit_node_db(Node* node, Value* value, size_t free_space_offset, size_t* end_of_nodes, FILE* ptr);