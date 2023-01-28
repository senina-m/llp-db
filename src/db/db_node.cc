#include "db_node.h"
FILE* ptr;
size_t end_of_last_node_offset;
File_header* header = NULL;

void open_file_bd(const char* fn){
    ptr = open_file(fn);
    size_t size = get_file_len(fn);
    if(size <= 1){//создать голову и первый блок и хедер и записать их в файл
        header = mmalloc(File_header);

        header->tree_head_offset = FILE_HEADER_SIZE;
        header->free_space_offset = FILE_HEADER_SIZE + BOOL_NODE_SIZE;
        header->end_of_last_node_offset = FILE_HEADER_SIZE + BOOL_NODE_SIZE + DEFAULT_BLOCK_SIZE;
        void write_header(File_header* header, FILE* ptr);
        write_header(header, ptr);
        write_block(0, 0, DEFAULT_BLOCK_SIZE, ptr, header->free_space_offset);

        Value_c c4 = {.bool_value=true};
        Node* head = mmalloc(Node);
        head->offset = FILE_HEADER_SIZE;
        head->node_len = BOOL_NODE_SIZE;
        head->first_child = 0;
        head->parent = 0;
        head->prev_sibling = 0;
        head->next_sibling = 0;
        head->value_len = 1;
        head->value_t = 3;
        head->value_c = c4;
        write_node(head, ptr);
        free_node(head);
    }else{
        header = read_header(ptr);
    }
}

void close_file_bd(){
    write_header(header, ptr);
    close_file(ptr);
    free(header);
    header = NULL;
}

Node* read_first_node(){
    return read_node(header->tree_head_offset, ptr);
}

Node* read_offset_node(size_t offset){
    return read_node(offset, ptr);
}

void replace_node_db(size_t new_offset, Node* node){
    replace_node(new_offset, node, ptr);
}

bool check_not_head(size_t offset){
    if (header == NULL) return false;
    if (offset == header->tree_head_offset) return false;
    return true;
}

size_t static get_last_child(Node* parent){
    size_t cur = parent->first_child;
    size_t next = read_next_sibling_offset(cur, ptr);
    while(next != 0){
        cur = next;
        next = read_next_sibling_offset(cur, ptr);
    }
    return cur;
}

size_t get_node_real_len(Node* node){
    return NODE_HEDER_SIZE + NODE_VALUE_META_SIZE + 
    get_value_size(&(node->value_c), get_t_by_int(node->value_t));
}

size_t static get_node_value_space(Node* node){
    return node->node_len - (NODE_HEDER_SIZE + NODE_VALUE_META_SIZE);
}

size_t static get_new_node_len(Node* node, size_t new_value_size){
    return new_value_size + NODE_HEDER_SIZE + NODE_VALUE_META_SIZE;
}

void static move_node(Node* node){
    //todo: удалить ноду с того места где она сейчас
    // записываем вместо ноды пустой блок

    node->offset = find_free_space(header->free_space_offset, node->node_len, ptr);
    if(node->offset == 0){//дырка не нашлась
        node->offset = header->end_of_last_node_offset; //записываем значит ноду в самый конец
        header->end_of_last_node_offset += node->node_len; //конец сдвигаем на длинну ноды
    }else{//дырка нашлась
        node->node_len = read_block_size(ptr, node->offset); // записываем ноду вместо всей дырки
        delete_block(node->offset, &(header->free_space_offset), ptr); // удаляем дырку
        print_free_space(header->free_space_offset, ptr);
    }
}

void static uppdate_children_by_parent_offset(Node* parent){
    size_t cur = parent->first_child;
    size_t next = read_next_sibling_offset(cur, ptr);
    change_parent_offset(cur, parent->offset, ptr);
    while(next != 0){
        cur = next;
        next = read_next_sibling_offset(cur, ptr);
        change_parent_offset(cur, parent->offset, ptr);
    }
}

void delete_children(size_t node);

void delete_children(size_t node){
    size_t first_child = read_first_child_offset(node, ptr);
    if(first_child != 0){ //если есть ребёнок
        delete_children(first_child);
    }

    size_t next_sibling = read_next_sibling_offset(node, ptr);
    if(next_sibling != 0){ //если есть братья
        delete_children(next_sibling);
    }
    //чистим эту ноду -- записываем вместо ноды пустой блок
    create_block(&(header->free_space_offset), node, read_node_len(node, ptr), ptr);
}

Node* add_node_db(Node* parent, Value* value){
    update_node(parent, ptr);

    Node* node = mmalloc(Node);

    fill_nodes_value_meta(node, value); //fill value_len, value_t and value_c
    node->node_len = node->value_len + NODE_HEDER_SIZE + NODE_VALUE_META_SIZE;
    move_node(node);

    node->first_child = 0;
    node->parent = parent->offset;
    node->next_sibling = 0;

    if(parent->first_child == 0){
        change_first_child_offset(node->parent, node->offset, ptr);
        node->prev_sibling = 0;
    }else{
        node->prev_sibling = get_last_child(parent);
        change_next_sibling_offset(node->prev_sibling, node->offset, ptr);
    }

    write_node(node, ptr);
    return node;
}


void edit_node_db(Node* node, Value* value){
    update_node(node, ptr);

    size_t new_value_size = get_value_size(&(value->c), value->t);
    size_t node_value_space = get_node_value_space(node);

    fill_nodes_value_meta(node, value); //обновляет только value_len value_t value_c

    if (node_value_space >= new_value_size){//места хватает => перезаписываем эту ноду
        //Теперь, если у этой ноды длина была больше новой, то её node_len не соответствует истинной.
        write_node_value(node, ptr);
    }else{//места не хватает нужно двигать
        //Мы уже обновили зачение value в ноде, поэтому: 
        create_block(&(header->free_space_offset), node->offset, node->node_len, ptr);

        node->node_len = get_node_real_len(node);//чтобы move_node искал правильный размер для дырки
        move_node(node); //обновляет node_len
        cout << endl;
        uppdate_children_by_parent_offset(node);//нужно обновить в детях оффсет родителя, если у ноды есть дети.
        
        if(node->prev_sibling == 0){//если это первый ребёнок этого родителя, родителя нужно уведомить, куда он переехал
                change_first_child_offset(node->parent, node->offset, ptr);
            if(node->next_sibling != 0){//нода НЕ единственный ребёнок -- уведомляем ещё и брата
                change_prev_sibling_offset(node->next_sibling, node->offset, ptr);//следующиего ребёнка тоже надо уведомить
            }
        }else if(node->next_sibling != 0){//если ребёнок был в середине, то уведомить нужно только ближайших братьев
            change_prev_sibling_offset(node->next_sibling, node->offset, ptr);
            change_next_sibling_offset(node->prev_sibling, node->offset, ptr);
        }else{//если ребёнок последний -- обновляем только предпоследнего
            change_next_sibling_offset(node->prev_sibling, node->offset, ptr);
        }
        write_node(node, ptr);
    }
}


//todo: при вызове метода проверять, что мы не пытаемся удалить вершину дерева
void delete_node_db(Node* node){
    update_node(node, ptr);

    if(node->parent == 0) return; //нельзя удалить head -- это мнимая нода

    // удалить её списка братьев, удалить из родителя, если это первый ребёнок
    if(node->prev_sibling == 0){//эта нода первый ребёнок
        if(node->next_sibling == 0){//нода единственный ребёнок -- уведомляем родителя что он теперь бездетный
            change_first_child_offset(node->parent, 0, ptr);
        }else{
            //в родителя пишем второго ребёнка, как первого
            change_first_child_offset(node->parent, node->next_sibling, ptr);
            // во второго пишем prev=0
            change_prev_sibling_offset(node->next_sibling, 0, ptr);
        }
    }else if(node->next_sibling != 0){//если ребёнок в серединие удаляем его из списка братьев
        change_prev_sibling_offset(node->next_sibling, node->prev_sibling, ptr);
        change_next_sibling_offset(node->prev_sibling, node->next_sibling, ptr);
    }else{ //если ребёнок последний, делаем последним предпоследнего
        change_prev_sibling_offset(node->prev_sibling, 0, ptr);
    }

    if(node->first_child != 0){ // если вообще есть дети, которых нужно удалять -- удаляем
        delete_children(node->first_child);
    }

    create_block(&(header->free_space_offset), node->offset, node->node_len, ptr);
}