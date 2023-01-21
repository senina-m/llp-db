#include "free_space.h"

/// @param block first block in block list
size_t find_free_space(size_t block, size_t size, FILE* ptr){
    size_t next;
    do{ 
        // cout << "\noffset:" << block << ", size:" << read_block_size(ptr, block);
        // cout << ", next:" << read_block_next(ptr, block)<< endl;
        next = read_block_next(ptr, block);
        if(read_block_size(ptr, block) >= size) return block;
        else block = next;
    } while(next != 0);
    return 0;
}

void delete_block(size_t offset, FILE* ptr){
    size_t next = read_block_next(ptr, offset);
    size_t prev = read_block_prev(ptr, offset);
    update_block_next(ptr, prev, next);
    update_block_prev(ptr, next, prev);
}