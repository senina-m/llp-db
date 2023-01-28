#include "test_generator.h"
#define MIN 0
#define MAX 100
#define LEN 16

void create_tree(size_t* nodes, size_t count){
    nodes[0] = get_tree_offset();
    Node* parent = mmalloc(Node);
    Value* val = mmalloc(Value);

    for (int i = 0; i < count; ++i) {
        generate_random_value(val, MIN, MAX, LEN);
        int j = get_random_int(0, i);
        size_t parrent_offset = nodes[j];
        replace_node_db(parrent_offset, parent);
        Node* added_node = add_node_db(parent, val);
        nodes[i + 1] = added_node->offset;
        free_node(added_node);
    }

    free_node(parent);
    free(val);
}


void random_insert_test(size_t count) {
    clean_file("test");
    open_file_db("test");
    size_t nodes_counter = 1;    
    size_t* nodes = new size_t[count + 1];
    nodes[0] = get_tree_offset();
    Node* parent = mmalloc(Node);
    Value* val = mmalloc(Value);

    // printf("elem_count,time\n");
    for (int i = 0; i < count; ++i) {
        generate_random_value(val, MIN, MAX, LEN);
        int j = get_random_int(0, nodes_counter - 1);
        size_t parrent_offset = nodes[j];
        replace_node_db(parrent_offset, parent);

        unsigned int start_time =  clock(); // начальное время
        Node* added_node = add_node_db(parent, val);
        unsigned int end_time = clock(); // конечное время
        nodes[nodes_counter] = added_node->offset;
        free_node(added_node);
        unsigned int search_time = end_time - start_time;
        printf("%d,%f\n", i, (double) search_time / CLOCKS_PER_SEC);
        ++nodes_counter;
    }
    close_file_db();
    delete [] nodes;
    free_node(parent);
    free(val);
}

void random_edit_test(size_t count) {
    clean_file("test");
    open_file_db("test");
    size_t nodes_counter = 1;    
    size_t* nodes = new size_t[count + 1];
    nodes[0] = get_tree_offset();
    Node* parent = mmalloc(Node);
    Node* edited_node = mmalloc(Node);
    Value* val = mmalloc(Value);
    Value* new_val = mmalloc(Value);

    // printf("elem_count,time\n");
    for (int i = 0; i < count; ++i) {
        generate_random_value(val, MIN, MAX, LEN);
        int j = get_random_int(0, nodes_counter - 1);

        size_t parrent_offset = nodes[j];
        replace_node_db(parrent_offset, parent);
        Node* added_node = add_node_db(parent, val);
        nodes[nodes_counter] = added_node->offset;
        free_node(added_node);
        
        ++nodes_counter;

        generate_random_value(new_val, MIN, MAX, LEN);
        size_t edit_offset = nodes[get_random_int(0, nodes_counter - 1)];
        replace_node_db(edit_offset, edited_node);

        unsigned int start_time =  clock(); // начальное время
        edit_node_db(edited_node, new_val);
        unsigned int end_time = clock(); // конечное время
        unsigned int search_time = end_time - start_time;
        printf("%d,%f\n", i, (double) search_time / CLOCKS_PER_SEC);
    }

    close_file_db();
    delete [] nodes;
    free_node(parent);
    free_node(edited_node);
    free(val);
    free(new_val);
}

void random_find_test(const Condition& condition, size_t count) {
    clean_file("test");
    open_file_db("test");
    size_t nodes_counter = 1;    
    size_t* nodes = new size_t[count + 1];
    nodes[0] = get_tree_offset();
    Node* parent = mmalloc(Node);
    Value* val = mmalloc(Value);

    // printf("elem_count,time\n");
    for (int i = 0; i < count; i+=5) {
        generate_random_value(val, MIN, MAX, LEN);
        int j = get_random_int(0, nodes_counter - 1);

        size_t parrent_offset = nodes[j];
        replace_node_db(parrent_offset, parent);
        Node* added_node = add_node_db(parent, val);
        nodes[nodes_counter] = added_node->offset;
        free_node(added_node);
        ++nodes_counter;

        unsigned int start_time = clock(); // начальное время
        Iter it = find_nodes(condition);
        while (it.next()); // итерирумся по всем элементам
        unsigned int end_time = clock(); // конечное время
        it.free();
        unsigned int search_time = end_time - start_time;
        printf("%d,%f\n", i, (double) search_time / CLOCKS_PER_SEC);

    }
    close_file_db();
    delete [] nodes;
    free_node(parent);
    free(val);
}

void random_delete_test(size_t count) {
    Node* node = mmalloc(Node);
    Value* val = mmalloc(Value);

    for(int cur_nodes_num = 1; cur_nodes_num <= count; cur_nodes_num++){
        clean_file("test");
        open_file_db("test");
        size_t* nodes = new size_t[cur_nodes_num + 1];
        create_tree(nodes, cur_nodes_num);
        int j = get_random_int(0, cur_nodes_num - 1);
        size_t offset = nodes[j];
        replace_node_db(offset, node);
        delete [] nodes;

        unsigned int start_time =  clock(); // начальное время
        delete_node_db(node);
        unsigned int end_time = clock(); // конечное время
        unsigned int search_time = end_time - start_time;
        printf("%d,%f\n", cur_nodes_num, (double) search_time / CLOCKS_PER_SEC);
        close_file_db();
    }
    free_node(node);
    free(val);
}


void random_file_size_test(size_t count) {

    // printf("elem_count,file_size\n");
    for (int i = 0; i < count; ++i) {
        clean_file("test");
        open_file_db("test");
        size_t* nodes = new size_t[i + 1];
        create_tree(nodes, i);
        delete [] nodes;
        printf("%d,%zu\n", i, get_file_len("test"));
    }
}