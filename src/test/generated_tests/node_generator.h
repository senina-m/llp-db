#include "../../api/commands.h"

int get_random_int(int from, int to);

bool get_random_bool();

double get_random_double(double from, double to);

char* get_random_string_from_array();

char* get_random_string(size_t len);

Value_c generate_random_value_by_type(const Value_t t, int min, int max, size_t str_len);

void generate_random_value(Value* val, int min, int max, size_t str_len);