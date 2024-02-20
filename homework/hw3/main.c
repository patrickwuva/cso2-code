#include "fork_run.c"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
    /*printf("Hi!\n");
    printf("Test: [[[%s]]]\n",
            getoutput("echo 1 2 3; sleep 2; echo 5 5"));
    printf("Bye\n");    
    */

    const char *argv_base[] = {
        "/bin/echo", "running", NULL
    };
    const char *output = parallelgetoutput(2, argv_base);

    printf("Text: [%s]\n", output);
    return 0;
}
