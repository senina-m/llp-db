#include "../io/node_io.h"
#include "../db/sturcts.h"

enum Compare{
    EQUAL,
    NOT_EQUAL
};

typedef struct Condition{
    Value* value;
    Compare comp;
} Condition;

void close_file();
void open_file(const char* filename);

bool add_node(Value v, Node* parent);
bool delete_node(Node* parent);
bool edit_node(Value v, Node* node);
bool find_nodes(Condition* condition, Node* start, vector<Node*> list);
