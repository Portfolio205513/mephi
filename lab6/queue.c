#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "queue.h"

Queue *new_queue(){
    return (Queue *) calloc(1, sizeof(Queue));
}

void free_queue(Queue *queue){
    Item *temp = queue->head, *previous;
    while (temp){
        previous = temp;
        temp = temp->next;
        free(previous);
    }
    free(queue);
}

int push(Queue *queue, char data){
    Item *temp = (Item *) malloc(sizeof(Item));

    if(!queue || !temp)
        return -1;

    temp->data = data;
    temp->next = NULL;

    if (!queue->head){
        queue->head = temp;
    } else {
        Item *i = queue->head;

        while(i->next){
            i = i->next;
        }

        i->next = temp;
    }

    return 0;
}

void print_queue (Queue *queue){
    Item *temp = queue->head;

    if (temp->data == '\0'){
        printf("\"\"\n");
        return;
    }
    
    printf("\"");

    while (temp->data != '\0'){
        printf("%c", temp->data);
        temp = temp->next;
    }

    printf("\"\n");
}

char *write_word(Item *item){
    int n = 1;
    char *ans = calloc(1, sizeof(char));
    Item *temp = item;

    while (temp->data != '\0'){
        ans[n - 1] = temp->data;
        temp = temp->next;
        ans = realloc(ans, (++n) * sizeof(char));
    }
    
    ans[n - 1] = '\0';
	
    return ans;
}

void swap (Item *first, Item *second){
    Item *end_first = first;
    Item *end_second = second;

    Item *item_buff = first->next;
    char buff = first->data;

    while (end_first->data != '\0')
        end_first = end_first->next;

    while (end_second->data != '\0')
        end_second = end_second->next;

    first->data = second->data;
    second->data = buff;

    first->next = second->next;
    second->next = item_buff;

    item_buff = end_first->next;
    end_first->next = end_second->next;
    end_second->next = item_buff;
}

void qs(Item **items, size_t left, size_t right){
    int64_t i, j;
    char *x;

    i = (int64_t) left;
    j = (int64_t) right;
    x = write_word(items[(left + right) / 2]);

    do{
        char *temp;

        if (i < (int64_t) right){
            temp = write_word(items[i]);

            while((i < (int64_t) right) && (strcmp(temp, x) < 0)){
                free(temp);
                i++;
                temp = write_word(items[i]);
            }

            free(temp);
        }

        if ((j > (int64_t) left)) {
            temp = write_word(items[j]);

            while ((j > (int64_t) left) && (strcmp(x, temp) < 0)) {
                free(temp);
                j--;
                temp = write_word(items[j]);
            }

            free(temp);
        }

        if (i <= j){
            swap(items[i], items[j]);
            i++;
            j--;
        }
    } while(i <= j);

    free(x);

    if((int64_t) left < j)
        qs(items, left, j);

    if(i < right)
        qs(items, i, right);
}

void quick_sort(Item **base, size_t n){
    qs(base, 0, n - 1);
}

void item_delete(Item *item){
    item->data = item->next->data;
    Item *buff = item->next;
    item->next = item->next->next;
    free(buff);
}

Item *space_delete(Item *start, const char *delim){
    static Item *old;

    if(!start)
        start = old;

    Item *i = start;

    while (i->data != '\0'){
        const char *d = delim;

        while (*d != '\0'){
            if (i->data == *d){
                item_delete(i);
                break;
            }

            d++;
        }

        if (*d == '\0'){
            i = i->next;
            break;
        }

    }

    if (start->data == '\0') {
        old = start;
        return NULL;
    }

    Item *ans = start;

    while (start->data != '\0') {
        const char *d = delim;

        while (*d != '\0'){
            if (start->data == *d)
                break;

            d++;
        }

        if (*d != '\0')
            break;

        start = start->next;
    }

    if (start->data == '\0') {
        old = start;
    } else {
        start->data = '\0';
        old = start->next;
    }

    return ans;
}
