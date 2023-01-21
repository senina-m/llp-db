#include "structs.h"
#include "../io/block_io.h"

size_t find_free_space(size_t block, size_t size, FILE* ptr);
void delete_block(size_t offset, FILE* ptr);