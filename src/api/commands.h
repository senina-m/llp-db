#include "../db/db_node.h"

enum Compare{
    EQUAL,
    NOT_EQUAL,
    NO_CONDITION
};

typedef struct Condition{
    Value* value;
    Compare comp;
} Condition;


bool compare_values(Condition* condition, Value_c* c, Value_t t);



struct Iter {
    Node* node;
    Condition condition;
    bool is_valid = true;

    Iter() : is_valid{false}, condition{.value=NULL, .comp=NO_CONDITION} {}

    Iter(Condition cond) : condition{std::move(cond)} {
        node = read_first_node();
        if (compare_values(condition, node->value_c, node->value_t)) {
            is_valid = true;
        } else {
            is_valid = next();
        }
    }

    bool next();
    Node* operator*();
    void print();
} Iter;

void close_file();
void open_file(const char* filename);
bool compare_values(Condition* condition, Value_c* c, Value_t t);
bool add_node(Value v, Node* parent);
bool delete_node(Node* parent);
bool edit_node(Value v, Node* node);
Iter find_nodes(Condition* condition, Node* start, vector<Node*> list);