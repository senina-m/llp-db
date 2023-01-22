#include "structs.h"
#include "../io/block_io.h"

size_t find_free_space(size_t block, size_t size, FILE* ptr);
void delete_block(size_t offset, FILE* ptr);
void create_block(size_t* free_space_offset, size_t block_offset, size_t block_size, FILE* ptr);
void print_free_space(size_t free_space_offset, FILE* ptr);