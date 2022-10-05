char *readline_();

int *read_row(char *str, size_t *n);

matrix *read_square_matrix_keyboard();

matrix *read_square_matrix_binary_file();

void print_matrix(matrix src);

void print_matrix_binary_file(matrix src);

int *check_int(const char *str);

int64_t check_natural(const char *s);

matrix *gen_matrix(size_t n);