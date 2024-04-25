#include "lab.h"

#include <string.h>     // for memset
#include <stdio.h>      // for printf

/* When complete, this function should fill in "buffer"
 * with a length-character \0-termianted string such that
 * check_passphrase(buffer) is true.
 *
 * The implementation we supplied tries the guesses of
 * 'a', 'b', and 'c' and prints out how long it takes
 * to check each of them.
 *
 * To do so, your implementation should rely on timing
 * how long check_passphrase takes, most likely by using
 * "measure_once" wrapper function.
 *
 * (Your implementation may not examine the memory in which
 *  the passphrase is stored in another way.)
 */
void check_letters(char *buffer, int index){
    char alphabet[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    int result;
    int t = 5;
    long slowest = 0;
    long avg = 0;
    char c_char = '\0';
    for(int i = 0; i < 26; i++){
        long total_time = 0;
        for(int j = 0; j < t; j++){  
            buffer[index] = alphabet[i];
            long time_char = measure_once(&result, buffer, check_passphrase);
            total_time += time_char; 
           

        }
        avg = total_time/t;
        if (avg > slowest){
            slowest = avg;
            c_char = alphabet[i];
        }
        printf("'%c' took %ld cycles\n", alphabet[i],avg);
    }
    //printf("slowest: %c\n",c_char);
    buffer[index] = c_char;
}


void find_passphrase(char *buffer, int length) {
    memset(buffer, 0, length);
    //check_letters(buffer, 0); 
    
    for(int i = 0; i < length; i++){
        check_letters(buffer, i);
    }
    //printf("buffer: %s\n", buffer);
}
