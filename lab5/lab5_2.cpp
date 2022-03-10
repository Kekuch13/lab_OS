#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

sem_t *sem; 
FILE* file; 

void *proc(void *flag)
{
    int *flagg = (int *)flag;
    printf("Thread has started working\n");

    while (*flagg != 1)
    {        
        sem_wait(sem);
        for(int i = 0; i < 5; ++i) {
            fputc('2', file);
            fflush(file);
            putchar('2');
            fflush(stdout);
            sleep(1);
        }
        sem_post(sem);
        usleep(1000);
    }
    printf("Thread has finished working\n");
    pthread_exit((void *)44);
}

int main()
{   
    sem = sem_open("/semaphore5", O_CREAT, 0644, 1);
    file = fopen("lab5.txt", "a+");
    int flagg = 0;
    int exitcode;
    pthread_t id;    

    pthread_create(&id, NULL, proc, &flagg);
    
    printf("Program is waiting for the key to be pressed\n\n");
    getchar();
    printf("Key has been pressed\n\n");

    flagg = 1;
    pthread_join(id, (void**) &exitcode);
    printf("\nThread exitcode = %d\n", exitcode);

    fclose(file);
    sem_close(sem);
    sem_unlink("/semaphore5");
    return 0;
}