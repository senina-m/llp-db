#include <stdio.h>
#include <iostream>

using namespace std; 

FILE* open_file(const char* filename);
size_t write_buffer_to_file(size_t offset, FILE* ptr, const void* buffer, size_t buffer_len, size_t size);
size_t read_buffer_from_file(size_t offset, FILE* ptr, void* buffer, size_t buffer_len, size_t size);
size_t get_file_len(const char* filename);
// size_t get_file_len(FILE* ptr);
void close_file(FILE* ptr);