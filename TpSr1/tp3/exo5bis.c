#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>

#define TAILLEMAXDES 512

void Affiche_inode(DIR *rep, struct stat *Infos, struct dirent Element)
{    
    struct passwd *pws;
    pws = getpwuid(geteuid());
    if (S_ISDIR(Infos->st_mode))
        printf("répertoire ");
    else if (S_ISLNK(Infos->st_mode))   
        printf("lien symbolique ");
    else
        printf("fichier ordinaire ");
    printf("%8ld octets %s %s\n", Infos->st_size, pws->pw_name, ctime(&Infos->st_atime));   
    
}

void ListRep(DIR *rep, struct stat *Infos, struct dirent *Element)
{
    while((Element = readdir(rep)) != NULL){
        if(lstat(Element->d_name, Infos) == -1){
            perror(Element->d_name);
            exit(2);
        }

        printf("./%-20s: ", Element->d_name);
        Affiche_inode(rep, Infos, *Element);
    } 
}   

int main(int argc, char** argv)
{
    DIR *rep;
    struct dirent *Element;
    struct stat Infos;

    if (argc < 2){
        perror("Il faut un argument minimum");
        exit(1);
    }

    if (argc == 2){
        if(lstat(argv[1], &Infos) == -1){
            perror(argv[1]);
            exit(2);
        }
        if (S_ISDIR(Infos.st_mode)){
            rep = opendir(argv[1]);
            ListRep(rep, &Infos, Element);
        } 
        else if (S_ISREG(Infos.st_mode)){
            printf("%-20s: ", argv[1]);
            Affiche_inode(rep, &Infos, *Element);
        }        
    }
    else {
        for (int i = 1 ; i < argc ; i++){
            if (lstat(argv[i], &Infos) == -1){
            perror(argv[i]);
            exit(1);
            }
            printf("%-20s : ", argv[i]);
            Affiche_inode(rep, &Infos, *Element);
        } 
    }
  
    return 0;
}