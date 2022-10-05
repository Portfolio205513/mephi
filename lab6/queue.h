typedef struct Item{
    char data;
    struct Item *next;
}Item;

typedef struct Queue{
    Item *head;
}Queue;

Queue *new_queue();

void free_queue(Queue *queue);

int push(Queue *queue, char data);

void print_queue (Queue *queue);

void quick_sort(Item **base, size_t n);

void item_delete(Item *item);

Item *space_delete(Item *start, const char *delim);