#include "commands.h"

const char* filename;
vector<Block*> free_space;
size_t end_of_last_node_offset;
File_node* head;


bool static compare_values(Condition* condition, Value_c* c, Value_t t){
    switch (condition->comp){
    case EQUAL:
        return equal_values(condition->value, c, t);
        break;
    case NOT_EQUAL:
        return !equal_values(condition->value, c, t);
        break;
    default:
        cout << "invalid condition type!";
        exit(-1);
    }
}

void open_file(const char* fn){
    filename = fn;
    size_t size = get_file_len(fn);
    if(size <= 1){ // if file is empty
        Node* node_head = create_tree();
        free_space  = 
    }else{ // if file isn't empty
        size_t offset = 0;
        size_t blocks_offset;
        read_buffer_from_file(&offset, filename, &blocks_offset, 1, sizeof(size_t));
        read_blocks(blocks_offset, size);
        offset += sizeof(size_t);
        head = read_one_file_node(&offset, filename);
    }
}

void close_file(){
    
}


bool add_node(Value v, Node* parent){}
bool delete_node(Node* parent){}
bool edit_node(Value v, Node* node){}
bool find_nodes(Condition* condition, Node* start, vector<Node*> list){}