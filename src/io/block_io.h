#include "../db/structs.h"
#include "file_io.h"

void write_block(Block* block, FILE* ptr, size_t offset);
void write_block(size_t next, size_t prev, size_t size, FILE* ptr, size_t offset);
Block* read_block(FILE* ptr, size_t offset);
size_t read_block_size(FILE* ptr, size_t offset);
size_t read_block_next(FILE* ptr, size_t offset);
size_t read_block_prev(FILE* ptr, size_t offset);
void update_block_prev(FILE* ptr, size_t offset, size_t new_prev_offset);
void update_block_next(FILE* ptr, size_t offset, size_t new_next_offset);