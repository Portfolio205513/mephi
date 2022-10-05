#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include<math.h>

#include "part.h"
#include "sorts.h"

part *index_insert(size_t index, part *element, part *mas, size_t *n);

size_t bin_search(part element, part *base, size_t n, int (*compare) (const void *, const void *)){
    size_t l = 0;
    size_t r = n;
    while (l < r){
        size_t m = (l + r) / 2;
        if(compare(base + m, &element) < 0)
            l = m + 1;
        else
            r = m;
    }
    return l;
}

void swap(part *one, part *two){
    part temp;

    if((*one).name)
        copy_part(&temp, one);

    free((*one).name);
    (*one).name = NULL;

    if((*two).name)
        copy_part(one, two);

    free((*two).name);
    copy_part(two, &temp);
    free(temp.name);
}

void qs(part *items, size_t left, size_t right, int (*compare) (const void *, const void *)){
    int64_t i, j;
    part x;

    i = (int64_t) left;
    j = (int64_t) right;
    copy_part(&x, &items[(left + right) / 2]);

    do{
        while((i < (int64_t) right) && (compare(&items[i], &x) < 0))
            i++;

        while((j > (int64_t) left) && (compare(&x, &items[j]) < 0))
            j--;

        if (i <= j){
            swap(&items[i], &items[j]);
            i++;
            j--;
        }
    } while(i <= j);

    if((int64_t) left < j)
        qs(items, left, j, compare);

    if(i < right)
        qs(items, i, right, compare);

    free(x.name);
}

void quick_sort(void *base, size_t n, int (*compare) (const void *, const void *)){
    qs((part *) base, 0, n - 1, compare);
}

void pair_sort(void *base, size_t n, int (*compare) (const void *, const void *)){
    for(size_t i = 1; i < n; i = i + 2){
        part max_, min_;
        int64_t j;

        if(compare(((part *) base) + (i - 1), ((part *) base) + i) < 0){
            copy_part(&max_,((part *) base) + i);
            copy_part(&min_, ((part *) base) + (i - 1));
        } else {
            copy_part(&min_,((part *) base) + i);
            copy_part(&max_, ((part *) base) + (i - 1));
        }

        j = (int64_t) i - 2;

        while((j >= 0) && (compare(&max_,((part *) base) + j) < 0)){
            free((*((part *) base + (j + 2))).name);
            copy_part(((part *) base + (j + 2)), ((part *) base) + j);
            j--;
        }

        j++;
        free((*(((part *) base) + (j + 1))).name);
        copy_part(((part *) base) + (j + 1), &max_);
        j--;

        while((j >= 0) && (compare(&min_,((part *) base) + j) < 0)){
            free((*((part *) base + (j + 1))).name);
            copy_part(((part *) base + (j + 1)), ((part *) base) + j);
            j--;
        }

        free((*(((part *) base) + (j + 1))).name);
        copy_part(((part *) base) + (j + 1), &min_);
        free(min_.name);
        free(max_.name);
    }

    part last;
    int64_t i = (int64_t) n - 2;

    copy_part(&last, ((part *) base) + (n - 1));

    while((i >= 0) && (compare(&last,((part *) base) + i) < 0)){
        free((*((part *) base + (i + 1))).name);
        copy_part(((part *) base + (i + 1)), ((part *) base) + i);
        i--;
    }

    free(last.name);
}

void r_sort(part *mas, size_t n, size_t d, char *(*field) (void *)){
    part* temp [52] = {NULL};
    size_t len_t [52] = {0};

    for(size_t j = 0; j < n; j++){
        int place = (int) field((part *) mas + j)[d];
        place = place > 96 ? place - 97 + 26 : place - 65;
        temp[place] = index_insert(len_t[place], (part *) mas + j, temp[place], &len_t[place]);
    }

    for (int j = 0; j < 52; j++){
        size_t previous = 0;
        for(size_t i = 0; i < len_t[j]; i++){
            if(strlen(field(temp[j] + i)) < d + 2){
                if (previous + 1 < i){
                    r_sort(temp[j] + previous, i - previous, d + 1, field);
                }
                previous = i + 1;
            }
        }
        if ((len_t[j] > 1) && (previous == 0))
            r_sort(temp[j], len_t[j], d + 1, field);
    }

    n = 0;

    for (int j = 0; j < 52; j++){
        for (size_t k = 0; k < len_t[j]; k++){
            free(((part *)mas + n)->name);
            copy_part((part *)mas + n, temp[j] + k);
            n++;
        }

        free_(temp[j], &len_t[j]);
        temp[j] = NULL;
        len_t[j] = 0;
    }
}

void radix_sort(void *base, size_t n, size_t size, char *(*field) (void *)){
    part* digits [10] = {NULL};
    size_t dig_flag [10] = {0};
    size_t max_len;
    size_t digit;
    int len_d;
    switch (size) {
        case 0:
            r_sort(base, n, 0, field);
           break;
        case 2:
            max_len = 0;
            for (size_t i = 0; i < n; i++){
                digit = ((part *) base + i)->amount;
                len_d = digit ? (int) log10l(digit) + 1 : 1;
                if(len_d > max_len)
                    max_len = len_d;
            }        
                size_t flag = 1;
            for (int i = 0; i < max_len; i++){

                for(size_t j = 0; j < n; j++){
                    int place = ((part *) base + j)->amount / flag % 10;
                    digits[place] = index_insert(dig_flag[place], (part *) base + j, digits[place], &dig_flag[place]);
                }

                n = 0;

                for (int j = 0; j < 10; j++){
                    for (size_t k = 0; k < dig_flag[j]; k++){
                        free(((part *)base + n)->name);
                        copy_part((part *)base + n, digits[j] + k);
                        n++;
                    }

                    free_(digits[j], &dig_flag[j]);
                    digits[j] = NULL;
                    dig_flag[j] = 0;
                }
                flag *= 10;
            }
            break;
    }
}

part *index_insert(size_t index, part *element, part *mas, size_t *n){
    mas = realloc(mas, (*n + 1) * sizeof(part));

    for (size_t i = *n; i > index; i--){
        copy_part(mas + i, mas + (i - 1));
        free(mas[i-1].name);
    }

    copy_part(mas + index, element);
    *n += 1;

    return mas;
}

part *index_delete(size_t index, size_t amount, part *mas, size_t *n){
    for (size_t i = index; i < *n - amount; i++){
        free(mas[i].name);
        copy_part(mas + i, mas + (i + amount));
    }

    for (size_t i = *n - amount; i < *n; i++){
        free(mas[i].name);
    }

    mas = realloc(mas, (*n - amount) * sizeof(part));
    *n -= amount;

    return mas;
}

int compare_id(const void *a, const void *b){
    return strcmp((*(part *) a).id, (*(part *) b).id);
}

int compare_name(const void *a, const void *b){
    return strcmp((*(part *) a).name, (*(part *) b).name);
}

int compare_amount(const void *a, const void *b) {
    if ((*(part *) a).amount < (*(part *) b).amount)
        return -1;
    else if ((*(part *) a).amount > (*(part *) b).amount)
        return 1;
    else
        return 0;
}