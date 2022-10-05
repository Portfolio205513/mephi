#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "matrix.h"
#include "streams.h"

char *readline_() {
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;

    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            if (!res) {
                return NULL;
            }
        } else if (n > 0) {
            int chunk_len = (int) strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        } else {
            scanf("%*c");
        }
    } while (n > 0);

    if (len > 0) {
        res[len] = '\0';
    } else {
        res = calloc(1, sizeof(char));
    }
    return res;
}//

int *read_row(char *str, size_t *n){
    *n = 0;
    char *token = strtok(str, " \t");
    int *row = NULL;
    while(token){
        int *buff = check_int(token);
        if (buff == NULL){
            free(row);
            free(buff);
            return NULL;
        }
        (*n)++;
        row = realloc(row, (*n) * sizeof(int));
        row[*n - 1] = *buff;
        free(buff);
        token = strtok(NULL, " \t");
    }
    if (!*n){
        *n = -1;
        free(row);
        return NULL;
    }
    return row;
}//

int *copy_row(int *dst, const int *src, size_t n){
    if (dst)
        free(dst);
    dst = calloc(n, sizeof(int));
    for (int i = 0; i < n; i++)
        dst[i] = src[i];
    return dst;
}//

matrix *read_square_matrix_keyboard(){
    matrix *ans = calloc(1, sizeof(matrix));
    ans->rows = 0;
    ans->columns = 0;
    ans->body = NULL;
    int *row = NULL;

    printf("Enter square matrix. Use \"Ctrl + D\" to stop.\n");
    char *input = readline_();

    if (!input)
        return ans;

    while(input){
        size_t n;
        row = read_row(input, &n);

        if (!ans->columns){
            ans->columns = n;
        }

        if (!row || n != ans->columns){

            if (n != ans->columns)
                free(row);

            free(input);
            printf("Wrong input. Try again.\n");
            input = readline_();
            continue;
        }
        ans->rows++;
        ans->body = realloc(ans->body, ans->rows * sizeof(int *));
        ans->body[ans->rows - 1] = copy_row(NULL, row, ans->columns);

        free(row);
        free(input);

        if (ans->rows == ans->columns){
            printf("Square matrix entered.\n");
            break;
        }

        input = readline_();
    }

    if (ans->columns != ans->rows){
        free_body(ans);
        printf("Entered not square matrix.\n");
    }

    return ans;
}//

matrix *read_square_matrix_binary_file(){
    matrix *ans = calloc(1, sizeof(matrix));
    ans->rows = 0;
    ans->columns = 0;
    ans->body = NULL;
    FILE *file;
    size_t size;

    printf("Enter file path.\n");
    char* input = readline_();
    file = fopen(input, "rb");
    free(input);

    if (!file){
        printf("Can't open file.\n");
        //fclose(file);
        return ans;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);

    if (size < 2 * sizeof(size_t)){
        printf("Wrong input data.\n");
        fclose(file);
        return ans;
    }

    rewind(file);
    fread(&ans->rows, sizeof(size_t), 1, file);
    fread(&ans->columns, sizeof(size_t), 1, file);

    if (ans->columns != ans->rows){
        printf("Wrong input data.\n");
        fclose(file);
        return ans;
    }

    if (size != sizeof(int) * ans->rows * ans->columns + 2 * sizeof(size_t)){
        printf("Wrong input data.\n");
        fclose(file);
        return ans;
    }

    ans->body = calloc(ans->rows, sizeof(int *));

    for(size_t i = 0; i < ans->rows; i++){
        ans->body[i] = calloc(ans->columns, sizeof(int));
        fread(ans->body[i], sizeof(int), ans->columns, file);
    }

    fclose(file);
    return ans;
}//

void print_matrix(matrix src){
    for(size_t i = 0; i < src.rows; i++){
        printf("%d", src.body[i][0]);
        for (size_t j = 1; j < src.columns; j++){
            printf(" %d", src.body[i][j]);
        }
        printf("\n");
    }
}//

void print_matrix_binary_file(matrix src){
    FILE *file;

    printf("Enter file path.\n");
    char* input = readline_();
    file = fopen(input, "wb");

    if (!file){
        free(input);
        fclose(file);
        printf("Can't open file.\n");
        return;
    }
    fwrite(&src.rows, sizeof(size_t), 1, file);
    fwrite(&src.columns, sizeof(size_t), 1, file);

    if (!src.body){
        free(input);
        fclose(file);
        return;
    }

    for(size_t i = 0; i < src.rows; i++)
        fwrite(src.body[i], sizeof(int), src.columns, file);

    free(input);
    fclose(file);
}//

int *check_int(const char *str){
    if (!str)
        return NULL;
    int len_max = (int) log10(pow(2,sizeof(int)*8 - 1) - 1) + 1;
    int sign = 1;
    int ans = 0;
    if (str[0] == '-')
        sign = -1;
    else if (str[0] == '0' && strlen(str) == 1)
        sign = 0;
    else if ((str[0] < 58) && (str[0] > 48))
        ans = ans * 10 + ((int) str[0] - 48);
    else
        return NULL;
    for (size_t i = 1; i < strlen(str); i++){
        if ((str[i] < 58) && (str[i] > 47))
            ans = ans * 10 + ((int) str[i] - 48);
        else
            return NULL;
    }

    if ((strlen(str) > len_max) || ans < 0)
        return NULL;
    ans *= sign;
    int *res = calloc(1, sizeof(int));
    memmove(res, &ans, 1 * sizeof(int));
    return res;
}//

int gen_int(){
    return rand() % 2 ? -1 * rand() % (int) (pow(2, sizeof(int) * 8 - 1) - 1) : rand() % (int) (pow(2, sizeof(int) * 8 - 1) - 1);
}//

int64_t check_natural(const char *s){
    if (!s)
        return -1;
    size_t len = strlen(s);
    int64_t ans = 0;
    if (len >= 19) {
        printf("Too big amount\n");
        return -1;
    }
    if((*s == '0') && (len == 1))
        return 0;
    else if (*s == '0')
        return -1;
    while(*s){
        if(*s >= 48 && *s <= 57){
            ans = ans * 10 + ((int) *s - 48);
            s += 1;
        } else {
            return -1;
        }
    }

    return ans;
}//

matrix *gen_matrix(size_t n){
    matrix *ans = calloc(1, sizeof(matrix));
    ans->rows = n;
    ans->columns = n;
    ans->body = calloc(ans->rows, sizeof(int *));
    for (size_t i = 0; i < ans->rows; i++){
        ans->body[i] = calloc(ans->columns, sizeof(int));
        for (size_t j = 0; j < ans->columns; j++){
            ans->body[i][j] = gen_int();
        }
    }
    return ans;
}//