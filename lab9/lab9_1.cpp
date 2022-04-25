#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    char file[128];
    printf("Enter the file name:");
    scanf("%s", file);
    int rv = chmod(file, S_IWRITE);
    if (rv == -1)
        perror("\nchmod");
    else
        printf("\nchmod() success\n");
    return 0;
}
