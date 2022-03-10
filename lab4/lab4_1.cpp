#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    pid_t pid;
    printf("\nID of the process lab4_1: %d\n", getpid());
    printf("ID of the parent process lab4_1: %d\n", getppid());

    printf("\nArguments:\n");
    for (int i = 1; i < argc; i++){
        printf("%s\n", argv[i]);
        sleep(1);
    }  

    exit(44);
    return 0;
}
