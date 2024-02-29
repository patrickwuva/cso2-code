#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
int main(){
    
    int fd = open("file.dat", O_RDWR);
    char *p;
    ftruncate(fd, 4096);
    p = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    char *test = "hello there";

    strcpy(p, test);
    close(fd);
    printf("p[0] addr: %d\n",p[0]);
    printf("mumapped: sleeping for 10 secs to allow p2 to run\n");
    munmap(p, 4096);
    sleep(10);
}

