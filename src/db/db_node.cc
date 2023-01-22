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

void static move_node(Node* node, size_t* free_space_offset, size_t* end_of_nodes, FILE* ptr){
    //todo: удалить ноду с того места где она сейчас
    // записываем вместо ноды пустой блок

    node->offset = find_free_space(*free_space_offset, node->node_len, ptr);

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

void delete_children(size_t node, size_t* free_space_offset, FILE* ptr);

void delete_children(size_t node, size_t* free_space_offset ,FILE* ptr){
    size_t first_child = read_first_child_offset(node, ptr);
    if(first_child != 0){ //если есть ребёнок
        delete_children(first_child, free_space_offset, ptr);
    }

    size_t next_sibling = read_next_sibling_offset(node, ptr);
    if(next_sibling != 0){ //если есть братья
        delete_children(next_sibling, free_space_offset, ptr);
    }
    //чистим эту ноду -- записываем вместо ноды пустой блок
    create_block(free_space_offset, node, read_node_len(node, ptr), ptr);
}

Node* add_node_db(Node* parent, Value* value, size_t* free_space_offset, size_t* end_of_nodes, FILE* ptr){
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


Node* edit_node_db(Node* node, Value* value, size_t* free_space_offset, size_t* end_of_nodes, FILE* ptr){
    size_t new_value_size = get_value_size(&(value->c), value->t);
    size_t node_value_space = get_node_value_space(node);

    fill_nodes_value_meta(node, value); //обновляет только value_len value_t value_c

    if (node_value_space >= new_value_size){//места хватает => перезаписываем эту ноду
        //Теперь, если у этой ноды длина была больше новой, то её node_len не соответствует истинной.
        write_node_value(node, ptr);
    }else{//места не хватает нужно двигать
        //Мы уже обновили зачение value в ноде, поэтому: 
        create_block(free_space_offset, node->offset, node->node_len, ptr);

        node->node_len = get_node_real_len(node);//чтобы move_node искал правильный размер для дырки
        move_node(node, free_space_offset, end_of_nodes, ptr); //обновляет node_len
        uppdate_children_by_parent_offset(node, ptr);//нужно обновить в детях оффсет родителя, если у ноды есть дети.
        
        if(node->prev_sibling == 0){//если это первый ребёнок этого родителя, родителя нужно уведомить, куда он переехал
            change_first_child_offset(node->parent, node->offset, ptr);
            change_prev_sibling_offset(node->next_sibling, node->offset, ptr);//следующиего ребёнка тоже надо уведомить
        }else if(node->next_sibling != 0){//если ребёнок был в середине, то уведомить нужно только ближайших братьев
            change_prev_sibling_offset(node->next_sibling, node->offset, ptr);
            change_next_sibling_offset(node->prev_sibling, node->offset, ptr);
        }else{//если ребёнок последний -- обновляем только предпоследнего
            change_next_sibling_offset(node->prev_sibling, node->offset, ptr);
        }
        write_node(node, ptr);
    }
    return node;
}


//todo: при вызове метода проверять, что мы не пытаемся удалить вершину дерева
void delete_node_db(Node* node, size_t* free_space_offset, FILE* ptr){
    // удалить её списка братьев, удалить из родителя, если это первый ребёнок
    if(node->prev_sibling == 0){//эта нода первый ребёнок
        //в родителя пишем второго ребёнка, как первого
        change_first_child_offset(node->parent, node->next_sibling, ptr);
        // во второго пишем prev=0
        change_prev_sibling_offset(node->next_sibling, 0, ptr);
    }else if(node->next_sibling != 0){//если ребёнок в серединие удаляем его из списка братьев
        change_prev_sibling_offset(node->next_sibling, node->prev_sibling, ptr);
        change_next_sibling_offset(node->prev_sibling, node->next_sibling, ptr);
    }else{ //если ребёнок последний, делаем последним предпоследнего
        change_prev_sibling_offset(node->prev_sibling, 0, ptr);
    }

    if(node->first_child != 0){ // если вообще есть дети, которых нужно удалять -- удаляем
        delete_children(node->first_child, free_space_offset, ptr);
    }

    create_block(free_space_offset, node->offset, node->node_len, ptr);
}