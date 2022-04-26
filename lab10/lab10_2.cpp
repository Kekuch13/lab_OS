#include <cstdio>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ev.h>

struct ev_loop *loop;
ev_child child_watcher;

static void child_cb(EV_P_  ev_child *w, int revents) {
    ev_child_stop(loop,  w);
    ev_break(loop,  EVBREAK_ALL);       

    printf("\nExit_status lab10_1 = %d\n", WEXITSTATUS(w->rstatus));
}

int main(int argc, char* argv[])
{
    printf("ID of the process lab10_2: %d\n", getpid());
    pid_t w, pid = fork();
    
    loop = EV_DEFAULT;
    ev_child_init(&child_watcher, child_cb, pid, 0);
    ev_child_start(loop, &child_watcher);

    if (pid == 0) {
        printf("ID of the child process lab10_2: %d\n", getpid());

        execv("lab10_1", argv);
    } 
    else if (pid > 0) {
        printf("ID of the parent process lab10_2: %d\n", getpid());
        ev_run (loop, 0);
    }
    else {
        perror("fork");
    }

    return 0;
}