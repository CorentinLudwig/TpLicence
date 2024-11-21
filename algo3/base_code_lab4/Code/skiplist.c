#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "skiplist.h"
#include "rng.h"


typedef struct s_Node {
    int value;
	int level;
    struct s_Node **forward;
	struct s_Node **backward;
} Node;

struct s_SkipList {
    int level;
    unsigned int size;
	RNG rng;
    struct s_Node *sentinel;
};

struct s_SkipListIterator {
	struct s_SkipList list;
	Node *begin;
    Node *current;
    Node (*fnext)(Node*);
	
};

SkipList skiplist_create(int nblevels) {
    SkipList list;
    list = (SkipList)malloc(sizeof(struct s_SkipList));
    int i;
    Node *sentinel = (Node *) malloc(sizeof(Node));
    list->sentinel = sentinel;
    list->sentinel->value = -1;
    list->sentinel->level = nblevels;
    sentinel->forward = (Node **) malloc(sizeof(Node*) * (nblevels));
    sentinel->backward = (Node **) malloc(sizeof(Node*) * (nblevels));
    for (i = 0; i < nblevels; i++) {
        sentinel->forward[i] = list->sentinel;
        sentinel->backward[i] = list->sentinel;
    }
    list->level = nblevels;
    list->size = 0;
    list->rng = rng_initialize(0);
    return list;
}


void skiplist_delete(SkipList d) {
    Node *current_node = d->sentinel->forward[1];
    while(current_node != d->sentinel) {
        Node *next_node = current_node->forward[1];
        free(current_node->forward);
        free(current_node);
        current_node = next_node;
    }
    free(current_node->forward);
    free(current_node);
    free(d);
}

unsigned int skiplist_size(SkipList d){
	return d->size;
}

SkipList skiplist_insert(SkipList d, int value){
    int i;
    d->sentinel->value = value;
    Node *current = d->sentinel;
    Node *update[d->level];
    for (i = (d->sentinel->level)-1; i >= 0; i--)
    {
        while(value > current->forward[i]->value)
            current = current -> forward[i];
        update[i] = current;
    }
    if(current->forward[0]->value == value && current->forward[0] != d->sentinel){
        return d;
    }
    Node *new = malloc(sizeof(struct s_SkipList));
    new->level = rng_get_value(&(d->rng), (d->level)-1)+1;
    new->backward = malloc(sizeof(Node*) * (new->level));
    new->forward = malloc(sizeof(Node*) * (new->level));
    new->value = value;
    for (int j = 0; j < new->level; j++)
    {
        new->backward[j] = update[j];
        new->forward[j] = update[j]->forward[j];
        update[j]->forward[j]->backward[j] = new;
        update[j]->forward[j] = new;
        
    }

    d->size++;
    return d;
}

int skiplist_at(SkipList d, unsigned int i){
  assert((0 <= i) && (i < d->size));
  Node *e = d->sentinel->forward[0];
  while (i--)
    e = e->forward[0];
  return e->value;
}

void skiplist_map(SkipList d, ScanOperator f, void *user_data){
  for (Node *e = d->sentinel->forward[0]; e != d->sentinel; e = e->forward[0]) {
    f(e->value,user_data);
  }
}



bool skiplist_search(SkipList d, int value, unsigned int *nb_operations){
	*nb_operations = 0;
    Node *current = d->sentinel;
    d->sentinel->value = value;
    (*nb_operations)++;
    for(int i = (d->level)-1;i >= 0; --i){
        while(value > current->forward[i]->value){
            current = current->forward[i];
            (*nb_operations)++;
            }
    
        if((current->forward[i]->value == value) && (current->forward[i] != d->sentinel))
            return true;
    }
    return false;

}


SkipList skiplist_remove(SkipList d, int value){
	int i;
    d->sentinel->value = value;
    Node *current = d->sentinel;
    Node *update[d->level];
    for (i = (d->sentinel->level)-1; i >= 0; i--)
    {
        while(value > current->forward[i]->value)
            current = current -> forward[i];
        update[i] = current;
    }
    if(current->forward[0]->value == value && current->forward[0] != d->sentinel){
		current=current->forward[0];
		for (int j = 0; j < current->level; j++)
		{
			current->forward[j]->backward[j] = update[j];
			update[j]->forward[j] = current->forward[j];
			
		}
		free(current);
	}
	return d;
}


// SkipListIterator skiplist_iterator_create(SkipList d, unsigned char w){
// 	SkipListIterator ite = malloc(sizeof(struct s_SkipListIterator));
// 	ite->list = *d;
// 	ite->begin = *d->sentinel->forward;
//     ite->current = ite->begin;
//     ite->fnext = w;
// 	return ite;
// }