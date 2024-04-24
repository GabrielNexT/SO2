#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int isopen(int fd) {
    int result = fcntl(fd, F_GETFD);
    // On error, -1 is returned, and errno is set to indicate the error.
    if(result != -1) return 1;
    //EBADF: fd is not an open file descriptor
    if(errno == EBADF) return 0;

    return 1;
}

int main(void) {
    fopen("test.out", "w");

    int nopen, fd;
    for (nopen = fd = 0; fd < getdtablesize(); fd++) {
        if (isopen(fd)) nopen++;
    }

    printf("Existem %d descritores abertos\n", nopen);
    return (0);
}

