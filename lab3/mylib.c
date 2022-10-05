#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


char *strchr_(char *s, char symbol){
    char *temp = s;
	while(*temp){
		if (*temp == symbol)
			return temp;
		temp += 1;
	}
    
    return NULL;
}

char *strrchr_(char *s, char symbol){
    int i = 0;
    char *res = NULL;

    while(s[i]){
        if(s[i] == symbol)
            res = &s[i];
        i++;
    }
    
    return res;
}

size_t strlen_(const char *s){
    int i = 0;

    while(s[i]){
        ++i;
    }
    
    return i;
}

void memcpy_(char *dest, const char *source, size_t count){
    for(size_t i = 0; i < count; i++){
        dest[i] = source[i];
    }
}

char *readline_(){
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
        }
        else if (n > 0) {
            int chunk_len = (int) strlen_(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy_(res + len, buf, chunk_len);
            len = str_len;
        }
        else {
            scanf("%*c");
        }
    } while (n > 0);

    if (len > 0) {
        res[len] = '\0';
    }
    else {
        res = calloc(1, sizeof(char));
    }

    return res;
}
