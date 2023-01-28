#include "tests.h"

void print(){
    FILE* ptr = open_file("test");
    cout << "\nblocks:\n";
    print_free_space(read_free_space_offset(ptr), ptr);
    ptr = open_file("test");
    cout << "\ntree:\n\n";
    print_tree(read_tree_head_offset(ptr), ptr, 0);
    cout << "\nheader:";
    print_header(ptr);
    close_file(ptr);
}

size_t create_state(){
    FILE* file_ptr = fopen("test", "w");
    fclose(file_ptr);

    open_file_db("test");

    Node *head = read_first_node();

    const char *str = mmalloc_array(char, 15); // 14+\0
    snprintf((char *)str, 15, "Hello world!!!");
    Value_c c2 = {.string_value = str};
    Value v2 = {.c = c2, .t = string_type};
    Node *node2 = add_node_db(head, &v2);
    // free((void * )str);

    Value_c c3 = {.double_value = 0.5};
    Value v3 = {.c = c3, .t = double_type};
    Node *node3 = add_node_db(node2, &v3);

    Value_c c4 = {.bool_value = true};
    Value v4 = {.c = c4, .t = bool_type};
    Node *node4 = add_node_db(head, &v4);

    Value_c c5 = {.double_value = -0.6};
    Value v5 = {.c = c5, .t = double_type};
    Node *node5 = add_node_db(head, &v4);

    Value_c c6 = {.int_value = 42};
    Value v6 = {.c = c6, .t = int_type};
    Node *node6 = add_node_db(node2, &v6);

    Value_c c7 = {.int_value = 54};
    Value v7 = {.c = c7, .t = int_type};
    Node *node7 = add_node_db(node3, &v4);

    size_t offset = node2->offset;
    free_node(head);
    free_node(node2);
    free_node(node3);
    free_node(node4);
    free_node(node5);
    free_node(node6);
    free_node(node7);

    close_file_db();

    cout << "============init_state :=========================== \n tree:\n";
    print();
    cout << "================================== \n\n";
    return offset;
}

void test1()
{

    cout << "\n----------------TEST 1----------------\n";
    Value_c c = {.string_value = "abc"};
    Node node = {.offset = 5, 
    .node_len = 15, 
    .first_child = 7, .parent = 4, .prev_sibling = 4, .next_sibling = 13, .value_len = 4, .value_t = 2, .value_c = c};

    FILE *ptr = open_file("test");
    write_node(&node, ptr);
    Node *new_node = read_node(5, ptr);
    cout << "printing of wtiten to file and read from it node:\n";
    print_node(&node);
    cout << "\n";
    print_node(new_node);
    free_node(new_node);

    change_first_child_offset(5, 1234, ptr);
    change_next_sibling_offset(5, 98097, ptr);
    change_parent_offset(5, 21424, ptr);

    cout << "\nread node header:\n";
    Node *header = read_node_header(5, ptr);
    print_node_header(header);
    free(header);

    close_file(ptr);
    cout << "\n----------------TEST 1----------------\n";
}

void test2()
{
    cout << "\n----------------TEST 2----------------\n";

    Value_c c1 = {.bool_value = false};
    Node head = {
        .offset = 24,
        .node_len = 50,
        .first_child = 134,
        .parent = 0,
        .prev_sibling = 0,
        .next_sibling = 0,
        .value_len = 1,
        .value_t = 3,
        .value_c = c1};

    const char *str = mmalloc_array(char, 15); // 14+\0
    snprintf((char *)str, 15, "Hello world!!!");
    Value_c c2 = {.string_value = str};
    Node node2 = {
        .offset = 134,
        .node_len = 64,
        .first_child = 198,
        .parent = 24,
        .prev_sibling = 0,
        .next_sibling = 255,
        .value_len = 15,
        .value_t = 2,
        .value_c = c2};

    Value_c c3 = {.double_value = 0.5};
    Node node3 = {
        .offset = 198,
        .node_len = 57,
        .first_child = 0,
        .parent = 134,
        .prev_sibling = 0,
        .next_sibling = 0,
        .value_len = 8,
        .value_t = 1,
        .value_c = c3};

    Value_c c4 = {.bool_value = true};
    Node node4 = {
        .offset = 255,
        .node_len = 50,
        .first_child = 0,
        .parent = 24,
        .prev_sibling = 134,
        .next_sibling = 0,
        .value_len = 1,
        .value_t = 3,
        .value_c = c4};

    Block block1 = {.next = 305, .prev = 0, .size = 24};
    Block block2 = {.next = 0, .prev = 74, .size = 50};
    File_header file_header = {
        .tree_head_offset = 24,
        .free_space_offset = 74,
        .end_of_last_node_offset = (305 + block2.size)};

    FILE *ptr = open_file("test");
    write_block(&block2, ptr, 305);
    write_node(&head, ptr);
    write_header(&file_header, ptr);

    write_block(&block1, ptr, 74);
    write_node(&node2, ptr);
    write_node(&node3, ptr);
    write_node(&node4, ptr);

    cout << "============init_state :=========================== \ntree:\n";
    print_tree(file_header.tree_head_offset, ptr, 0);

    cout << "\nblocks:\n";
    print_free_space(file_header.free_space_offset, ptr);

    cout << "\nheader:";
    print_header(ptr);
    cout << "================================== \n\n";
    close_file(ptr);

    Value_c c5 = {.bool_value = true};
    Value v = {.c = c5, .t = bool_type};

    open_file_db("test");
    Node *added_node = add_node_db(&node2, &v);
    close_file_db();

    cout << "\nadded node: \n";
    print_node(added_node);
    cout << endl;

    free_node(added_node);
    free((void*) node2.value_c.string_value);

    ptr = open_file("test");
    cout << "\nblocks:\n";
    print_free_space(file_header.free_space_offset, ptr);

    cout << "\nnodes:\n";
    print_tree(file_header.tree_head_offset, ptr, 0);

    cout << "\nheader:";
    print_header(ptr);

    close_file(ptr);
    // free((char *)str);
    cout << "\n----------------TEST 2----------------\n";
}

void test3()
{
    cout << "\n----------------TEST 3----------------\n";
    size_t offset = create_state();
    // exit(1);
    // Value_c c8 = {.bool_value=true};
    // Value v = {.c=c8, .t=bool_type};
    const char *s = mmalloc_array(char, 20); // 14+\0
    snprintf((char *)s, 20, "Hello world, dear:)");
    Value_c c8 = {.string_value = s};
    Value v = {.c = c8, .t = string_type};


    open_file_db("test");
    Node* read_node = read_offset_node(offset);

    edit_node_db(read_node, &v);
    close_file_db();

    cout << "\nedited_node: \n";
    print_node(read_node);
    cout << endl;
    free_node(read_node);
    // free((void*) s);

    print();
    cout << "\n----------------TEST 3----------------\n";
}

void test4()
{
    cout << "\n----------------TEST 4----------------\n";

    size_t offset = create_state();

    open_file_db("test");
    Node* read_node = read_offset_node(offset);

    delete_node_db(read_node);
    close_file_db();

    cout << "\nedited_node: \n";
    print_node(read_node);
    cout << endl;
    free_node(read_node);

    print();
    cout << "\n----------------TEST 4----------------\n";
}

void test5(){
    cout << "\n----------------TEST 5----------------\n";

    size_t offset = create_state();

    Value_c c4 = {.bool_value = true};
    Value v4 = {.c = c4, .t = bool_type};

    Condition cond = {.value=v4, .comp=EQUAL };

    open_file_db("test");
    Iter it = find_nodes(cond);
    it.print();
    it.free();
    close_file_db();

    // print();
    cout << "\n----------------TEST 5----------------\n";
}