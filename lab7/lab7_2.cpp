#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>

int flagg = 0;
int fd;

void *thrd_read(void *flag)
{
    int *flag1 = (int *)flag;

    dev_t devID;
    while (*flag1 != 1)
    {        
        devID = 0;
        int rv = read(fd, &devID, sizeof(dev_t));
        if (rv == -1)
        {
            perror("read");
            sleep(1);
        }
        else
        {
            printf("ID of device containing file: %lu\n", devID);
        }
        sleep(1);
    }
    pthread_exit((void *)44);
}

int main()
{
    pthread_t th_rd;
    int *exitcode;

    mkfifo("/tmp/my_pipe", 0644);
    fd = open("/tmp/my_pipe", O_RDONLY|O_NONBLOCK|O_CREAT);
    pthread_create(&th_rd, NULL, thrd_read, &flagg);
    
    printf("Program is waiting for the key to be pressed\n\n");
    getchar();
    printf("Key has been pressed\n\n");

    flagg = 1;
    pthread_join(th_rd, (void**) &exitcode);
    printf("Thrd_read exitcode = %p\n", exitcode);
    
    close(fd);
    unlink("/tmp/my_pipe");

    return 0;
}