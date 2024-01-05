#include <stdlib.h>
#include <stdio.h>
#include "split.c"


int main(){
    int words = 0;
    char** test = string_split("hello there","test",&words);
    return 0;
}
