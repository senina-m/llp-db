#include "block_io.h"


void write_block(Block* block, FILE* ptr, size_t offset){
    offset = write_buffer_to_file(offset, ptr, &(block->next), 1, sizeof(size_t));
    offset = write_buffer_to_file(offset, ptr, &(block->prev), 1, sizeof(size_t));
    offset = write_buffer_to_file(offset, ptr, &(block->size), 1, sizeof(size_t));
}

Block* read_block(FILE* ptr, size_t offset){
    Block* block = mmalloc(Block);
    offset = read_buffer_from_file(offset, ptr, &(block->next), 1, sizeof(size_t));
    offset = read_buffer_from_file(offset, ptr, &(block->prev), 1, sizeof(size_t));
    offset = read_buffer_from_file(offset, ptr, &(block->size), 1, sizeof(size_t));
    return block;
}

size_t read_block_size(FILE* ptr, size_t offset){
    size_t block_size = 0;
    read_buffer_from_file(offset + BLOCK_SIZE_OFFSET , ptr, &block_size, 1, sizeof(size_t));
    return block_size;
}

size_t read_block_next(FILE* ptr, size_t offset){
    size_t block_next = 0;
    read_buffer_from_file(offset + BLOCK_NEXT_OFFSET, ptr, &block_next, 1, sizeof(size_t));
    return block_next;
}

size_t read_block_prev(FILE* ptr, size_t offset){
    size_t block_prev = 0;
    read_buffer_from_file(offset + BLOCK_PREV_OFFSET, ptr, &block_prev, 1, sizeof(size_t));
    return block_prev;
}

void update_block_prev(FILE* ptr, size_t offset, size_t new_prev_offset){
    write_buffer_to_file(offset + BLOCK_PREV_OFFSET, ptr, &new_prev_offset, 1, sizeof(size_t));
}

void update_block_next(FILE* ptr, size_t offset, size_t new_next_offset){
    write_buffer_to_file(offset + BLOCK_NEXT_OFFSET, ptr, &new_next_offset, 1, sizeof(size_t));
}