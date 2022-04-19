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

int sd, flagg = 0;
struct sockaddr_un servAddr;
struct sockaddr_un clAddr;

void sig_handler(int fd)
{
    printf("Client is crushed or disconnect with server\n");
}

void *snd(void *flag)
{
    int *flag2 = (int *)flag;
    
    int rqstID = 1;
    while (*flag2 != 1)
    {
        int rv = sendto(sd, &rqstID, sizeof(rqstID), 0, (struct sockaddr *)&servAddr, sizeof(servAddr));
        if (rv != -1) {
            printf("Request %d passed\n", rqstID);
            rqstID++;
        }
        sleep(1);
    }
    pthread_exit((void *)22);
}

void *rsv(void *flag)
{
    int *flag1= (int *)flag;

    dev_t devID;
    socklen_t len = sizeof(clAddr);;
    char buffer[256];
    while (*flag1 != 1)
    {
        memset(buffer,0,256);
        int rv = recvfrom(sd, buffer, 256, 0, (struct sockaddr *)&clAddr, &len);
        if (rv == -1) sleep(1);
        else printf("received: buffer = |%s|\n",buffer);
    }
    pthread_exit((void *)11);
}

int main() {
    signal(SIGPIPE, sig_handler);
    pthread_t th_rsv, th_snd;
    int exitcode1, exitcode2, optval = 1;

    sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(sd < 0) {
        perror("socket");
        return 0;
    }
    fcntl(sd, F_SETFL, O_NONBLOCK);

    servAddr.sun_family = AF_UNIX;
    strcpy(servAddr.sun_path, "srvsocket"); 

    clAddr.sun_family = AF_UNIX;
    strcpy(clAddr.sun_path, "clnsocket"); 

    if (bind(sd, (struct sockaddr*)&clAddr, sizeof(clAddr)) < 0) {
        perror("bind");
        close(sd);
        return 0;
    }
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)); 

    pthread_create(&th_rsv, NULL, rsv, &flagg);
    usleep(100);
    pthread_create(&th_snd, NULL, snd, &flagg);

    printf("Program is waiting for the key to be pressed\n\n");
    getchar();
    printf("Key has been pressed\n\n");

    flagg = 1;
    pthread_join(th_rsv, (void **)&exitcode1);
    pthread_join(th_snd, (void **)&exitcode2);
    printf("exitcode th_rsv = %d\n", exitcode1);
    printf("exitcode th_snd = %d\n", exitcode2);

    close(sd);
    unlink("srvsocket");
    unlink("clnsocket");
    return 0;
}
