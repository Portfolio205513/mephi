typedef struct part{
    char id[9];
    char *name;
    size_t amount;
}part;

void free_(part *mas, size_t *n);
void copy_part(part *dst, part *src);
part gen_part();
char *get_id(void * a);
char *get_name(void * a);