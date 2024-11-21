#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void afficher_info(){
    printf("%d\n",getpid());
    printf("%d\n",getppid());
    printf("%d\n",getpgrp());
    printf("%s\n",getlogin());
    printf("%d\n",getuid());
    printf("%d\n",getgid());
    exit(0);
}

int main(){
    

    afficher_info();
    exit(0);
    }