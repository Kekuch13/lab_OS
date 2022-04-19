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
#include <sys/socket.h>
#include <sys/un.h>
#include <semaphore.h>
#include <queue>

sem_t *semm;
int sd, flagg = 0;
struct sockaddr_un clAddr;
struct sockaddr_un servAddr;
std::queue<int> queries;

void sig_handler(int fd)
{
    printf("Client is crushed or disconnect with server\n");
}

void *rsv(void *flag)
{
    int *flag1= (int *)flag;

    int rqstID = 0;
    socklen_t len;
    while (*flag1 != 1)
    {
        len = sizeof(servAddr);
        int rv = recvfrom(sd, &rqstID, sizeof(rqstID), 0, (struct sockaddr *)&servAddr, &len); 
        if (rv != -1) {
            sem_wait(semm);
            queries.push(rqstID);         
            sem_post(semm);
            printf("Request %d accepted\n", rqstID);
        }
        else sleep(1);
    }
    pthread_exit((void *)44);
}

void *snd(void *flag)
{
    int *flag2 = (int *)flag;

    dev_t devID;
    struct stat buff;
    int rqstID = 0;
    char buffer[256];
    while (*flag2 != 1)
    {
        sem_wait(semm);
        if(!queries.empty()) {
            rqstID = queries.front();
            queries.pop();
            sem_post(semm);
            stat("lab8_serv.cpp", &buff);
            devID = buff.st_dev;
            int len = sprintf(buffer,"devID = %ld; rqstID = %d",devID,rqstID);
            int rv = sendto(sd, buffer, len, 0, (struct sockaddr *)&clAddr, sizeof(clAddr));
            if (rv != -1) printf("sent: buffer = |%s|\n",buffer);
        }
        else {
            sem_post(semm);
            usleep(500000);            
        }
    }
    pthread_exit((void *)33);
}

int main()
{
    signal(SIGPIPE, sig_handler);
    pthread_t th_rsv, th_snd;
    
    int exitcode1, exitcode2, optval = 1;

    semm = sem_open("/semm", O_CREAT, 0644, 1);

    sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(sd < 0) {
        perror("socket");
        sem_close(semm);
        sem_unlink("/semm");
        close(sd); 
        return 0;
    }
    fcntl(sd, F_SETFL, O_NONBLOCK);
    
    servAddr.sun_family = AF_UNIX;
    strcpy(servAddr.sun_path, "srvsocket"); 
     
    clAddr.sun_family = AF_UNIX;
    strcpy(clAddr.sun_path, "clnsocket"); 

    if (bind(sd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("bind");
        sem_close(semm);
        sem_unlink("/semm");
        close(sd);
        return 0;
    }
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));   

    pthread_create(&th_snd, NULL, rsv, &flagg);
    usleep(100);
    pthread_create(&th_rsv, NULL, snd, &flagg);

    printf("Program is waiting for the key to be pressed\n\n");
    getchar();
    printf("Key has been pressed\n\n");

    flagg = 1;
    pthread_join(th_rsv, (void **)&exitcode1);
    pthread_join(th_snd, (void **)&exitcode2);
    printf("exitcode th_rsv = %d\n", exitcode1);
    printf("exitcode th_snd = %d\n", exitcode2);

    sem_close(semm);
    sem_unlink("/semm");
    close(sd);  
    unlink("srvsocket");
    unlink("clnsocket");
    return 0;
}