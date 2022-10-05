#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"


char *readline_() {
    char *ans = calloc(1, sizeof(char));
    int n = 1;

    ans[n-1] = getchar();

    if (ans[n-1] == EOF){
        free(ans);
        return NULL;
    }
        
    while(ans[n-1] != '\n'){
        ans = realloc(ans, (++n)*sizeof(char));
        ans[n-1] = getchar();

        if (ans[n-1] == EOF)
            break;
    }

    ans[n-1] = '\0';

    return ans;
}

int enter(){
    Queue *queue;
    Item **words;
    Item * item_temp;
    char *input;
    char *temp;
    int n;

    printf("Enter string:\n");
    input = readline_();

    if (!input){
        return -1;
    }

    temp = input;
    queue = new_queue();

    while (*temp != '\0'){
        push(queue, *temp);
        temp++;
    }

    push(queue, '\0');

    n = 0;
    words = calloc(++n, sizeof(Item *));
    words[n-1] = space_delete(queue->head, " \t");

    while (words[n-1]) {
        words = realloc(words, (++n)*sizeof(Item *));
        words[n-1] = space_delete(NULL, " \t");
    }

    if (n > 1)
        quick_sort(words, n - 1);

    item_temp = queue->head;

    while (item_temp){
        if (item_temp->data == '\0' && item_temp->next){
            if (item_temp->next->data == '\0')
                item_delete(item_temp);
            else
                item_temp->data = ' ';
        }
        
        item_temp = item_temp->next;
    }

    printf("\"%s\"\n", input);
    print_queue(queue);
    free(words);
    free(input);
    free_queue(queue);

    return 0;
}

int main (){
    while(enter() == 0){} 
}