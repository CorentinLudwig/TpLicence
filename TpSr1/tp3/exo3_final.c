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


void Affiche_inode(struct stat * Infos){
    if(S_ISREG(Infos->st_mode)!=0){
        printf("fichier ordinaire");
    }
    else if(S_ISDIR(Infos->st_mode)!=0){
        printf("repertoire");
    }
    else if(S_ISFIFO(Infos->st_mode)!=0){
        printf("fichier spécial tube");
    }
    else if(S_ISCHR(Infos->st_mode)!=0){
        printf("périphérique caractère");
    }
    else if(S_ISBLK(Infos->st_mode)!=0){
        printf("périphérique bloc");
    }
    else if(S_ISLNK(Infos->st_mode)!=0){
        printf("lien symbolique");
    }
    else if(S_ISSOCK(Infos->st_mode)!=0){
        printf("socket");
    }


    printf("%10ld octets",Infos->st_size);
    printf(" %s", ctime(&Infos->st_mtime));
}



int main(int argc, char *argv[]){
    struct stat Infos;
    int t,i;
    if(argc<2){
        fprintf(stderr,"%s nom_fichier\n",argv[0]);
        exit(1);
    }

    if(argc==2){
        DIR *dir;
        dir = opendir(argv[1]);
        if(dir!=NULL){ 
            struct dirent *fich;
            while((fich = readdir(dir))!=NULL){
                char dest[120];
                strcpy(dest,argv[1]);
                strcat(dest,"/");
                strcat(dest,fich->d_name);

                t=lstat(dest,&Infos);
                if(t==-1){
                    perror("lstat");
                    exit(2);
                }
                Affiche_inode(&Infos);
            }
        }
        else{
            t=lstat(argv[1],&Infos);
            if(t==-1){
                perror("lstat");
                exit(2);
            }
            Affiche_inode(&Infos);
            }

    }
    else{
        for(i=1;i<argc;i++){
        t=lstat(argv[i],&Infos);
        if(t==-1){
            perror("lstat");
            exit(2);
        }
        Affiche_inode(&Infos);
        }
    }
    exit(0);
}