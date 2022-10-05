typedef struct matrix{
    size_t rows;
    size_t columns;
    int **body;
}matrix;

void free_all(matrix *src);

void free_body(matrix *src);

matrix *copy_matrix(matrix *dst, matrix *src);

void row_insert(matrix *dst, int *row, size_t index);

void column_insert(matrix *src, const int *column, size_t index);

void row_delete(matrix *src, size_t index);

void column_delete(matrix *src, size_t index);

int64_t det(matrix *src);