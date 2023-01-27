#include "commands.h"

bool compare_values(Condition* condition, Value_c* c, Value_t t){
    switch (condition->comp){
    case EQUAL:
        return equal_values(condition->value, c, t);
        break;
    case NOT_EQUAL:
        return !equal_values(condition->value, c, t);
        break;
    case NO_CONDITION:
        return true;
        break;
    default:
        cout << "invalid condition type!";
        exit(-1);
    }
}

void close_file(){
    close_file_bd();
}

void open_file(const char* filename){
    open_file_bd(filename);
}

Node* add_node(Value v, Node* parent){
    add_node_db(parent, v);
}

void delete_node(Node* node){
    delete_node_db(node);
}

Node* edit_node(Value v, Node* node){
    edit_node_db(node, v);
}

Iter find_nodes(Condition* condition, Node* start){
    
}