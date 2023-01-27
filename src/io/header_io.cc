#include "header_io.h"

void write_header(File_header* header, FILE* ptr){
    size_t offset = START_OFFSET;
    offset = write_buffer_to_file(offset, ptr, &(header->tree_head_offset), 1, sizeof(size_t));
    offset = write_buffer_to_file(offset, ptr, &(header->free_space_offset), 1, sizeof(size_t));
    offset = write_buffer_to_file(offset, ptr, &(header->end_of_last_node_offset), 1, sizeof(size_t));
}

File_header* read_header(FILE* ptr){
    File_header* header = mmalloc(File_header);
    size_t offset = START_OFFSET;
    offset = read_buffer_from_file(offset, ptr, &(header->tree_head_offset), 1, sizeof(size_t));
    offset = read_buffer_from_file(offset, ptr, &(header->free_space_offset), 1, sizeof(size_t));
    offset = read_buffer_from_file(offset, ptr, &(header->end_of_last_node_offset), 1, sizeof(size_t));
    return header;
}

void print_header(FILE* ptr){
    size_t value;
    size_t offset = START_OFFSET;
    cout << "\noffset: " << START_OFFSET;
    offset = read_buffer_from_file(offset, ptr, &(value), 1, sizeof(size_t));
    cout << " nodes: " << value;
    offset = read_buffer_from_file(offset, ptr, &(value), 1, sizeof(size_t));
    cout << " blocks: " << value;
    offset = read_buffer_from_file(offset, ptr, &(value), 1, sizeof(size_t));
    cout << " end: " << value << endl;
}


size_t read_tree_head_offset(FILE* ptr){
    size_t roffset;
    read_buffer_from_file(TREE_HEAD_OFFSET, ptr, &(roffset), 1, sizeof(size_t));
    return roffset;
}

size_t read_free_space_offset(FILE* ptr){
    size_t roffset;
    read_buffer_from_file(FREE_SPACE_OFFSET, ptr, &(roffset), 1, sizeof(size_t));
    return roffset;
}

size_t read_end_of_last_node_offset(FILE* ptr){
    size_t roffset;
    read_buffer_from_file(END_OF_LAST_NODE_OFFSET, ptr, &(roffset), 1, sizeof(size_t));
    return roffset;
}

void write_tree_head_offset(size_t offset, FILE* ptr){
    write_buffer_to_file(TREE_HEAD_OFFSET, ptr, &(offset), 1, sizeof(size_t));
}

void write_free_space_offset(size_t offset, FILE* ptr){
    write_buffer_to_file(FREE_SPACE_OFFSET, ptr, &(offset), 1, sizeof(size_t));
}

void write_end_of_last_node_offset(size_t offset, FILE* ptr){
    write_buffer_to_file(END_OF_LAST_NODE_OFFSET, ptr, &(offset), 1, sizeof(size_t));
}