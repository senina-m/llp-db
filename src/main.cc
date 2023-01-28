#include "test/hand_tests/tests.h"
#include "test/generated_tests/test_generator.h"

int main(){
    // test1();
    // test2();
    // test3();
    // test4();
    // test5();

    // random_insert_test(20000);
    
    // random_edit_test(20000);

    // const char *str = mmalloc_array(char, 5);
    // snprintf((char *)str, 5, "amet");
    // Value_c c = {.string_value = str};
    // Value v = {.c = c, .t = string_type};
    // random_find_test( Condition(v, EQUAL), 20000);
    // free((void*) str);

    
    // random_delete_test(20000);
    // random_file_size_test(10000);

    open("test"); //начинаем работу с файлом
    Node* node = read_first(); //находим голову дерева
    
    //подготваливаем значение, чтобы добавить новую ноду
    const char *str = (char*) malloc(sizeof(char)*5);
    snprintf((char *)str, 5, "hell");
    Value_c c1 = {.string_value = str};
    Value v1 = {.c = c1, .t = string_type};

    Node* added = add_node_db(node, &v1); //добавляем ноду

    print(added); //печатаем её в консоль

    //подготваливаем значение, чтобы изменить ноду
    Value_c c2 = {.bool_value = true};
    Value v2 = {.c = c2, .t = bool_type};

    edit_node(v2, added); //изменяем ноду

    print(added); //печатаем её в консоль

    delete_node(added); //удаляем ноду

    f_node(added);
    f_node(node);

    close(); //завершаем работу с файлом

    return 0;
}