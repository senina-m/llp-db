#include "node_io.h"

size_t write_value_to_file(Value_c* c, Value_t t, size_t offset, FILE* ptr){
    size_t size = sizeof(Value_c);
    switch (t) {
    case int_type:
        offset = write_buffer_to_file(offset, ptr, &(c->int_value), 1, size);
        break;
    case double_type:
        offset = write_buffer_to_file(offset, ptr, &(c->double_value), 1, size);
        break;
    case string_type:
        {offset = write_buffer_to_file(offset, ptr, c->string_value, get_value_size(c, t), sizeof(char));
        break;}
    case bool_type:
        offset = write_buffer_to_file(offset, ptr, &(c->bool_value), 1, size);
        break;
    default:
        cout << "Wrong type " << t << endl;
        exit(-1); 
    }
    return offset;
}

void write_node(Node* node, FILE* ptr){
    size_t offset = node->offset;
    //write header
    offset = write_buffer_to_file(offset, ptr, &(node->node_len), 1, sizeof(size_t));
    offset = write_buffer_to_file(offset, ptr, &(node->first_child), 1, sizeof(size_t));
    offset = write_buffer_to_file(offset, ptr, &(node->parent), 1, sizeof(size_t));
    offset = write_buffer_to_file(offset, ptr, &(node->prev_sibling), 1, sizeof(size_t));
    offset = write_buffer_to_file(offset, ptr, &(node->next_sibling), 1, sizeof(size_t));

    // cout << "write v:" << value_to_string(&(node->value_c), get_t_by_int(node->value_t));
    // cout << ", t: " << (int)(node->value_t) << ", offset = " << *offset << endl;
    offset = write_buffer_to_file(offset, ptr, &(node->value_len), 1, sizeof(size_t));
    offset = write_buffer_to_file(offset, ptr, &(node->value_t), 1, sizeof(int8_t));
    offset = write_value_to_file(&(node->value_c), get_t_by_int(node->value_t), offset, ptr);
}

size_t read_value_from_file (Value_c* c, int8_t type, size_t size, size_t offset, FILE* ptr){
    switch (type) {
    case int_type:
        offset = read_buffer_from_file(offset, ptr, &(c->int_value), 1, sizeof(Value_c));
        break;
    case double_type:
        offset = read_buffer_from_file(offset, ptr, &(c->double_value), 1, sizeof(Value_c));
        break;
    case string_type:
        {c->string_value = mmalloc_array(char, size);
        offset = read_buffer_from_file(offset, ptr, (void*) c->string_value, size, sizeof(char));
        break;}
    case bool_type:
        offset = read_buffer_from_file(offset, ptr, &(c->bool_value), 1, sizeof(Value_c));
        break;
    default:
        cout << "Wrong type " << type << endl;
        exit(-1); 
    }

    return offset;
}


Node* read_node_header(size_t offset, FILE* ptr){
    // cout << "reading from offset =" << *offset;
    Node* node = mmalloc(Node);
    node->offset = offset;
    offset = read_buffer_from_file(offset, ptr, &(node->node_len), 1, sizeof(size_t));
    offset = read_buffer_from_file(offset, ptr, &(node->first_child), 1, sizeof(size_t));
    offset = read_buffer_from_file(offset, ptr, &(node->parent), 1, sizeof(size_t));
    offset = read_buffer_from_file(offset, ptr, &(node->prev_sibling), 1, sizeof(size_t));
    offset = read_buffer_from_file(offset, ptr, &(node->next_sibling), 1, sizeof(size_t));
    node->value_t = -1;
    return node;
}

Node* read_node(size_t offset, FILE* ptr){
    // cout << "reading from offset =" << *offset;

    Node* node = read_node_header(offset, ptr); 
    offset += NODE_HEDER_SIZE;
    offset = read_buffer_from_file(offset, ptr, &(node->value_len), 1, sizeof(size_t));
    offset = read_buffer_from_file(offset, ptr, &(node->value_t), 1, sizeof(int8_t));
    offset = read_value_from_file(&(node->value_c), node->value_t, node->value_len, offset, ptr);
    // cout <<  " v:" << value_to_string(&(node->value_c), get_t_by_int(node->value_t)) 
    // << ", t: " << (int)(node->value_t) << "\n";
    return node;
}

size_t read_next_sibling_offset(size_t cur_offset, FILE* ptr){
    size_t next = 0;
    read_buffer_from_file(cur_offset + NODE_NEXT_SIBLING_OFFSET, ptr, &next, 1, sizeof(size_t));
    return next;
}

void change_next_sibling_offset(size_t node_offset, size_t next_sibling_offset, FILE* ptr){
    write_buffer_to_file(node_offset + NODE_NEXT_SIBLING_OFFSET, ptr, &next_sibling_offset, 1, sizeof(size_t));
}

void change_first_child_offset(size_t node_offset, size_t first_child_offset, FILE* ptr){
    write_buffer_to_file(node_offset + NODE_FIRST_CHILD_OFFSET, ptr, &first_child_offset, 1, sizeof(size_t));
}

void change_parent_offset(size_t cur_offset, size_t parent_offset, FILE* ptr){
    write_buffer_to_file(cur_offset + NODE_PARENT_OFFSET, ptr, &parent_offset, 1, sizeof(size_t));
}


void write_node_value(Node* node, FILE* ptr){
    size_t offset = node->offset + NODE_HEDER_SIZE;
    offset = write_buffer_to_file(offset, ptr, &node->value_len, 1, sizeof(size_t));
    offset = write_buffer_to_file(offset, ptr, &node->value_t, 1, sizeof(int8_t));
    offset = write_value_to_file(&(node->value_c), get_t_by_int(node->value_t), offset, ptr);
}
