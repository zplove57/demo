#include<stdio.h>
#include <fcntl.h>
#include<stdlib.h>



int main()
{
    int fd;

    fd=open("/dev/demo_test",O_RDWR);
    if(fd==-1)
    {
        perror("fail to open");
        return -1;
    }
    printf("open success!\n");
    getchar();
    close(fd);
    return 0;

}
