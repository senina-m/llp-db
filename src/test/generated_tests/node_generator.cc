#include "node_generator.h"

char symbols[36] = {
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g',
        'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '1', '2', '3', '4',
        '5', '6', '7', '8', '9', '0'};

string strings_dict[50] = {
        "Vestibulum", "malesuada", "amet", "morbi", "aenean", "luctus", "amet", "dapibus", "velit", "molestie",
        "faucibus", "dui", "malesuada", "augue", "accumsan", "lorem", "mattis", "ultricies", "Sit", "mattis", "quis",
        "odio", "Molestieet", "eleifend", "nunc", "justo", "ornare", "quis", "lorem", "interdum", "augue", "amet",
        "velit", "tempus", "mattis", "nulla", "molestie", "non", "vulputate", "lacinia", "in", "ultricies", "Ornare",
        "nunc", "eleifend", "non", "sit", "faucibus", "Nulla", "quamquam"
};

Value_t get_random_value_t(){
    return get_t_by_int(rand() % (4));
}

int get_random_int(int from, int to) {
    return from + rand() % (to - from + 1);
}

bool get_random_bool() {
    return get_random_int(0, 1) == 1;
}

double get_random_double(double from, double to) {
    return (double) rand() / (double) RAND_MAX * (to - from) + from;
}


char* get_random_string_from_array() {
    string str = strings_dict[get_random_int(0, 50)];
    size_t len = str.size() + 1;
    char *s = mmalloc_array(char, len);
    snprintf(s, len, (const char*) str.c_str());
    return s;
}

char* get_random_string(size_t len) {
    auto res = string(len, 'a');
    for (int i = 0; i < len; ++i) {
        res[i] = symbols[get_random_int(0, 35)];
    }
    char *s = mmalloc_array(char, len);
    snprintf(s, len, res.c_str());
    return s;
}


Value_c generate_random_value_by_type(const Value_t t, int min, int max, size_t str_len) {
    Value_c c;
    switch (t) {
        case int_type:
            c = {.int_value = get_random_int(min, max)};
            break;
        case double_type:
            c = {.double_value = get_random_double(min, max)};
            break;
        case string_type:
            c = {.string_value = get_random_string_from_array()};
            break;
        case bool_type:
            c = {.bool_value = get_random_bool()};
            break;
    }

    return c;
}


void generate_random_value(Value* val, int min, int max, size_t str_len){
    Value_t t = get_random_value_t();
    Value_c c = generate_random_value_by_type(t, min, max, str_len);
    val->c = c;
    val->t = t;
}