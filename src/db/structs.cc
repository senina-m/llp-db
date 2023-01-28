#include "structs.h"

size_t get_value_size(Value_c* c, Value_t t){
    switch (t) {
    case int_type:
        return sizeof(int);
    case double_type:
        return sizeof(double);
    case string_type:
        return strlen(c->string_value) + 1;
    case bool_type:
        return sizeof(bool);
    default:
        cout << "Wrong type " << t << endl;
        exit(-1); 
    }
}

string value_to_string(Value_c* c, Value_t t){
    switch (t) {
    case int_type:
        return to_string(c->int_value);
    case double_type:
        return to_string(c->double_value);
    case string_type:
    {
        // printf("   addr: %p", c->string_value);
        string str(c->string_value);
        return str;
    }
    case bool_type:
        return to_string(c->bool_value);
    default:
        cout << "Wrong type " << t << endl;
        exit(-1); 
    }
}

Value_t get_t_by_int(int8_t t){
    switch (t) {
    case 0:
        return int_type;
    case 1:
        return double_type;
    case 2:
        return string_type;
    case 3:
        return bool_type;
    default:
        cout << "Wrong type " << t << endl;
        exit(-1); 
    }
}

bool equal_values(Value* value, Value_c* c, Value_t t){
    if(t == value->t){
        switch (t) {
        case 0:
            return value->c.int_value == c->int_value;
        case 1:
            return value->c.double_value == c->double_value;
        case 2:
            return strcmp(value->c.string_value, c->string_value);
        case 3:
            return value->c.bool_value == c->bool_value;
        default:
            cout << "Wrong type " << t << endl;
            exit(-1); 
        }
    }
    return false;
}

void fill_nodes_value_meta(Node* node, Value* value){
    free_value(node);
    node->value_len = get_value_size(&(value->c), value->t);
    node->value_t = (int8_t) value->t;
    node->value_c = value->c;

        //to copy string values
        // int size = strlen(value->c.string_value) + 1;
        // // cout << size << " sizeof(" << value_to_string(&(value->c), value->t) << ")" << endl;
        // const char* str = mmalloc_array(char, size);
        // strncpy((char*) str, value->c.string_value, size);
        // node->value_c.string_value = str;
}


//TODO: check if it's propper
void free_node(Node* node){
    free_value(node);
    free(node);
}

void free_value(Node* node){
    if(!node) return;
    if(node->value_t == string_type){
        free((char*)(node->value_c.string_value));
    }
}

void print_node_header(Node* node){
    cout << "offset:" << node->offset;

    cout << ", node_len:" << node->node_len;
    cout << ", first_child:" << node->first_child;
    cout << ", parent:" << node->parent;
    cout << ", prev_sibling:" << node->prev_sibling;
    cout << ", next_sibling:" << node->next_sibling;
}


void print_node(Node* node){
    print_node_header(node);
    cout << ", t:" << (int) node->value_t;
    cout << ", v:" << value_to_string(&(node->value_c), get_t_by_int(node->value_t));
}


void free_block(Block* block){
    free(block);
}

void print_block(Block* block){
    cout << "next:" << block->next;
    cout << ", prev:" << block->prev;
    cout << ", size:" << block->size;
}