#include <stdio.h>
#include <string.h>

int main(){

    char sep[4] = "XYZ";
    char tmp[2] = "A";

    printf("%zu\n",strcspn(sep,tmp));
    return 0;

}
