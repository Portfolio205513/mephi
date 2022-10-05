#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "matrix.h"


void free_all(matrix *src){
    if (!(src->body)){
        free(src);
        return;
    }
    for (size_t i = 0; i < src->rows; i++)
        free(src->body[i]);
    free(src->body);
    src->body = NULL;
    src->rows = 0;
    src->columns = 0;
    free(src);
}//

void free_body(matrix *src){
    if (!src->body)
        return;
    for (size_t i = 0; i < src->rows; i++)
        free(src->body[i]);
    free(src->body);
    src->body = NULL;
    src->rows = 0;
    src->columns = 0;
}//

matrix *copy_matrix(matrix *dst, matrix *src){
    if (dst->body)
        free_body(dst);
    if (!dst)
        dst = calloc(1, sizeof(matrix));
    dst->rows = src->rows;
    dst->columns = src->columns;
    if (src->body)
        dst->body = calloc(src->rows, sizeof(int *));
    for (size_t i = 0; i < src->rows; i++){
        dst->body[i] = calloc(src->columns, sizeof(int));
        memmove(dst->body[i], src->body[i], src->columns * sizeof(int));
    }
    return dst;
}//

void row_insert(matrix *dst, int *row, size_t index){
    dst->body = realloc(dst->body, (dst->rows + 1) * sizeof(int *));
    dst->body[dst->rows] = calloc(dst->columns, sizeof(int));

    for (size_t i = dst->rows; i > index; i--)
        memmove(dst->body[i] , dst->body[i - 1], dst->columns * sizeof(int));

    memmove(dst->body[index], row, dst->columns * sizeof(int));
    dst->rows++;
}//

void column_insert(matrix *src, const int *column, size_t index){
    for (size_t i = 0; i < src->rows; i++){
        src->body[i] = realloc(src->body[i], (src->columns + 1) * sizeof(int));
        memmove(&src->body[i][index + 1], &src->body[i][index], (src->columns - index) * sizeof(int));
        src->body[i][index] = column[i];
    }

    src->columns++;
}//

void row_delete(matrix *src, size_t index){
    if (src->rows == 1){
        free_body(src);
        return;
    }
    for (size_t i = index; i < src->rows - 1; i++)
        memmove(src->body[i], src->body[i + 1], src->columns * sizeof(int));

    free(src->body[src->rows - 1]);
    src->body = realloc(src->body, (src->rows - 1) * sizeof(int *));
    src->rows--;
}//

void column_delete(matrix *src, size_t index){
    if (src->columns == 1){
        free_body(src);
        return;
    }
    for (size_t i = 0; i < src->rows; i++){
        memmove(&src->body[i][index], &src->body[i][index + 1], (src->columns - index - 1) * sizeof(int));
        src->body[i] = realloc(src->body[i], (src->columns - 1) * sizeof(int));
    }

    src->columns--;
}//

int inversions(const size_t *permutation, size_t n){
    size_t *sorted;
    int ans = 1;
    sorted = calloc(n, sizeof(size_t));
    for (size_t i = 0; i < n; i++)
        sorted[i] = i;
    for (size_t i = 0; i < n; i++){
        if (sorted[permutation[i]] % 2 == 1)
            ans *= -1;
        for (size_t j = permutation[i] + 1; j < n; j++)
            if (sorted[j])
                sorted[j]--;
    }
    free(sorted);
    return ans;
}//

int64_t determinant(matrix *src, size_t index, bool *used, size_t *generate){
    int64_t ans = 0;
    if (index == src->columns){
        ans = inversions(generate, index);
        for (size_t i = 0; i < index; i++)
            ans *= src->body[i][generate[i]];
        return ans;
    }
    for (size_t i = 0; i < src->columns; i++){
        if (used[i])
            continue;
        generate[index] = i;
        used[i] = true;
        ans += determinant(src, index + 1, used, generate);
        used[i] = false;
    }
    return ans;
}//

int64_t det(matrix *src){
    bool *used = calloc(src->columns, sizeof(bool));
    size_t *generate = calloc(src->columns, sizeof(size_t));
    int64_t ans = determinant(src, 0, used, generate);

    free(used);
    free(generate);

    return ans;
}//

