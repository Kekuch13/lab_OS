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

void *thrd_write(void *flag);
int flagg = 0;
int fd;

void sig_handler(int fd)
{
    printf("Reader fifo has been disabled\n");
}

void *thrd_open(void *flag)
{
    while (flagg != 1)
    {     
        pthread_t th_wrt;  
        fd = open("/tmp/my_pipe", O_WRONLY|O_NONBLOCK|O_CREAT);
        if (fd == -1) {
            perror("open");
            sleep(1);
        }
        else {
            pthread_create(&th_wrt, NULL, thrd_write, flag);
            pthread_join(th_wrt, NULL);
        }
    }
    pthread_exit((void *)33);
}

void *thrd_write(void *flag)
{
    dev_t devID;
    struct stat buff;
    while (flagg != 1)
    {        
        stat("lab7_1.cpp", &buff);
        devID = buff.st_dev;
        int rv = write(fd, &devID, sizeof(dev_t));
        if (rv == -1)
        {
            perror("write");
        }
        sleep(1);
    }
    pthread_exit((void *)22);
}

int main()
{
    signal(SIGPIPE, sig_handler);
    pthread_t th_opn;
    int *exitcode;

    mkfifo("/tmp/my_pipe", 0644);
    pthread_create(&th_opn, NULL, thrd_open, &flagg);
    
    printf("Program is waiting for the key to be pressed\n\n");
    getchar();
    printf("Key has been pressed\n\n");

    flagg = 1;
    pthread_join(th_opn, (void**) &exitcode);
    printf("Thrd_open exitcode = %p\n", exitcode);
    
    close(fd);
    unlink("/tmp/my_pipe");

    return 0;
}
