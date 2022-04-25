#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/capability.h>

int main() 
{
    cap_t cp1, cp2;
    cp1 = cap_get_file((const char*)"lab9_1");
    char *st = cap_to_text(cp1, NULL); 
    printf("lab9_1 capabilities:\n%s\n", st);

    cp2 = cap_from_text("cap_fowner=+eip");
    int rv = cap_set_file("lab9_1", cp2);
    if(rv == -1)
        perror("cap_set_file");
    else printf("\ncap_set_file 'lab9_1' success\n"); 

    return 0;
}