#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

typedef struct s_info
{
    int id;
    int nbl;
    int nnm;
} Info;

pthread_mutex_t mutex_affichage;

void *thread_fonction(void *arg)
{

    Info *info = (Info *)arg;

    for (int i = 0; i < info->nnm; i++)
    {
        pthread_mutex_lock(&mutex_affichage);
        for (int j = 0; j < info->nbl; j++)
        {
            printf("Afficheur %d (%ld), j'affiche ligne %d/%d du message %d/%d\n",
                   info->id, pthread_self(), j, info->nbl, i, info->nnm);
        }
        pthread_mutex_unlock(&mutex_affichage);
    }

    printf("Afficheur %d (%ld), je me termine\n", info->id, pthread_self());

    return NULL;
}

int main(int argc, char const *argv[])
{

    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <num_threads> <num_iterations>\n", argv[0]);
        exit(0);
    }
    int NBT, NBM, NBL;

    NBT = atoi(argv[1]);
    NBM = atoi(argv[2]);
    NBL = atoi(argv[3]);

    pthread_mutex_init(&mutex_affichage, NULL);

    pthread_t pthread[NBT];

    for (int i = 0; i < NBT; i++)
    {
        Info *info = malloc(sizeof(Info));
        info->nbl = NBL;
        info->nnm = NBM;
        info->id = i;
        if (pthread_create(&pthread[i], NULL, thread_fonction, (void *)info) == -1)
        {
            perror("thread_create");
            exit(0);
        }
    }

    for (int i = 0; i < NBT; i++)
    {
        pthread_join(pthread[i], NULL);
    }

    return 0;
}
