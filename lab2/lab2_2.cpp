#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>

pthread_mutex_t mut;

void *proc1(void *flag)
{
    int *flag1 = (int *)flag;
    printf("Thread 1 has started working\n");
    while (*flag1 != 1)
    {
        int rv = pthread_mutex_trylock(&mut);
        if (rv == 0)
        {
            for (int i = 0; i < 5; ++i)
            {
                putchar('1');
                fflush(stdout);
                sleep(1);
            }
            pthread_mutex_unlock(&mut);
            sleep(1);
        }
        else
        {
            printf("\nThread 1: pthread_mutex_trylock() error: %s\n", std::strerror(rv));
            sleep(2);
        }
    }
    printf("Thread 1 has finished working\n");
    pthread_exit((void *)33);
}

void *proc2(void *flag)
{
    int *flag2 = (int *)flag;
    printf("Thread 2 has started working\n");
    while (*flag2 != 1)
    {
        int rv = pthread_mutex_trylock(&mut);
        if (rv == 0)
        {

            for (int i = 0; i < 5; ++i)
            {
                putchar('2');
                fflush(stdout);
                sleep(1);
            }
            pthread_mutex_unlock(&mut);
            sleep(1);
        }
        else
        {
            printf("\nThread 2: pthread_mutex_trylock() error: %s\n", std::strerror(rv));
            sleep(2);
        }
    }
    printf("Thread 2 has finished working\n");
    pthread_exit((void *)44);
}

int main()
{
    pthread_t id1, id2;
    int flag1 = 0, flag2 = 0;
    int exitcode1, exitcode2;

    pthread_mutex_init(&mut, NULL);
    pthread_create(&id1, NULL, proc1, &flag1);
    pthread_create(&id2, NULL, proc2, &flag2);

    printf("Program is waiting for the key to be pressed\n\n");
    getchar();
    printf("Key has been pressed\n\n");

    flag1 = flag2 = 1;
    pthread_join(id1, (void**) &exitcode1);
    pthread_join(id2, (void**) &exitcode2);

    printf("\nThread 1 exitcode = %d\n", exitcode1);
    printf("Thread 2 exitcode = %d\n", exitcode2);

    pthread_mutex_destroy(&mut);
    return 0;
}
