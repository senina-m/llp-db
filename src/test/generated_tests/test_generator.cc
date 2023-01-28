#include "test_generator.h"
#define MIN 0
#define MAX 100
#define LEN 16


void random_insert_test(size_t count) {
    open_file_db("test");
    size_t nodes_counter = 1;    
    size_t* nodes = new size_t[count + 1];
    nodes[0] = get_tree_offset();
    Node* parent = mmalloc(Node);
    Value* val = mmalloc(Value);

    printf("elem_count,time\n");
    for (int i = 0; i < count; ++i) {
        generate_random_value(val, MIN, MAX, LEN);
        int j = get_random_int(0, nodes_counter - 1);
        size_t parrent_offset = nodes[i];
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