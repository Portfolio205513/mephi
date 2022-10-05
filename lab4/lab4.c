#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <stdint.h>


#include "part.h"
#include "sorts.h"

void help(int menu, int mod);
char *readline_();

part *input_(part *mas, size_t *n, int *flag);
part *treat(part *mas, size_t *n, int *flag);
part *timing_menu(part *mas, size_t n, int *flag);
int output(part *mas, size_t n);
part read_part(char *s);
char *freadline_(FILE *file);
int64_t check_natural(const char *s);
part *sort_menu(part *mas, size_t n, int *flag);


int main(){
    int flag = -1;
    srand(time(NULL));
    part *data = NULL;
    size_t n = 0;
    help(0, 0);
    while (1){
        char *input = readline_();
        if (input && (strlen(input) == 1)){
            switch (input[0]) {
                case '0':
                    free(input);
                    if(data)
                        free_(data, &n);
                    printf("Program end.\n");
                    return 0;
                case '1':
                    if (data)
                        help(0, 1);
                    else
                        help(0, 0);
                    free(input);
                    break;
                case '2':
                    data = input_(data, &n, &flag);
                    free(input);
                    break;
                case '3':
                    if (data)
                        output(data, n);
                    else
                        help(0, -1);
                    free(input);
                    break;
                case '4':
                    if (data)
                        data = treat(data, &n, &flag);
                    else
                        help(0,-1);
                    free(input);
                    break;
                case '5':
                    if (data)
                        data = timing_menu(data, n, &flag);
                    else
                        help(0,-1);
                    free(input);
                    break;
                default:
                    help(0, -1);
                    free(input);
            }
        } else{
            help(0, -1);
            free(input);
        }
    }
}

void help(int menu, int mod){
    switch (menu) {
        case 0:
            switch (mod) {
                case 0:
                    printf("0. Close program.\n"
                           "1. Help.\n"
                           "2. Input data.\n");
                    break;
                case 1:
                    printf("0. Close program.\n"
                           "1. Help.\n"
                           "2. Input data.\n"
                           "3. Output data\n"
                           "4. Manipulate with data.\n"
                           "5. Timing.\n");
                    break;
                default:
                    printf("Wrong command, try again. Enter \"1\" for help\n");
            }
            break;
        case 1:
            printf("0. Back.\n"
                   "1. Help.\n"
                   "2. Input from keyboard.\n"
                   "3. Input from file.\n"
                   "4. Random generate of input data.\n");
            break;
        case 2:
            printf("0. Back.\n"
                   "1. Help.\n"
                   "2. Index insert.\n"
                   "3. Insert to sort data.\n"
                   "4. Delete by index.\n"
                   "5. Sort.\n");
            break;
        case 3:
            if(!mod)
                printf("0. Back.\n"
                       "1. Help.\n"
                       "2. Quick sort.\n"
                       "3. Pair insertion sort.\n"
                       "4. Radix sort.\n");
            else
                printf("0. id.\n"
                       "1. name.\n"
                       "2. amount.\n");
            break;
        case 4:
            printf("0. Back.\n"
                   "1. Help.\n"
                   "2. Output to console.\n"
                   "3. Output to file.\n");
            break;
        case 5:
            printf("0. Back.\n"
                   "1. Help.\n"
                   "2. Timing quick sort.\n"
                   "3. Timing pair insertion sort.\n"
                   "4. Timing radix sort.\n");
            break;
        default:
            printf("Wrong command, try again. Enter \"1\" for help\n");
    }
}


part *input_(part *mas, size_t *n, int *flag){
    help(1, 0);
    while(1) {
        char *input = readline_();
        if (input && (strlen(input) == 1)) {
            switch (input[0]) {
                case '0':
                    if(mas)
                        help(0, 1);
                    else
                        help(0, 0);
                    free(input);
                    return mas;
                case '1':
                    help(1,0);
                    free(input);
                    break;
                case '2':
                    free(input);
                    printf("Use Ctrl+D to stop input data.\n");
                    input = readline_();
                    if (input) {
                        free_(mas, n);
                        mas = NULL;
                        *flag = -1;
                    }
                    while(input){
                        part temp;
                        temp = read_part(input);
                        if(temp.name) {
                            *n += 1;
                            mas = realloc(mas, (*n) * sizeof(part));
                            copy_part(mas + (*n - 1), &temp);
                        } else
                            printf("Try again.\n");
                        free(input);
                        free(temp.name);
                        input = readline_();
                    }
                    if(mas && (*mas).name){
                        help(0, 1);
                        return mas;
                    } else{
                        help(1, 0);
                        break;
                    }
                case '3':
                    free(input);
                    printf("Enter path to file:\n");
                    input = readline_();
                    FILE *file = fopen(input, "r");
                    free(input);
                    if(file){
                        input = freadline_(file);
                        if (input) {
                            free_(mas, n);
                            mas = NULL;
                            *flag = -1;
                        } else {
                            printf("Wrong format of input data.\n");
                            help(1, 0);
                            break;
                        }
                        while (input){
                            part temp;
                            temp = read_part(input);
                            if(temp.name) {
                                *n += 1;
                                mas = realloc(mas, (*n) * sizeof(part));
                                copy_part(mas + (*n - 1), &temp);
                                free(temp.name);
                            } else{
                                free(input);
                                free_(mas, n);
                                mas = NULL;
                                break;
                            }
                            free(input);
                            input = freadline_(file);
                        }
                        if (mas == NULL){
                            help(1, 0);
                            break;
                        }
                        fclose(file);
                        help(0, 1);
                        return mas;
                    } else{
                        printf("File can't open.\n");
                        help(1, 0);
                        break;
                    }
                case '4':
                    free(input);
                    printf("Enter amount of elements data:\n");
                    input = readline_();
                    int64_t n_temp = check_natural(input);
                    free(input);
                    if(0 < n_temp){
                        free_(mas, n);
                        *n = n_temp;
                        mas = calloc(*n, sizeof(part));
                        for(int i = 0; i < *n; i++)
                            mas[i] = gen_part();
                        *flag = -1;
                        help(0, 1);
                        return mas;
                    } else{
                        printf("Wrong format of argument \"amount of elements data\".\n");
                        help(1, 0);
                        break;
                    }
                default:
                    help(-1, -1);
                    free(input);
            }
        } else {
            help(-1, -1);
            free(input);
        }
    }
}

part *treat(part *mas, size_t *n, int *flag){
    help(2, 0);
    while(1) {
        char *input = readline_();
        if (input && (strlen(input) == 1)) {
            switch (input[0]) {
                case '0':
                    help(0,1);
                    free(input);
                    return mas;
                case '1':
                    help(2,0);
                    free(input);
                    break;
                case '2':
                    free(input);
                    printf("Enter index to insert (counting form zero):\n");
                    input = readline_();
                    int64_t index = check_natural(input);
                    free(input);
                    if ((0 <= index) && (index <= (*n + 1))) {
                        printf("Enter new element:\n");
                        input = readline_();
                        part temp = read_part(input);
                        free(input);
                        if(temp.name){
                            mas = index_insert(index, &temp, mas, n);
                            *flag = -1;
                            free(temp.name);
                            printf("Element inserted.\n");
                            help(2, 0);
                            break;
                        } else {
                            help(2, 0);
                            break;
                        }
                    } else {
                        printf("Wrong index.\n");
                        help(2, 0);
                        break;
                    }
                case '3':
                    free(input);
                    part temp;
                    temp.name = NULL;
                    switch (*flag) {
                        case 0:
                            printf("Enter new element:\n");
                            input = readline_();
                            temp = read_part(input);
                            free(input);
                            if(temp.name)
                                mas = index_insert(bin_search(temp, mas, *n, compare_id), &temp, mas, n);
                            else
                                help(2, 0);
                            break;
                        case 1:
                            printf("Enter new element:\n");
                            input = readline_();
                            temp = read_part(input);
                            free(input);
                            if(temp.name)
                                mas = index_insert(bin_search(temp, mas, *n, compare_name), &temp, mas, n);
                            else
                                help(2, 0);
                            break;
                        case 2:
                            printf("Enter new element:\n");
                            input = readline_();
                            temp = read_part(input);
                            free(input);
                            if(temp.name)
                                mas = index_insert(bin_search(temp, mas, *n, compare_amount), &temp, mas, n);
                            else
                                help(2, 0);
                            break;
                        default:
                            printf("No sorted field.\n");
                            break;
                    }
                    if(temp.name){
                        free(temp.name);
                        printf("Element inserted.\n");
                    }
                    help(2, 0);
                    break;
                case '4':
                    free(input);
                    printf("Enter index to delete (counting form zero):\n");
                    input = readline_();
                    index = check_natural(input);
                    free(input);
                    printf("Enter amount of elements to delete (counting form zero):\n");
                    input = readline_();
                    int64_t amount = check_natural(input);
                    free(input);
                    if ((0 <= index) && (index < *n)) {
                        if ((0 < amount) && (amount <= *n - index)){
                            mas = index_delete(index, amount, mas, n);
                            printf("Element(s) deleted.\n");
                            help(2, 0);
                            break;
                        } else {
                            printf("Wrong amount.\n");
                            help(2, 0);
                            break;
                        }
                    } else {
                        printf("Wrong index.\n");
                        help(2, 0);
                        break;
                    }
                case '5':
                    if (!mas){
                        printf("No elements.\n");
                        help(2, 0);
                        free(input);
                        break;
                    }
                    mas = sort_menu(mas, *n, flag);
                    free(input);
                    break;
                default:
                    help(-1, -1);
                    free(input);
            }
        } else {
            help(-1, -1);
            free(input);
        }
    }
}

part *sort_menu(part *mas, size_t n, int *flag){
    help(3, 0);
    while(1) {
        char *input = readline_();
        if (input && (strlen(input) == 1)) {
            switch (input[0]) {
                case '0':
                    help(2,0);
                    free(input);
                    return mas;
                case '1':
                    help(3,0);
                    free(input);
                    break;
                case '2':
                    free(input);
                    printf("Choose field to sort.\n");
                    help(3, 1);
                    input = readline_();
                    if (input && (strlen(input) == 1)) {
                        switch (input[0]) {
                            case '0':
                                quick_sort(mas, n, compare_id);
                                printf("Sorted by \"id\" field.\n");
                                *flag = 0;
                                free(input);
                                help(3, 0);
                                break;
                            case '1':
                                quick_sort(mas, n, compare_name);
                                printf("Sorted by \"name\" field.\n");
                                *flag = 1;
                                free(input);
                                help(3, 0);
                                break;
                            case '2':
                                quick_sort(mas, n, compare_amount);
                                printf("Sorted by \"amount\" field.\n");
                                *flag = 2;
                                free(input);
                                help(3, 0);
                                break;
                            default:
                                help(-1, -1);
                                free(input);
                        }
                    } else{
                        help(-1, -1);
                        free(input);
                    }
                    break;
                case '3':
                    free(input);
                    printf("Choose field to sort.\n");
                    help(3, 1);
                    input = readline_();
                    if (input && (strlen(input) == 1)) {
                        switch (input[0]) {
                            case '0':
                                pair_sort(mas, n, compare_id);
                                printf("Sorted by \"id\" field.\n");
                                *flag = 0;
                                free(input);
                                help(3, 0);
                                break;
                            case '1':
                                pair_sort(mas, n, compare_name);
                                printf("Sorted by \"name\" field.\n");
                                *flag = 1;
                                free(input);
                                help(3, 0);
                                break;
                            case '2':
                                pair_sort(mas, n, compare_amount);
                                printf("Sorted by \"amount\" field.\n");
                                *flag = 2;
                                free(input);
                                help(3, 0);
                                break;
                            default:
                                help(-1, -1);
                                free(input);
                        }
                    } else{
                        help(-1, -1);
                        free(input);
                    }
                    break;
                case '4':
                    free(input);
                    printf("Choose field to sort.\n");
                    help(3, 1);
                    input = readline_();
                    if (input && (strlen(input) == 1)) {
                        switch (input[0]) {
                            case '0':
                                radix_sort(mas, n, 0, get_id);
                                printf("Sorted by \"id\" field.\n");
                                *flag = 0;
                                free(input);
                                help(3, 0);
                                break;
                            case '1':
                                radix_sort(mas, n, 0, get_name);
                                printf("Sorted by \"name\" field.\n");
                                *flag = 1;
                                free(input);
                                help(3, 0);
                                break;
                            case '2':
                                radix_sort(mas, n, 2, get_id);
                                printf("Sorted by \"amount\" field.\n");
                                *flag = 2;
                                free(input);
                                help(3, 0);
                                break;
                            default:
                                help(-1, -1);
                                free(input);
                        }
                    } else{
                        help(-1, -1);
                        free(input);
                    }
                    break;
                default:
                    help(-1, -1);
                    free(input);
            }
        } else{
            help(-1, -1);
            free(input);
        }
    }
}

part read_part(char *s){
    char *id = strtok(s, " \t");
    char *name = strtok(NULL, " \t");
    char *amount = strtok(NULL, " \t");
    char *next = strtok(NULL, " \t");
    int64_t amount_i;
    part temp;

    if (id && name && amount && !next){
        if(strlen(id) == 8){
            amount_i = check_natural(amount);
            if(0 >= amount_i){
                temp.name = NULL;
                printf("Wrong format of argument \"amount\".\n");
                return temp;
            }
            size_t i = 0;
            while(name[i]){
                if((name[i] >= 91 || name[i] <= 64) && (name[i] >= 123 || name[i] <= 96)){
                    temp.name = NULL;
                    printf("Wrong format of argument \"name\".\n");
                    return temp;
                }
                i += 1;
            }
            for(i = 0; i < 8; i++){
                if((*id >= 91 || *id <= 64) && (*id >= 123 || *id <= 96)){
                    temp.name = NULL;
                    printf("Wrong format of argument \"id\".\n");
                    return temp;
                }
                temp.id[i] = id[i];
            }
            temp.id[8] = '\0';
            size_t len = strlen(name);
            temp.name = calloc(len + 1, sizeof(char));
            memmove(temp.name, name, len*sizeof(char));
            temp.name[len] = '\0';
            temp.amount = amount_i;
            return temp;
        } else {
            temp.name = NULL;
            printf("Wrong length of id.\n");
            return temp;
        }
    } else {
        temp.name = NULL;
        printf("Wrong amount of arguments for one part.\n");
        return temp;
    }
}

part *timing_menu(part *mas, size_t n, int *flag){
    help(5, 0);
    clock_t start, end;
    
    while(1) {
        char *input = readline_();
        if (input && (strlen(input) == 1)) {
            switch (input[0]) {
                case '0':
                    help(0,1);
                    free(input);
                    return mas;
                case '1':
                    help(5,0);
                    free(input);
                    break;
                case '2':
                    free(input);
                    printf("Choose field to sort.\n");
                    help(3, 1);
                    input = readline_();
                    if (input && (strlen(input) == 1)) {
                        switch (input[0]) {
                            case '0':
                                start = clock();
                                quick_sort(mas, n, compare_id);
                                end = clock();
                                printf("Sorted by \"id\" field, %f time spent in sec.\n", (double) (end - start) / CLOCKS_PER_SEC);
                                *flag = 0;
                                free(input);
                                help(5, 0);
                                break;
                            case '1':
                                start = clock();
                                quick_sort(mas, n, compare_name);
                                end = clock();
                                printf("Sorted by \"name\" field, %f time spent in sec.\n", (double) (end - start) / CLOCKS_PER_SEC);
                                *flag = 1;
                                free(input);
                                help(5, 0);
                                break;
                            case '2':
                                start = clock();
                                quick_sort(mas, n, compare_amount);
                                end = clock();
                                printf("Sorted by \"amount\" field, %f time spent in sec.\n", (double) (end - start) / CLOCKS_PER_SEC);
                                *flag = 2;
                                free(input);
                                help(5, 0);
                                break;
                            default:
                                help(-1, -1);
                                free(input);
                        }
                    } else{
                        help(-1, -1);
                        free(input);
                    }
                    break;
                case '3':
                    free(input);
                    printf("Choose field to sort.\n");
                    help(3, 1);
                    input = readline_();
                    if (input && (strlen(input) == 1)) {
                        switch (input[0]) {
                            case '0':
                                start = clock();
                                pair_sort(mas, n, compare_id);
                                end = clock();
                                printf("Sorted by \"id\" field, %f time spent in sec.\n", (double) (end - start) / CLOCKS_PER_SEC);
                                *flag = 0;
                                free(input);
                                help(5, 0);
                                break;
                            case '1':
                                start = clock();
                                pair_sort(mas, n, compare_name);
                                end = clock();
                                printf("Sorted by \"name\" field, %f time spent in sec.\n", (double) (end - start) / CLOCKS_PER_SEC);
                                *flag = 1;
                                free(input);
                                help(5, 0);
                                break;
                            case '2':
                                start = clock();
                                pair_sort(mas, n, compare_amount);
                                end = clock();
                                printf("Sorted by \"amount\" field, %f time spent in sec.\n", (double) (end - start) / CLOCKS_PER_SEC);
                                *flag = 2;
                                free(input);
                                help(5, 0);
                                break;
                            default:
                                help(-1, -1);
                                free(input);
                        }
                    } else{
                        help(-1, -1);
                        free(input);
                    }
                    break;
                case '4':
                    free(input);
                    printf("Choose field to sort.\n");
                    help(3, 1);
                    input = readline_();
                    if (input && (strlen(input) == 1)) {
                        switch (input[0]) {
                            case '0':
                                start = clock();
                                radix_sort(mas, n, 0, get_id);
                                end = clock();
                                printf("Sorted by \"id\" field, %f time spent in sec.\n", (double) (end - start) / CLOCKS_PER_SEC);
                                *flag = 0;
                                free(input);
                                help(5, 0);
                                break;
                            case '1':
                                start = clock();
                                radix_sort(mas, n, 0, get_name);
                                end = clock();
                                printf("Sorted by \"name\" field, %f time spent in sec.\n", (double) (end - start) / CLOCKS_PER_SEC);
                                *flag = 1;
                                free(input);
                                help(5, 0);
                                break;
                            case '2':
                                start = clock();
                                radix_sort(mas, n, 2, get_id);
                                end = clock();
                                printf("Sorted by \"amount\" field, %f time spent in sec.\n", (double) (end - start) / CLOCKS_PER_SEC);
                                *flag = 2;
                                free(input);
                                help(5, 0);
                                break;
                            default:
                                help(-1, -1);
                                free(input);
                        }
                    } else{
                        help(-1, -1);
                        free(input);
                    }
                    break;
                default:
                    help(-1, -1);
                    free(input);
            }
        } else{
            help(-1, -1);
            free(input);
        }
    }
}

int output(part *mas, size_t n){
    help(4, 0);
    while(1) {
        char *input = readline_();
        if (input && (strlen(input) == 1)) {
            switch (input[0]) {
                case '0':
                    help(0,1);
                    free(input);
                    return 0;
                case '1':
                    help(4,0);
                    free(input);
                    break;
                case '2':
                    for (int i = 0; i < n; i++)
                        printf("id: %s, name: %s, amount: %d.\n", mas[i].id, mas[i].name, (int) mas[i].amount);
                    free(input);
                    help(4, 0);
                    break;
                case '3':
                    free(input);
                    printf("Enter path to file:\n");
                    input = readline_();
                    FILE *file = fopen(input, "w");
                    free(input);
                    if(file){
                        for (int i = 0; i < n; i++)
                            fprintf(file, "%s %s %d\n", mas[i].id, mas[i].name, (int) mas[i].amount);
                        fclose(file);
                        help(4, 0);
                    } else{
                        printf("File can't open or make.\n");
                        help(4, 0);
                    }
                    break;
                default:
                    help(-1, -1);
                    free(input);
            }
        } else {
            help(-1, -1);
            free(input);
        }
    }
}


int64_t check_natural(const char *s){
    size_t len = strlen(s);
    int64_t ans = 0;
    
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
    if ((len > 19) || ((len == 19) && (ans < 0))) {
        printf("Too big amount\n");
        return -1;
    }
    return ans;
}

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
}

char *freadline_(FILE *file){
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;

    do {
        n = fscanf(file, "%80[^\n]", buf);
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
            fscanf(file, "%*c");
        }
    } while (n > 0);

    if (len > 0) {
        res[len] = '\0';
    } else {
        res = calloc(1, sizeof(char));
    }
    return res;
}