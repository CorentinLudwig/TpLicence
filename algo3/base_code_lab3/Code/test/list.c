/*-----------------------------------------------------------------*/
/*
 Licence Informatique - Structures de données
 Mathias Paulin (Mathias.Paulin@irit.fr)
 
 Implantation du TAD List vu en cours.
 */
/*-----------------------------------------------------------------*/
#include <stdio.h>

#include <stdlib.h>
#include <assert.h>

#include "list.h"

typedef struct s_LinkedElement {
	int value;
	struct s_LinkedElement *previous;
	struct s_LinkedElement *next;
} LinkedElement;

/* Use of a sentinel for implementing the list :
 The sentinel is a LinkedElement * whose next pointer refer always to the head of the list and previous pointer to the tail of the list
 */
struct s_List {
	LinkedElement *sentinel;
	int size;
};

typedef struct s_subList *SubList;
struct s_subList {

	LinkedElement *tete;
	LinkedElement *queue;
	
};


/*-----------------------------------------------------------------*/

List *list_create() {
	List *l = malloc(sizeof(struct s_List));
	l->sentinel = malloc(sizeof(LinkedElement));
	l->sentinel->previous = l->sentinel->next = l->sentinel;
	l->size = 0;

	return l;
}

/*-----------------------------------------------------------------*/

List *list_push_back(List *l, int v) {
	LinkedElement *e = malloc(sizeof(LinkedElement));
	e -> value = v;
	e -> next = l -> sentinel;
	e -> previous = e -> next -> previous;
	e -> previous -> next = e;
	e -> next -> previous = e;
	++(l -> size);
	

	return l;
}

/*-----------------------------------------------------------------*/

void list_delete(ptrList *l) {
	

	for(LinkedElement *e = (*l) -> sentinel -> next; e != (*l) -> sentinel; e = e -> next)
	{
		free(e);
	}
}

/*-----------------------------------------------------------------*/

List *list_push_front(List *l, int v) {
	LinkedElement *e = malloc(sizeof(LinkedElement));
	e->value = v;
	e->next = l -> sentinel -> next;
	e->previous = l->sentinel;
	e->previous->next = e;
	e->next->previous = e;
	++(l->size);
	
		
	
	return l;
}

/*-----------------------------------------------------------------*/

int list_front(List *l) {
	
	return (l -> sentinel -> next) -> value;
}

/*-----------------------------------------------------------------*/

int list_back(List *l) {
	
	return l -> sentinel -> previous -> value;
}

/*-----------------------------------------------------------------*/

List *list_pop_front(List *l) {
	assert(!list_is_empty(l));
	LinkedElement *e;
	e = l->sentinel->next;
	l->sentinel->next = e->next;
	l->sentinel->next->previous = l->sentinel;
	--(l->size);
	free(e);
	return l;
}

/*-----------------------------------------------------------------*/

List *list_pop_back(List *l){
	assert(!list_is_empty(l));
	LinkedElement *e;
	e = l->sentinel->previous;
	l->sentinel->previous = e->previous;
	l->sentinel->previous->next = l->sentinel;
	--(l->size);
	free(e);
	return l;
}

/*-----------------------------------------------------------------*/

List *list_insert_at(List *l, int p, int v) {
	
	assert((0 <= p) && (p <= l->size));
	LinkedElement *insert = l->sentinel->next;
	while (p--) insert = insert->next;
	LinkedElement *e = malloc(sizeof(LinkedElement));
	e->value = v;
	e->next = insert;
	e->previous = insert->previous;
	e->previous->next = e;
	e->next->previous = e;
	++(l->size);
	return l;

	
}

/*-----------------------------------------------------------------*/

List *list_remove_at(List *l, int p) {
	assert((0 <= p) && (p < l->size));
	LinkedElement *remove = l->sentinel->next;
	while (p--) remove = remove->next;
	remove->previous->next = remove->next;
	remove->next->previous = remove->previous;
	free(remove);
	--(l->size);
	
	return l;
}

/*-----------------------------------------------------------------*/

int list_at(List *l, int p) {
	assert((0 <= p) && (p < l->size));
	LinkedElement *e = l->sentinel->next;
	while (p--) e = e->next;
	


	return e->value;
}

/*-----------------------------------------------------------------*/

bool list_is_empty(List *l) {
	
	return (l -> size) == 0;
}

/*-----------------------------------------------------------------*/

int list_size(List *l) {
	

	return l -> size;
}

/*-----------------------------------------------------------------*/

List * list_map(List *l, SimpleFunctor f) {
	for(LinkedElement *e = l -> sentinel -> next; e != l -> sentinel; e = e -> next)
	{
		e -> value = f(e -> value);
	}
	return l;
}


List *list_reduce(List *l, ReduceFunctor f, void *userData) {
	for (LinkedElement *e = l->sentinel->next; e != l->sentinel; e = e->next)
		f(e->value, userData);
	return l;
}

/*-----------------------------------------------------------------*/



/*-----------------------------------------------------------------*/

SubList list_split(SubList l)
{
	SubList sub =  malloc(sizeof(struct s_subList));
	LinkedElement *head = l->tete;
	LinkedElement *tail = l->queue;
	
	if(head->next == tail)
	{
		sub->tete = head;
		sub->queue = tail;
		head->next = NULL;
		tail->previous = NULL;
		head->previous = NULL;
		tail->next = NULL;
		return sub;

	}
	while(head != tail && head->next != tail)
	{
		head = head->next;
		tail = tail->previous;
	}
	if(head == tail)
	{
		head = head->previous;
	}
	sub->tete = head;
	sub->queue = tail;
	head->next = NULL;
	tail->previous = NULL;
	
	return sub;
}

/*-----------------------------------------------------------------*/

SubList list_merge(SubList leftlist, SubList rightlist,OrderFunctor f)
{
    SubList merged =  malloc(sizeof(struct s_subList));
    LinkedElement *Lhead = leftlist->tete;
    LinkedElement *Rhead = rightlist->tete;
    if(f(Lhead->value,Rhead->value))
        {
            merged->tete = merged->queue = Lhead; 
            Lhead = Lhead->next;
        } else {    
            merged->tete = merged->queue = Rhead;
            Rhead = Rhead->next;
		}
    merged->tete->next = merged->tete->previous = NULL;
    while(Lhead || Rhead) 
    {
    LinkedElement * elt;
    if(Lhead && (!Rhead || f(Lhead->value,Rhead->value))) {
        	elt = Lhead;
        	Lhead = Lhead->next;
        } 
		else {
        	elt = Rhead;
        	Rhead = Rhead->next;
        }
    merged->queue->next = elt;
    elt->previous = merged->queue;
    merged->queue = elt;
    }
    merged->queue->next = NULL;
	merged->tete->previous = NULL;
    
    return merged;
}

/*-----------------------------------------------------------------*/

SubList list_mergesort(SubList l, OrderFunctor f)
{
	if(l->tete->next == NULL)
		return l;
	SubList left =  malloc(sizeof(struct s_subList));
	SubList right =  malloc(sizeof(struct s_subList));
	left->tete = l->tete;
	right->queue = l->queue;
	SubList merged = list_split(l);
	left->queue = merged->tete;
	right->tete = merged->queue;
	
	left = list_mergesort(left, f);
	
	right = list_mergesort(right, f);
	

	return list_merge(right, left, f);
}

/*-----------------------------------------------------------------*/

List *list_sort(List *l, OrderFunctor f)
{
	SubList sub = malloc(sizeof(struct s_subList));
	sub->tete = l->sentinel->next;
	sub->queue = l->sentinel->previous;
	
	

	sub = list_mergesort(sub, f);

	sub->tete->previous = l->sentinel;
	sub->queue->next = l->sentinel;
	l->sentinel->next = sub->tete;
	l->sentinel->previous = sub->queue;

	return l;
}

/*-----------------------------------------------------------------*/