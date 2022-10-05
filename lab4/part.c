#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "part.h"

void free_(part *mas, size_t *n){
    for(int i = 0; i < *n; i++){
        free(mas[i].name);
    }
    free(mas);
    *n = 0;
}

void copy_part(part *dst, part *src){
    for(int i = 0; i < 8; i++)
        (*dst).id[i] = (*src).id[i];
    (*dst).id[8] = '\0';
    size_t len = strlen((*src).name);
    (*dst).name = calloc(len + 1, sizeof(char));
    memmove((*dst).name, (*src).name, len*sizeof(char));
    (*dst).name[len] = '\0';
    (*dst).amount = (*src).amount;
}

char gen_char(){
    return (char) (rand() % 2 ? rand() % 26 + 97 : rand() % 26 + 65);
}

part gen_part(){
    part temp;
    for(int i = 0; i < 8; i++)
        temp.id[i] = gen_char();
    temp.id[8] = '\0';
    int n = rand() % 100 + 1;
    temp.name = calloc(n + 1, sizeof(char));
    for(int i = 0; i < n; i++)
        temp.name[i] = gen_char();
    temp.amount = rand() % 10000000 + 1;
    return temp;
}

char *get_id(void * a){
    return ((part *)a)->id;
}

char *get_name(void * a){
    return ((part *)a)->name;
}