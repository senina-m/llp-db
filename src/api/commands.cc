#include "commands.h"

bool compare_values(Condition condition, Node* node){
    switch (condition.comp){
    case EQUAL:
        return equal_values(&(condition.value), &(node->value_c), get_t_by_int(node->value_t));
        break;
    case NOT_EQUAL:
        return !equal_values(&(condition.value), &(node->value_c), get_t_by_int(node->value_t));
        break;
    case NO_CONDITION:
        return true;
        break;
    default:
        cout << "invalid condition type!";
        exit(-1);
    }
}

void close(){
    close_file_db();
}

void open(const char* filename){
    open_file_db(filename);
}

Node* add_node(Value v, Node* parent){
    Node* node = add_node_db(parent, &v);
    return node;
}

void delete_node(Node* node){
    delete_node_db(node);
}

void edit_node(Value v, Node* node){
    edit_node_db(node, &v);
}

Node* read_first(){
    return read_first_node();
}

Iter find_nodes(Condition condition){
    Iter res = Iter(condition);
    return res;
}

Node* Iter::operator*() {
    return node;
}

void print(Node* node){
    print_node(node);
}

void f_node(Node* node){
    free_node(node);
}

void static looking_at(Node* node){
    cout << "looking at node => ";
    print_node(node);
    cout << "\n";
}

bool Iter::next() {
    if (!is_valid) {
        return false;
    }
    //если писать рекурсией, то локальные переменные будут оставаться на стеке
    //поэтому лучше цикл


    do {
        if (node->first_child != 0) { //если дети есть -- пусть и отдуваются
            replace_node_db(node->first_child, node);
        } else if (node->next_sibling != 0) { //если есть братья то тоже они.. 
            replace_node_db(node->next_sibling, node);
        } else { //нет ни братьев ни детей
                // а эта нода нам уже не подошла, раз мы её смотрим
            if (!check_not_head(node->parent)) { //если родитель голова -- идти больше некуда, значит мы ничего не нашли
                is_valid = false;
                return false;
            }
            //снизу ничего нет
            replace_node_db(node->parent, node);//значит поднимаемся в родителя
            //и смотрим, что у него
        

            //если у родителя есть братья, проверяем их
            while (node->next_sibling == 0) {
                if (!check_not_head(node->parent)) {
                    is_valid = false;
                    return false;
                }
                replace_node_db(node->parent, node);
            }

            //если братьев нет проверяем родителя родителя
            if (node->next_sibling != 0) {
                replace_node_db(node->next_sibling, node);
            } else { // мы уперлись в head
                is_valid = false;
                return false;
            }
        }
    } while (!compare_values(condition, node));

    return true;
}

// To debug
void Iter::print() {
    printf("--- Result Iterator | Condition: %s ---\n", this->condition.to_string().c_str());

    if (!this->is_valid) {
        printf("Iterator is not valid\n");
        printf("\n");
        return;
    }
    
    do {
        print_node(node);
        cout << "\n";
    } while (this->next());
    printf("\n");
}


void Iter::free(){
    free_node(node);
}
