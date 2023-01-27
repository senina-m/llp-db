#include "file_io.h"
#include <sys/stat.h>
#define LINUX

size_t write_buffer_to_file(size_t offset, FILE* ptr, const void* buffer, size_t buffer_len, size_t size);
size_t read_buffer_from_file(size_t offset, FILE* ptr, void* buffer, size_t buffer_len, size_t size);


#ifdef LINUX
FILE* open_file(const char* filename){

    //todo: почитать как этот костыль можно починить
    //this has create a file if it doesn't exist
    // FILE* fptr = fopen(filename, "wb");
    // fclose(fptr);


    FILE* ptr = fopen(filename, "rb+");
    if(!ptr) {
        cout << "can't open a file "<< filename << " !" << endl;
        exit(-1); 
    }
    return ptr;
}

size_t write_buffer_to_file(size_t offset, FILE* ptr, const void* buffer, size_t buffer_len, size_t size){
    fseek(ptr, offset, SEEK_SET); //move readinng pointer to ofset from the file beginning.
    fwrite(buffer, size, buffer_len, ptr); // write bytes from our buffer
    return offset + size * buffer_len;
}

size_t read_buffer_from_file(size_t offset, FILE* ptr, void* buffer, size_t buffer_len, size_t size){
    fseek(ptr, offset, SEEK_SET); //move readinng pointer to ofset from the file beginning.
    fread(buffer, size, buffer_len, ptr); // read bytes to our buffer
    return offset + size * buffer_len;
}

size_t get_file_len(const char* filename){
    // if( fseek(ptr, 0, SEEK_END) )return -1;
    // else return ftell(ptr);
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

void close_file(FILE* ptr){
    fclose(ptr);
}


#endif

#ifdef WINDOWS
//...

#endif

