#include "../db/db_node.h"
#pragma once

enum Compare{
    EQUAL,
    NOT_EQUAL,
    NO_CONDITION
};

const string compare_str[] = {
    "equal",
    "not_equal",
    "no_condition"
};

typedef struct Condition{
    Value value;
    Compare comp;

    Condition(Value new_value, Compare new_comp)
            : value{move(new_value)}, comp{new_comp} {}

    string to_string() const {
        return value_to_string((Value_c*)&(value.c), value.t) + ' ' + compare_str[comp];
    }
}Condition;


bool compare_values(Condition condition, Node* node);

typedef struct Iter {
    Node* node;
    Condition condition;
    bool is_valid = true;

    // Iter() : is_valid{false}, condition{.value=default_condition, .comp=NO_CONDITION} {}

    Iter(Condition cond) : condition{move(cond)} {
        this->node = read_first_node();
        if (compare_values(condition, node)) this->is_valid = true;
        else this->is_valid = next();
    }

    bool next();
    Node* operator*();
    void print();
    void free();
}Iter;


void close();
void open(const char* filename);
Node* add_node(Value v, Node* parent);
void delete_node(Node* parent);
void edit_node(Value v, Node* node);
Iter find_nodes(Condition condition);