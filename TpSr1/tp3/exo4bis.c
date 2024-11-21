#define _POSIX_C_SOURCE 200809L

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>


void erreur(const char* msg, int code)
{
    perror(msg);
    exit(code);
}

int fichierVides(char *nom_rep)
{
    DIR *rep = opendir(nom_rep);
    
    if(rep == NULL)
    {

        erreur("nom repertoire", 2);
    }
    int compteur = 0;
    struct dirent* element;
    
    char dest[120];
    while((element = readdir(rep)) != NULL)
    {
        struct stat inode;
        strcpy(dest,nom_rep);
        strcat(dest,"/");
        strcat(dest,element->d_name);
        
        if(stat(dest, &inode))
        {       
            erreur(element->d_name,3);
        }
        if(S_ISREG(inode.st_mode) && inode.st_size == 0)
        {
            compteur++;
        }
        if(S_ISDIR(inode.st_mode) && strcmp(".",element->d_name) && strcmp("..",element->d_name))
        {
            
            compteur +=fichierVides(element->d_name);
        }
    } 
    closedir(rep);


    return compteur;
    
    
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage : %s nom_rep\n", argv[0]);
        exit(1);
    }
    

    printf("%d \n",fichierVides(argv[1]));
    





}