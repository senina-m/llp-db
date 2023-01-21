#include "db_node.h"


size_t static get_last_child(Node* parent, FILE* ptr){
    size_t cur = parent->first_child;
    size_t next = read_next_sibling_offset(cur, ptr);
    while(next != 0){
        cur = next;
        next = read_next_sibling_offset(cur, ptr);
    }
    return cur;
}

size_t static get_node_real_len(Node* node){
    return NODE_HEDER_SIZE + NODE_VALUE_META_SIZE + 
    get_value_size(&(node->value_c), get_t_by_int(node->value_t));
}

size_t static get_node_value_space(Node* node){
    return node->node_len - (NODE_HEDER_SIZE + NODE_VALUE_META_SIZE);
}

size_t static get_new_node_len(Node* node, size_t new_value_size){
    return new_value_size + NODE_HEDER_SIZE + NODE_VALUE_META_SIZE;
}

void static move_node(Node* node, size_t free_space_offset, size_t* end_of_nodes, FILE* ptr){
    node->offset = find_free_space(free_space_offset, node->node_len, ptr);

    if(node->offset == 0){//дырка не нашлась
        node->offset = *end_of_nodes; //записываем значит ноду в самый конец
        *end_of_nodes += node->node_len; //конец сдвигаем на длинну ноды
    }else{//дырка нашлась
        node->node_len = read_block_size(ptr, node->offset); // записываем ноду вместо всей дырки
        delete_block(node->offset, ptr); // удаляем дырку
    }
}

void static uppdate_children_by_parent_offset(Node* parent, FILE* ptr){
    size_t cur = parent->first_child;
    size_t next = read_next_sibling_offset(cur, ptr);
    change_parent_offset(cur, parent->offset, ptr);
    while(next != 0){
        cur = next;
        next = read_next_sibling_offset(cur, ptr);
        change_parent_offset(cur, parent->offset, ptr);
    }
}

Node* add_node_db(Node* parent, Value* value, size_t free_space_offset, size_t* end_of_nodes, FILE* ptr){
    Node* node = mmalloc(Node);

    fill_nodes_value_meta(node, value); //fill value_len, value_t and value_c
    node->node_len = node->value_len + NODE_HEDER_SIZE + NODE_VALUE_META_SIZE;
    
    move_node(node, free_space_offset, end_of_nodes, ptr);

    node->first_child = 0;
    node->parent = parent->offset;
    node->next_sibling = 0;

    if(parent->first_child == 0){
        change_first_child_offset(node->parent, node->offset, ptr);
        node->prev_sibling = 0;
    }else{
        node->prev_sibling = get_last_child(parent, ptr);
        change_next_sibling_offset(node->prev_sibling, node->offset, ptr);
    }

    write_node(node, ptr);
    return node;
}


Node* edit_node_db(Node* node, Value* value, size_t free_space_offset, size_t* end_of_nodes, FILE* ptr){
    size_t new_value_size = get_value_size(&(value->c), value->t);
    size_t node_value_space = get_node_value_space(node);

    fill_nodes_value_meta(node, value); //обновляет только value_len value_t value_c

    if (node_value_space >= new_value_size){//места хватает => перезаписываем эту ноду
        //Теперь, если у этой ноды длина была больше новой, то её node_len не соответствует истинной.
        write_node_value(node, ptr);
    }else{//места не хватает нужно двигать
        //Мы уже обновили зачение value в ноде, поэтому: 
        node->node_len = get_node_real_len(node);//чтобы move_node искал правильный размер для дырки
        move_node(node, free_space_offset, end_of_nodes, ptr); //обновляет node_len
        uppdate_children_by_parent_offset(node, ptr);//нужно обновить в детях оффсет родителя, если у ноды есть дети.
        write_node(node, ptr);
    }
    return node;
}