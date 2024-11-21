#include "bstree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>


#include "queue.h"


void bstree_remove_node(ptrBinarySearchTree *t, ptrBinarySearchTree current);
typedef enum{
red,black
}NodeColor;

ptrBinarySearchTree fixredblack_insert(ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_insert_case0(ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_insert_case1(ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_insert_case2(ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_insert_case2_right(ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_insert_case2_left(ptrBinarySearchTree x);

ptrBinarySearchTree fixredblack_remove(ptrBinarySearchTree p, ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_remove_case1(ptrBinarySearchTree p, ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_remove_case1_left(ptrBinarySearchTree p, ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_remove_case1_right(ptrBinarySearchTree p, ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_remove_case2(ptrBinarySearchTree p, ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_remove_case2_left(ptrBinarySearchTree p, ptrBinarySearchTree x);
ptrBinarySearchTree fixredblack_remove_case2_right(ptrBinarySearchTree p, ptrBinarySearchTree x);

/*=========================================================================*/
/*------------------------  BSTreeType  -----------------------------*/

struct _bstree {
    BinarySearchTree *parent;
    BinarySearchTree *left;
    BinarySearchTree *right;
    NodeColor color;
    int root;
};

/*------------------------  BaseBSTree  -----------------------------*/

BinarySearchTree *bstree_create() {
    return NULL;
}

/* This constructor is private so that we can maintain the oredring invariant on
 * nodes. The only way to add nodes to the tree is with the bstree_add function
 * that ensures the invariant.
 */
BinarySearchTree *bstree_cons(BinarySearchTree *left, BinarySearchTree *right, int root) {
    BinarySearchTree *t = malloc(sizeof(struct _bstree));
    t->parent = NULL;
    t->color=red;
    t->left = left;
    t->right = right;
    if (t->left != NULL)
        t->left->parent = t;
    if (t->right != NULL)
        t->right->parent = t;
    t->root = root;
    return t;
}
void bstree_delete_node(const BinarySearchTree *t,void *data);

void bstree_delete(ptrBinarySearchTree *t) {
    bstree_depth_postfix(*t,bstree_delete_node,NULL);
}

void bstree_delete_node(const BinarySearchTree *t, void *data){
   (void)data;
    free((BinarySearchTree*)t);
}

bool bstree_empty(const BinarySearchTree *t) {
    return t == NULL;
}

int bstree_root(const BinarySearchTree *t) {
    assert(!bstree_empty(t));
    return t->root;
}

BinarySearchTree *bstree_left(const BinarySearchTree *t) {
    assert(!bstree_empty(t));
    return t->left;
}

BinarySearchTree *bstree_right(const BinarySearchTree *t) {
    assert(!bstree_empty(t));
    return t->right;
}

BinarySearchTree *bstree_parent(const BinarySearchTree *t) {
    assert(!bstree_empty(t));
    return t->parent;
}

bool is_black(BinarySearchTree * x){
    return (!x || x->color==black);
}

bool is_red(BinarySearchTree * x){
    return (x && x->color==red);
}
/*------------------------  BSTreeDictionary  -----------------------------*/
/*#################"Fonctions Pour le debugage###################"*/
void print_treefun(const BinarySearchTree *t, void *userData) {
    (void) userData;
    printf("%d ", bstree_root(t));
}
/*################################################################*/
/* Obligation de passer l'arbre par référence pour pouvoir le modifier */
void bstree_add(ptrBinarySearchTree *t, int v) {
    ptrBinarySearchTree cur = *t;
    BinarySearchTree *par = NULL;

    while(cur){
        par=cur;
        if(cur->root==v)return;
        cur =((cur->root>v)?(cur->left):(cur->right));
    }

    cur=bstree_cons(NULL,NULL,v);

    if(par) (par->root > v) ? (par->left = cur) : (par->right = cur);
    else *t = cur;
    cur->parent=par;


    if(cur->parent)
        fixredblack_insert(cur);
    while(*t && (*t)->parent !=NULL) *t = (*t)->parent;


    return;
}

/*-------------------------------------------------------------------------*/

bool bstree_search(const BinarySearchTree *t, int v) {
    BinarySearchTree *cur= (BinarySearchTree *)t;
    while(!bstree_empty(cur)){
        if(v<cur->root) cur=cur->left;
        else if(v>cur->root) cur=cur->right;
        else return true;
    }
    return false;
}

/*------------------------------------------------------------------------*/
BinarySearchTree *bstree_successor(const BinarySearchTree *x) {
    assert(!bstree_empty(x));
    BinarySearchTree *cur=(BinarySearchTree*)x;
    BinarySearchTree *y=x->right;
    
    if(y){
        while(y->left) y=y->left;
    }else{
        y=cur->parent;
        while(y && cur==y->right)
        {
            cur=y;
            y=y->parent;
        }
    }
    return y;
}

/*-----------------------------------------------------------------------*/
BinarySearchTree *bstree_predecessor(const BinarySearchTree *x) {
    assert(!bstree_empty(x));
    BinarySearchTree *cur=(BinarySearchTree*)x;
    BinarySearchTree *y=x->left;
    if(y){
        while(y->right) y=y->right;
    }else{
        y=cur->parent;
        while(y && cur==y->left)
        {
            cur=y;
            y=y->parent;
        }
    }
    return y;
}
/*-------------------------------------------------------------------------*/
void bstree_swap_nodes(ptrBinarySearchTree *tree, ptrBinarySearchTree from, ptrBinarySearchTree to){
    assert(!bstree_empty(*tree) && !bstree_empty(from) && !bstree_empty(to));

    int reserve;
    NodeColor couleur=to->color;
    to->color=from->color;
    from->color=couleur;

    reserve=to->root;
    to->root=from->root;
    from->root=reserve;
    tree=tree;
}
/*--------------------------------------------------------------------------*/
// t -> the tree to remove from, current -> the node to remove
void bstree_remove_node(ptrBinarySearchTree *t, ptrBinarySearchTree current) {
    assert(!bstree_empty(*t) && !bstree_empty(current));
    ptrBinarySearchTree *m;
    ptrBinarySearchTree substitute;

printf("\n>>Noeud a éffacer: %d<<     [%p]\n",current->root, (void*)current);
current->color==black?printf(">>>>Couleur: black\n"):printf(">>>>Couleur: red\n");
int numero=current->root;
    NodeColor Ccur= current->color;
    ptrBinarySearchTree p= bstree_parent(current);
    if(!current->parent)
    {
        m=(ptrBinarySearchTree*)t;
    }
    else if(current->parent->left==current)
        m=&(current->parent->left);
    else
        m=&(current->parent->right);

    if(!current->left&&!current->right)
    {
        substitute=NULL;
        *m= NULL;
        free(current);
    }
    else if(!current->left)
    {
        substitute=current->right;
        *m= current->right;
        current->right->parent =current->parent;
        free(current);
    }
    else if (!current->right)
    {
        substitute=current->right;
        *m=current->left;
        current->left->parent = current->parent;
        free(current);
    }
    else if(current->left && current->right)
    {
        BinarySearchTree *l;
        l=bstree_successor(current);
        if(l)
        {
            substitute=current;
            current->root=l->root;
            bstree_swap_nodes(t,current,l);
            bstree_remove_node(t,l);
        }
    }

    if(Ccur==black){;
printf(">> Current: %d<<\n",numero);
        if((!substitute) || (substitute->color==black)){
            printf("IF: OK\n");
            ptrBinarySearchTree subtreeroot = fixredblack_remove(p,substitute);
            if(!subtreeroot->parent)
                *t=subtreeroot;
            else{
                if(substitute)substitute->color=black;
            }
        }
    }

printf("\n##################NEW TREE####################\n");
    return;
}
/*------------------------------------------------------------------------*/
void bstree_remove(ptrBinarySearchTree *t, int v) {
ptrBinarySearchTree node=(BinarySearchTree*)*t;

    while(node)
    {
        node = ((node->root>v)? node->left:node->right);

        if(node && node->root==v)
        {
            bstree_remove_node(t,node);
            return;
        }
    }
}
/*------------------------  BSTreeVisitors  -----------------------------*/
void bstree_depth_prefix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    if(bstree_empty(t))
        return;
    f(t,userData);
    bstree_depth_prefix(t->left,f,userData);
    bstree_depth_prefix(t->right,f,userData);
}
/*======================================================================*/
void bstree_depth_infix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    if(bstree_empty(t)) 
        return;
    bstree_depth_infix(t->left,f,userData);
    f(t,userData);
    bstree_depth_infix(t->right,f,userData);
}
/*======================================================================*/
void bstree_depth_postfix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    if(bstree_empty(t))
        return;
// printf("\n<>--- %d [%p] %p %p %p ---<>\n", t->root,(void*)t, (void*)t->parent, (void*)t->left, (void*)t->right);
    bstree_depth_postfix(t->left,f,userData);
    bstree_depth_postfix(t->right,f,userData);
    f(t,userData);
}
/*======================================================================*/
void bstree_iterative_depth_infix(const BinarySearchTree *t, OperateFunctor f, void *userData){
    BinarySearchTree *cur =(BinarySearchTree*)t;
    BinarySearchTree *next=t->parent;
    BinarySearchTree *prev=t->parent;

    while(!bstree_empty(cur)){
        if(prev==cur->parent){
            prev=cur;
            next=cur->left;
        }
        if(bstree_empty(next)||prev==cur->left){
            f(cur,userData);
            prev=cur;
            next=cur->right;
        }
        if(bstree_empty(next)||prev==cur->right){
            prev=cur;
            next=cur->parent;
        }
        cur=next;
    }
}
/*====================================================================*/
void bstree_iterative_breadth_prefix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
    Queue *q=createQueue();
    BinarySearchTree *cur=(BinarySearchTree*)t;
    queuePush(q,cur);
    while(!queueEmpty(q))
    {    
        f(queueTop(q),userData);
        cur=(BinarySearchTree*)queueTop(q);
        queuePop(q);
        if(!bstree_empty(cur->left))
        {    
            queuePush(q,cur->left);
        }
        if(!bstree_empty(cur->right))
        {
            queuePush(q,cur->right);
        }
    }
    deleteQueue(&q);
}

/*------------------------  BSTreeIterator  -----------------------------*/

struct _BSTreeIterator {
    /* the collection the iterator is attached to */
    const BinarySearchTree *collection;
    /* the first element according to the iterator direction */
    const BinarySearchTree *(*begin)(const BinarySearchTree *);
    /* the current element pointed by the iterator */
    const BinarySearchTree *current;
    /* function that goes to the next element according to the iterator direction */
    BinarySearchTree *(*next)(const BinarySearchTree *);
};

/* minimum element of the collection */
const BinarySearchTree *goto_min(const BinarySearchTree *e){
    while(e->left)e=e->left;
	return e;
}

/* maximum element of the collection */
const BinarySearchTree *goto_max(const BinarySearchTree *e) {
	while(e->right)e=e->right;
	return e;
}

/* constructor */
BSTreeIterator *bstree_iterator_create(const BinarySearchTree *collection, IteratorDirection direction) {
    BSTreeIterator *iterator=malloc(sizeof(BSTreeIterator));

    iterator->collection=collection;
    
    if(direction==backward)
    {
        iterator->begin=goto_max;
        iterator->next=bstree_predecessor;
    }
    else
    {
        iterator->begin=goto_min;
        iterator->next=bstree_successor;
    }
    iterator->current=NULL;
	
    return iterator;
}

/* destructor */
void bstree_iterator_delete(ptrBSTreeIterator *i) {
    free(*i);
    *i = NULL;
}

BSTreeIterator *bstree_iterator_begin(BSTreeIterator *i) {
    i->current = i->begin(i->collection);
    return i;
}

bool bstree_iterator_end(const BSTreeIterator *i) {
    return i->current == NULL;
}

BSTreeIterator *bstree_iterator_next(BSTreeIterator *i) {
    i->current = i->next(i->current);
    return i;
}

const BinarySearchTree *bstree_iterator_value(const BSTreeIterator *i) {
    return i->current;
}
/*---------------------------  RBTSpecific  -------------------------------*/

void leftrotate(BinarySearchTree *x){
    BinarySearchTree *buffer= malloc(sizeof(BinarySearchTree*));
    
    if(x->right)
    {
        buffer=x->right;
        x->right=x->right->left;
        if(x->right) x->right->parent=x;
        buffer->left=x;
        buffer->parent=x->parent;
        if(x->parent)
        {
            if(x==x->parent->right)
                x->parent->right=buffer;
            else
                x->parent->left=buffer;
        }
        x->parent=buffer;
    }
    else
    {
        printf("\n Pas d'enfant disponible");
    }
}

/*======================================================================================================*/

void rightrotate(BinarySearchTree* x){
    BinarySearchTree *buffer= malloc(sizeof(BinarySearchTree*));
    if(x && x->left)
    {
        buffer=x->left;
        x->left=x->left->right;
        if(x->left) x->left->parent=x;
        
        buffer->right=x;
        buffer->parent=x->parent;
        if(x->parent)
        {
            if(x==x->parent->right)
                x->parent->right=buffer;
            else
                x->parent->left=buffer;
        }     
        x->parent=buffer;
    }
    else
    {
        printf("\n Pas d'enfant disponible");
    }
}
/*======================================================================================================*/
ptrBinarySearchTree grandparent(ptrBinarySearchTree t){
    assert(!bstree_empty(t));
    if(bstree_parent(t))
        return t->parent->parent;
    return NULL; 
}
/*=======================================================================================================*/
ptrBinarySearchTree uncle(ptrBinarySearchTree t){
    assert(!bstree_empty(t));
    ptrBinarySearchTree grandpa=grandparent(t);
    if(grandpa && t->parent== grandpa->right) return grandpa->left;
    if(grandpa && t->parent==grandpa->left) return grandpa->right;
    return NULL;
}
/*=======================================================================================================*/
ptrBinarySearchTree frere(ptrBinarySearchTree t, ptrBinarySearchTree p){
    if(p && t==p->left)return p->right;
    if(p && t== p->right)return p->left;
    return NULL;
    
}
/*=======================================================================================================*/
ptrBinarySearchTree fixredblack_insert_case2_left(ptrBinarySearchTree x){
    if(grandparent(x) && x==bstree_parent(x)->left)
    {
printf("in the 'IF'\n");
        bstree_parent(x)->color=black;
        grandparent(x)->color=red;
        rightrotate(grandparent(x));
        return bstree_parent(x);
    }
    else
    {
printf("in the 'ELSE'\n");
        x->color=black;
        grandparent(x)->color=red;
        leftrotate(bstree_parent(x));
        rightrotate(bstree_parent(x));
        return x;
    }
}
/*=======================================================================================================*/
ptrBinarySearchTree fixredblack_insert_case2_right(ptrBinarySearchTree x){
printf("\n             <> fixredblack_insert_case2_right informatin <>\n");
printf(">>x: %d<<        [%p]\n",x->root,(void*)x);
    
    if(grandparent(x) && x==bstree_parent(x)->right)
    {
printf("in the 'IF'\n");
        bstree_parent(x)->color=black;
        grandparent(x)->color=red;
        leftrotate(grandparent(x));
        return x;
    }
    else
    {
printf("in the 'ELSE'\n");
        x->color=black;
        grandparent(x)->color=red;
        rightrotate(bstree_parent(x));
        leftrotate(bstree_parent(x));
        return x;
    }
}
/*=======================================================================================================*/
ptrBinarySearchTree fixredblack_insert_case2(ptrBinarySearchTree x){
    if(bstree_parent(x) && grandparent(x) && bstree_parent(x)==grandparent(x)->left)
    {
        return fixredblack_insert_case2_left(x);

    }
    return fixredblack_insert_case2_right(x);
}
/*=======================================================================================================*/
ptrBinarySearchTree fixredblack_insert_case1(ptrBinarySearchTree x){
printf("\n          <> fixredblack_insert_case1 informatin <>\n");
printf(">>x: %d<<        [%p]\n",x->root,(void*)x);

    if(uncle(x) && uncle(x)->color==red)
    {
        uncle(x)->color=black;
        bstree_parent(x)->color=black;
        grandparent(x)->color=red;
        return fixredblack_insert(grandparent(x));
    }
    return fixredblack_insert_case2(x);
}
/*=======================================================================================================*/
ptrBinarySearchTree fixredblack_insert_case0(ptrBinarySearchTree x){
    if(grandparent(x))
        return fixredblack_insert_case1(x);
    else 
        bstree_parent(x)->color=black;

    return x;
}

/*=======================================================================================================*/
ptrBinarySearchTree fixredblack_insert(ptrBinarySearchTree x){
    if(bstree_parent(x) && bstree_parent(x)->color==red)
    {
        return fixredblack_insert_case0(x);
    }
    return x;
}
/*=======================================================================================================*/
void testrotateleft ( BinarySearchTree * t) {
leftrotate (t );
}
/*=======================================================================================================*/
void testrotateright ( BinarySearchTree *t ) {
rightrotate (t) ;
}
/*--------------------RTB remove ---------------------------------*/

/*----------------------------------------------------------------*/
ptrBinarySearchTree fixredblack_remove(ptrBinarySearchTree p, ptrBinarySearchTree x){
    
    ptrBinarySearchTree f= frere(x,p);
    if(!p)
    {
     x->color=black;   
    }
    if(is_black(f))
    {
        return fixredblack_remove_case1(p,x);
    }
    else
    {
        return fixredblack_remove_case2(p,x);
    }
    
}
/*=======================================================================================================*/
ptrBinarySearchTree fixredblack_remove_case1(ptrBinarySearchTree p, ptrBinarySearchTree x){
    if(x==p->left)
        return fixredblack_remove_case1_left(p,x);
    else
        return fixredblack_remove_case1_right(p,x);
}
/*=======================================================================================================*/
ptrBinarySearchTree fixredblack_remove_case1_left(ptrBinarySearchTree p, ptrBinarySearchTree x){
printf("\n         <> fixredblack_remove_case1_left informatin <>\n");    

    ptrBinarySearchTree f= frere(x,p);
    ptrBinarySearchTree d= NULL;
    if(f)d=f->right;
    ptrBinarySearchTree g=NULL;
    if(f)g= f->left;
    
    if(f && (is_black(g)) && (is_black(d)))
    {
        x->color=black;
        if(f)f->color=red;
        if(p && p->color==red)
        {
            p->color=black;
            return p;
        }
        return fixredblack_remove(grandparent(x),p);
    }
    else if(is_red(d)) 
    {
        leftrotate(p);
        f->color=p->color;
        x->color=black;
        p->color=black;
        d->color=black;
    }
    else if(is_black(d) && is_red(g))
    {
        rightrotate(f);
        g->color=black;
        f->color=red;
        leftrotate(p);
        f->color=p->color;
    }
    return p;
}
/*=======================================================================================================*/
ptrBinarySearchTree fixredblack_remove_case1_right(ptrBinarySearchTree p, ptrBinarySearchTree x){
    printf("\n         <> fixredblack_remove_case1_right informatin <>\n");    

    ptrBinarySearchTree f= frere(x,p);
    ptrBinarySearchTree d= NULL;
    if(f)d=f->right;
    ptrBinarySearchTree g=NULL;
    if(f)g= f->left;

    if(((!f) ||( is_black(g)&& is_black(d))))
    {
        if(x)x->color=black;
        if(f)f->color=red;
        if(p && p->color==red)
        {
            p->color=black;
            return p;
        }
        return fixredblack_remove(grandparent(x),p);
    }
    else if(is_red(g)) 
    {
        rightrotate(p);
        f->color=p->color;
        x->color=black;
        p->color=black;
        g->color=black;
    }
    else if( is_black(g) && is_red(d))
    {
        leftrotate(f);
        d->color=black;
        f->color=red;
        rightrotate(p);
        f->color=p->color;
    }
    return p;
}
/*=======================================================================================================*/
ptrBinarySearchTree fixredblack_remove_case2(ptrBinarySearchTree p, ptrBinarySearchTree x){
    if(x==p->right)
    {
        return fixredblack_remove_case2_right(p,x);
    }
    else
    {
        return fixredblack_remove_case2_left(p,x);

    }
}
/*=======================================================================================================*/
ptrBinarySearchTree fixredblack_remove_case2_left(ptrBinarySearchTree p, ptrBinarySearchTree x){
printf("\n         <> fixredblack_remove_case2_left informatin <>\n");    

    ptrBinarySearchTree f= frere(x,p);
    leftrotate(p);
    p->color=red;
    f->color=black;
    return fixredblack_remove_case1(p,x);
}
/*=======================================================================================================*/
ptrBinarySearchTree fixredblack_remove_case2_right(ptrBinarySearchTree p, ptrBinarySearchTree x){
printf("\n         <> fixredblack_remove_case2_right informatin <>\n");    
    ptrBinarySearchTree f= frere(x,p);
    p->color=red;
    if(f)f->color=black;
    rightrotate(p);
    return fixredblack_remove_case1(p,x);
}
/*=======================================================================================================*/
void printNode(const BinarySearchTree *n, void *out){
    FILE *file = (FILE *)out;
    printf("%d ", bstree_root(n));
    if(n->color == red) fprintf(out, "\tn%d [style=filled,fillcolor=red,label=\"{{<parent>}|%d|{<left>|<right>}}\"];\n",
            bstree_root(n),bstree_root(n));
    else fprintf(out, "\tn%d [style=filled,fillcolor=grey,label=\"{{<parent>}|%d|{<left>|<right>}}\"];\n",
            bstree_root(n),bstree_root(n));

    if (bstree_left(n)) {
        fprintf(file, "\tn%d:left:c -> n%d:parent:c [headclip=false, tailclip=false]\n",
                bstree_root(n), bstree_root(bstree_left(n)));
    } else {
        fprintf(file, "\tlnil%d [style=filled, fillcolor=grey, label=\"NIL\"];\n", bstree_root(n));
        fprintf(file, "\tn%d:left:c -> lnil%d:n [headclip=false, tailclip=false]\n",
                bstree_root(n), bstree_root(n));
    }
    if (bstree_right(n)) {
        fprintf(file, "\tn%d:right:c -> n%d:parent:c [headclip=false, tailclip=false]\n",
                bstree_root(n), bstree_root(bstree_right(n)));
    } else {
        fprintf(file, "\trnil%d [style=filled, fillcolor=grey, label=\"NIL\"];\n", bstree_root(n));
        fprintf(file, "\tn%d:right:c -> rnil%d:n [headclip=false, tailclip=false]\n",
                bstree_root(n), bstree_root(n));
    }
}
/*========================================================================================================*/
void rbtree_export_dot ( const BinarySearchTree *t , FILE * file ) {
    fprintf ( file , " digraph RedBlackTree {\n\tgraph[ ranksep =0.5];\n\tnode [ shape = record ];\n\n") ;
    bstree_iterative_depth_infix (t , printNode , file );
    fprintf ( file , "\n}\n");
}
