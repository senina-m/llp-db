#include <stdint.h>
#include <iostream>
#include <string>
#include <string.h>
// #include <stdio.h>

#pragma once

using namespace std;

#define mmalloc(T) ((T*)malloc(sizeof(T)))
#define mmalloc_array(T, count) ((T*)malloc(sizeof(T)*count))

enum  Value_t {int_type=0, double_type, string_type, bool_type};

union contents{
    int int_value;
    double double_value;
    const char* string_value;
    bool bool_value;
};

typedef union contents Value_c;

typedef struct{
    Value_c c;
    Value_t t;
} Value;


#define NODE_HEDER_SIZE sizeof(size_t)*5
#define NODE_VALUE_META_SIZE sizeof(size_t) + sizeof(int8_t)
#define NODE_NEXT_SIBLING_OFFSET sizeof(size_t)*4
#define NODE_PREV_SIBLING_OFFSET sizeof(size_t)*3
#define NODE_FIRST_CHILD_OFFSET sizeof(size_t)
#define NODE_PARENT_OFFSET sizeof(size_t)*2
#define BOOL_NODE_SIZE NODE_HEDER_SIZE+NODE_VALUE_META_SIZE + sizeof(bool)
typedef struct Node{
    size_t offset; //doesn't persist in file

    //node header
    size_t node_len;
    size_t first_child; // first child offest
    size_t parent; //parent offset
    size_t prev_sibling; // previos sibling offset 
    size_t next_sibling; // previos sibling offset 

    //value part of struct
    size_t value_len;
    int8_t value_t;
    Value_c value_c;
} Node;


#define BLOCK_SIZE sizeof(size_t)*3
#define DEFAULT_BLOCK_SIZE BLOCK_SIZE
#define BLOCK_SIZE_OFFSET  sizeof(size_t)*2
#define BLOCK_PREV_OFFSET  sizeof(size_t)
#define BLOCK_NEXT_OFFSET 0
typedef struct Block{
    size_t next;
    size_t prev;
    size_t size;
} Block;

#define START_OFFSET 0
#define FILE_HEADER_SIZE sizeof(size_t)*3
#define TREE_HEAD_OFFSET 0
#define FREE_SPACE_OFFSET sizeof(size_t)
#define END_OF_LAST_NODE_OFFSET sizeof(size_t)*2
typedef struct File_header{
    size_t tree_head_offset;
    size_t free_space_offset;
    size_t end_of_last_node_offset;
} File_header;

size_t get_value_size(Value_c* c, Value_t t);
string value_to_string(Value_c* c, Value_t t);
Value_t get_t_by_int(int8_t t);
bool equal_values(Value* value, Value_c* c, Value_t t);
void fill_nodes_value_meta(Node* node, Value* value);

void free_node(Node* node);
void free_value(Node* node);
void print_node_header(Node* node);
void print_node(Node* node);

void free_block(Block* block);
void print_block(Block* block);