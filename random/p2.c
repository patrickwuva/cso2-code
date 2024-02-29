#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
int main(){
    
    int fd = open("file.dat", O_RDWR | O_CREAT);
    char *p;
    p = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);


    printf("p[0] addr: %d\n",p[0]);
    printf("string: %s\n", p);
    munmap(p, 4096);
    close(fd);
}

