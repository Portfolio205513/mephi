#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <time.h>


char *delchr(char *str, char *plc){
    size_t len = strlen(str);
    char *buf = calloc(len, sizeof(char));
    size_t index_plc = plc - str;

    memcpy(buf, str, index_plc);
    memcpy(&buf[index_plc], &str[index_plc+1], len - index_plc);
    buf[len-1] = '\0';
    free(str);
    str = calloc(len, sizeof(char));
    memcpy(str, buf, len);
    free(buf);

    return str;
}


char *addchr(char *str, char *plc, char chr){
    size_t len = strlen(str);
    char *buf = calloc(len + 2, sizeof(char));
    size_t index_plc = plc - str;

    memcpy(buf, str, index_plc);
    buf[index_plc] = chr;
    memcpy(&buf[index_plc + 1], &str[index_plc], len - index_plc + 1);
    buf[len+1] = '\0';
    free(str);
    str = calloc(len + 2, sizeof(char));
    memcpy(str, buf, len + 2);
    free(buf);

    return str;
}


char *strspacedel(char *str)
{
    size_t i = 0;

    while(str[i]){
        if(str[i] == ' ' || str[i] == '\t'){
            if (i){
                str = addchr(str, &str[i], ' ');
                i++;
            }
            while (str[i] == ' ' || str[i] == '\t'){
                str = delchr(str, &str[i]);
            }
        } else {
            i++;
        }
    }
    if(i > 0 && str[i - 1] == ' '){
        str = delchr(str, &str[i-1]);
    }
    
    return str;
}

char *strtreatment(char *str){
    size_t len;
    char *start_point;
    char *end_point;

    start_point = strchr(str, '.');
    end_point = strrchr(str, ';');
    len = strlen(str);
    start_point = start_point ? start_point : str;
    end_point = end_point ? end_point : &str[len];
    if (start_point <= end_point){
        size_t len_new_str;
        char *buf;

        len_new_str = end_point - start_point + 1;
        buf = calloc(len_new_str+1, sizeof(char));
        memcpy(buf, start_point, len_new_str);
        buf[len_new_str]='\0';
        free(str);
        str = calloc(len_new_str + 1, sizeof(char));
        memcpy(str, buf, len_new_str + 1);
        free(buf);
    } else {
        free(str);
        str = calloc(1, sizeof(char));
    }
    return str;
}

int main(){
    char *s = NULL;

    do {
        s = readline("Enter string:\n");
        if (s){        
            s = strspacedel(s);
            s = strtreatment(s);
            printf("Result string:\n%s\n", s);
        }
        free(s);
    } while (s);

    return 0;
}