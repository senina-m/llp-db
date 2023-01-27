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

void delete_block(size_t offset, size_t* tree_head_offset, FILE* ptr){
    size_t next = read_block_next(ptr, offset);
    size_t prev = read_block_prev(ptr, offset);

    if(prev == 0){ //первый в списке
        *tree_head_offset = next;
        update_block_prev(ptr, next, 0);
    }else if (next == 0){//последний в списке
        update_block_next(ptr, prev, 0);
    }else{// в середине
        update_block_next(ptr, prev, next);
        update_block_prev(ptr, next, prev);
    }

    //ВАЖНО! единственным быть не может, потому что
    //первая дырка при старте специально делается маленькой
    //чтобы в неё не влезла ни одна нода, но чтобы не возиться при её удалении
}

void create_block(size_t* free_space_offset, size_t block_offset, size_t block_size, FILE* ptr){
    //записываем в бывший первый блок наш новый блок как первый
    update_block_prev(ptr, *free_space_offset, block_offset);
    write_block(*free_space_offset, 0, block_size, ptr, block_offset);
    // записывааем в free_space_offset местоположение нового блока
    *free_space_offset = block_offset;
}

void print_free_space(size_t free_space_offset, FILE* ptr){
    size_t next;
    size_t block = free_space_offset;
    do{ 
        cout << "offset:" << block << ", size:" << read_block_size(ptr, block);
        cout << ", next:" << read_block_next(ptr, block)<< ", prev:" << read_block_prev(ptr, block) << endl;
        next = read_block_next(ptr, block);
        block = next;
    } while(next != 0);
}