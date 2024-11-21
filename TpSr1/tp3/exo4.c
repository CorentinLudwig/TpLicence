#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>
#include <string.h>


void erreur(const char* msg, int code)
{
    perror(msg);
    exit(code);
}


int f_vide(char *nom_repertoir){
    DIR *dir;
    int nb=0;
    struct stat Infos;
    dir = opendir(nom_repertoir);
    if(dir!=NULL){
        struct dirent *fich;
        char dest[120];
        while((fich = readdir(dir))!=NULL){
                strcpy(dest,nom_repertoir);
                strcat(dest,"/");
                strcat(dest,fich->d_name);
            if(stat(dest, &Infos)){       
                        erreur(fich->d_name,3);
                    }
                    if(S_ISREG(Infos.st_mode) && Infos.st_size == 0) {
                        nb++;
                    }
                    if(S_ISDIR(Infos.st_mode) && strcmp(".",fich->d_name) && strcmp("..",fich->d_name)) {
                        nb +=f_vide(fich->d_name);
                    }
                } 
                closedir(dir);
                
                
            }
    return nb;
}


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage : %s nom_rep\n", argv[0]);
        exit(1);
    }
    printf("%d \n",f_vide(argv[1]));
}