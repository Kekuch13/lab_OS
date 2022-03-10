#include <cstdio>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    printf("ID of the process lab4_2: %d\n", getpid());
    pid_t w, pid = fork();

    if (pid == 0) {
        printf("ID of the child process lab4_2: %d\n", getpid());

        execv("lab4_1", argv);
    } 
    else if (pid > 0) {
        printf("ID of the parent process lab4_2: %d\n", getpid());

        int status;
        while (w = waitpid(pid, &status, WNOHANG) == 0) {
            usleep(500000);
        }
        if (w == -1) perror("waitpid");
        else printf("\nExitstatus lab4_1 = %d\n", WEXITSTATUS(status));
    }
    else {
        perror("fork");
    }

    return 0;
}