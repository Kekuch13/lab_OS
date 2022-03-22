#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

sem_t *sem_wrt, *sem_rd;
int memID;
dev_t *addr;

void *proc(void *flag)
{
    int *flagg = (int *)flag;
    printf("Thread has started working\n");

    dev_t devID;
    while (*flagg != 1)
    {
        sem_wait(sem_wrt);
        devID = *addr;        
        printf("2. ID of device containing file: %lu\n", devID);
        sem_post(sem_rd);
    }
    printf("Thread has finished working\n");
    pthread_exit((void *)33);
}

int main()
{
    int flagg = 0, exitcode;
    pthread_t id;
    
    memID = shmget(ftok("lab6_1.cpp", 0), sizeof(dev_t), 0644 | IPC_CREAT);
    addr = (dev_t*)shmat(memID, NULL, 0);
    sem_wrt = sem_open("/sem_write", O_CREAT, 0644, 0);
    sem_rd = sem_open("/sem_read", O_CREAT, 0644, 0);

    pthread_create(&id, NULL, proc, &flagg);

    printf("Program is waiting for the key to be pressed\n\n");
    getchar();
    printf("Key has been pressed\n\n");

    flagg = 1;
    pthread_join(id, (void **)&exitcode);
    printf("Thread exitcode = %d\n", exitcode);

    sem_close(sem_rd);
    sem_unlink("/sem_read");
    sem_close(sem_wrt);
    sem_unlink("/sem_write");
    shmdt(addr);
    shmctl(memID, IPC_RMID, NULL);

    return 0;
}