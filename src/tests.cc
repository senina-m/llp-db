#include "tests.h"

void test1(){

    cout << "\n----------------TEST 1----------------\n";
    Value_c c = {.string_value="abc"};
    Node node = {.offset=5, .node_len=15, .first_child=7, .parent=4, .prev_sibling=4,
    .next_sibling=13, .value_len=4 , .value_t=2, .value_c=c};

    FILE* ptr = open_file("test");
    write_node(&node, ptr);
    Node* new_node = read_node(5, ptr);
    cout << "printing of wtiten to file and read from it node:\n";
    print_node(&node);
    cout << "\n";
    print_node(new_node);
    free_node(new_node);

    change_first_child_offset(5, 1234, ptr);
    change_next_sibling_offset(5, 98097, ptr);
    change_parent_offset(5, 21424, ptr);


    cout << "\nread node header:\n";
    Node* header = read_node_header(5, ptr);
    print_node_header(header);
    free(header);

    close_file(ptr);
    cout << "\n----------------TEST 1----------------\n";
}

void test2(){
    cout << "\n----------------TEST 2----------------\n";

    File_header file_header = {.tree_head_offset=58, .free_space_offset=16};

    Value_c c1 = {.bool_value=false};
    Node head = {
        .offset=66,
        .node_len=50,
        .first_child=116,
        .parent=0,
        .prev_sibling=0,
        .next_sibling=0,
        .value_len=1,
        .value_t=3,
        .value_c=c1};

    const char* str = mmalloc_array(char, 15);  //14+\0
    snprintf((char*) str, 15, "Hello world!!!");
    Value_c c2 = {.string_value=str};
    Node node2 = {
        .offset=116,
        .node_len=64,
        .first_child=180,
        .parent=66,
        .prev_sibling=0,
        .next_sibling=237,
        .value_len=15,
        .value_t=2,
        .value_c=c2};

    Value_c c3 = {.double_value=0.5};
    Node node3 = {
        .offset=180,
        .node_len=57,
        .first_child=0,
        .parent=116,
        .prev_sibling=0,
        .next_sibling=0,
        .value_len=8,
        .value_t=1,
        .value_c=c3};

    Value_c c4 = {.bool_value=true};
    Node node4 = {
        .offset=237,
        .node_len=50,
        .first_child=0,
        .parent=66,
        .prev_sibling=116,
        .next_sibling=0,
        .value_len=1,
        .value_t=3,
        .value_c=c4};

    Block block1 = {.next=287, .prev=0, .size=30}; 
    Block block2 = {.next=0, .prev=16, .size=55}; //from 263 to 318
    size_t end_of_nodes = 324;

    FILE* ptr = open_file("test");
    write_block(&block1, ptr, 16);
    write_block(&block2, ptr, 287);
    write_node(&head, ptr);
    write_node(&node2, ptr);
    write_node(&node3, ptr);
    write_node(&node4, ptr);

    Value_c c5 = {.bool_value=true};
    Value v = {.c=c5, .t=bool_type};
    Node* added_node = add_node_db(&node2, &v, file_header.free_space_offset, &end_of_nodes, ptr);
    cout << "\nadded node: \n";
    print_node(added_node);
    cout << endl;
    free_node(added_node);

    Node* new_node5 = read_node(16, ptr);
    Node* new_head = read_node(66, ptr);
    Node* new_node2 = read_node(116, ptr);
    Node* new_node3 = read_node(180, ptr);
    Node* new_node4 = read_node(237, ptr);
    Block* new_block1 = read_block(ptr, 16);


    print_block(new_block1);
    cout << "\n";

    print_node(new_node5);
    cout << "\n";

    print_node(new_head);
    cout << "\n";
    
    print_node(new_node2);
    cout << "\n";
    
    print_node(new_node3);
    cout << "\n";

    print_node(new_node4);
    cout << "\n";

    close_file(ptr);

    free_block(new_block1);
    free_node(new_head);
    free_node(new_node2);
    free_node(new_node3);
    free_node(new_node4);
    free_node(new_node5);
    free((char*) str);
    cout << "\n----------------TEST 2----------------\n";
}

void test3(){
    cout << "\n----------------TEST 3----------------\n";

    File_header file_header = {.tree_head_offset=58, .free_space_offset=16};

    Value_c c1 = {.bool_value=false};
    Node head = {
        .offset=66,
        .node_len=50,
        .first_child=116,
        .parent=0,
        .prev_sibling=0,
        .next_sibling=0,
        .value_len=1,
        .value_t=3,
        .value_c=c1};

    const char* str = mmalloc_array(char, 15);  //14+\0
    snprintf((char*) str, 15, "Hello world!!!");
    Value_c c2 = {.string_value=str};
    Node node2 = {
        .offset=116,
        .node_len=64,
        .first_child=180,
        .parent=66,
        .prev_sibling=0,
        .next_sibling=237,
        .value_len=15,
        .value_t=2,
        .value_c=c2};

    Value_c c3 = {.double_value=0.5};
    Node node3 = {
        .offset=180,
        .node_len=57,
        .first_child=0,
        .parent=116,
        .prev_sibling=0,
        .next_sibling=0,
        .value_len=8,
        .value_t=1,
        .value_c=c3};

    Value_c c4 = {.bool_value=true};
    Node node4 = {
        .offset=237,
        .node_len=50,
        .first_child=0,
        .parent=66,
        .prev_sibling=116,
        .next_sibling=0,
        .value_len=1,
        .value_t=3,
        .value_c=c4};

    Block block1 = {.next=287, .prev=0, .size=30}; 
    Block block2 = {.next=0, .prev=16, .size=55}; //from 263 to 318
    size_t end_of_nodes = 324;

    FILE* ptr = open_file("test");
    write_block(&block1, ptr, 16);
    write_block(&block2, ptr, 287);
    
    write_node(&head, ptr);
    write_node(&node2, ptr);
    write_node(&node3, ptr);
    write_node(&node4, ptr);

    // Value_c c5 = {.bool_value=true};
    // Value v = {.c=c5, .t=bool_type};
    const char* s = mmalloc_array(char, 20);  //14+\0
    snprintf((char*) s, 20, "Hello world, dear:)");
    Value_c c5 = {.string_value=s};
    Value v = {.c=c5, .t=string_type};

    Node* edited_node = edit_node_db(&node2, &v, file_header.free_space_offset, &end_of_nodes, ptr);
    cout << "\nedited_node: \n";
    print_node(edited_node);
    cout << endl;

    Node* new_head = read_node(66, ptr);
    Node* new_node2 = read_node(116, ptr);
    Node* new_node3 = read_node(180, ptr);
    Node* new_node4 = read_node(237, ptr);
    Node* new_node5 = read_node(16, ptr);
    Node* new_node6 = read_node(324, ptr);
    Block* new_block1 = read_block(ptr, 16);
    Block* new_block2 = read_block(ptr, 287);
    Block* new_block3 = read_block(ptr, 116);


    cout << "blocks:\n";
    print_block(new_block1);
    cout << "\n";

    print_block(new_block2);
    cout << "\n";

    print_block(new_block3);
    cout << "\n";

    cout << "nodes:\n";
    print_node(new_head);
    cout << "\n";
    
    print_node(new_node2);
    cout << "\n";
    
    print_node(new_node3);
    cout << "\n";

    print_node(new_node4);
    cout << "\n";

    print_node(new_node5);
    cout << "\n";

    print_node(new_node6);
    cout << "\n";

    close_file(ptr);

    free_block(new_block1);
    free_block(new_block2);
    free_block(new_block3);
    free_node(new_head);
    free_node(new_node2);
    free_node(new_node3);
    free_node(new_node4);
    free_node(new_node5);
    free_node(new_node6);
    free((char*) str);
    free((char*) s);
    cout << "\n----------------TEST 3----------------\n";
}