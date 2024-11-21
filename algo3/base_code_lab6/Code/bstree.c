#include "bstree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

void bstree_remove_node(ptrBinarySearchTree *t, ptrBinarySearchTree current);

/*------------------------  BSTreeType  -----------------------------*/

typedef enum {red, black} NodeColor;

struct _bstree {
  BinarySearchTree *parent;
  BinarySearchTree *left;
  BinarySearchTree *right;
  int root;
  NodeColor color;
};

/*------------------------  BaseBSTree  -----------------------------*/

BinarySearchTree *bstree_create() { return NULL; }

/* This constructor is private so that we can maintain the oredring invariant on
 * nodes. The only way to add nodes to the tree is with the bstree_add function
 * that ensures the invariant.
 */
BinarySearchTree *bstree_cons(BinarySearchTree *left, BinarySearchTree *right, int root) {
  BinarySearchTree *t = malloc(sizeof(struct _bstree));
  t->parent = NULL;
  t->left = left;
  t->right = right;
  if (t->left != NULL)
    t->left->parent = t;
  if (t->right != NULL)
    t->right->parent = t;
  t->root = root;
  t->color = red;
  return t;
}

void bstree_delete(ptrBinarySearchTree *t) {
  while (!bstree_empty(*t))
    bstree_remove_node(t, *t);
}

bool bstree_empty(const BinarySearchTree *t) { 
  return t == NULL; }

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

/*------------------------  BSTreeDictionary  -----------------------------*/

/* Obligation de passer l'arbre par référence pour pouvoir le modifier */
void bstree_add(ptrBinarySearchTree *t, int v) {
  ptrBinarySearchTree *cur = t;
  BinarySearchTree *par = NULL;
  while (*cur) {
    par = *cur;
    if ((*cur)->root == v)
      return;
    cur = (((*cur)->root > v) ? &((*cur)->left) : &((*cur)->right));
  }
  *cur = bstree_cons(NULL, NULL, v);
  (*cur)->parent = par;
}

bool bstree_search(const BinarySearchTree *t, int v) {
  BinarySearchTree *cur = (BinarySearchTree *)t;
  while (cur) {
    if (cur->root == v)
      return true;
    (cur->root > v) ? (cur = cur->left) : (cur = cur->right);
  }
  return false;
}

BinarySearchTree *bstree_successor(const BinarySearchTree *x) {
  assert(!bstree_empty(x));
  BinarySearchTree *y = x->left;
  if (y) {
    while (y->right)
      y = y->right;
  } else {
    y = x->parent;
    while (y && (x == y->left)) {
      x = y;
      y = y->parent;
    }
  }
  return y;
}

BinarySearchTree *bstree_predecessor(const BinarySearchTree *x) {
  assert(!bstree_empty(x));
  BinarySearchTree *y = x->right;
  if (y) {
    while (y->left)
      y = y->left;
  } else {
    y = x->parent;
    while (y && (x == y->right)) {
      x = y;
      y = y->parent;
    }
  }
  return y;
}

void swap_parent(ptrBinarySearchTree from, ptrBinarySearchTree to) {
  ptrBinarySearchTree buffer = from->parent;
  from->parent = to->parent;

  if (from->parent) {
    if (from->parent->left == to)
      from->parent->left = from;
    if (from->parent->right == to)
      from->parent->right = from;
  }

  to->parent = buffer;

  if (from->parent) {
    if (to->parent->left == buffer)
      to->parent->left = to;
    if (to->parent->right == buffer)
      to->parent->right = to;
  }
}

void swap_right(ptrBinarySearchTree from, ptrBinarySearchTree to) {
  ptrBinarySearchTree buffer = from->right;

  from->right = to->right;
  if (from->right)
    from->right->parent = from;

  to->right = buffer;
  if (to->right)
    to->right->parent = to;
}

void swap_left(ptrBinarySearchTree from, ptrBinarySearchTree to) {
  ptrBinarySearchTree buffer = from->left;

  from->left = to->left;
  if (from->left)
    from->left->parent = from;

  to->left = buffer;
  if (to->left)
    to->left->parent = to;
}

void bstree_swap_nodes(ptrBinarySearchTree *tree, ptrBinarySearchTree from, ptrBinarySearchTree to) {
  assert(!bstree_empty(*tree) && !bstree_empty(from) && !bstree_empty(to));

  swap_parent(from, to);
  swap_left(from, to);
  swap_right(from, to);

  if (from == *tree)
    *tree = to;
  if (to == *tree)
    *tree = from;
}

// t -> the tree to remove from, current -> the node to remove
void bstree_remove_node(ptrBinarySearchTree *t, ptrBinarySearchTree current) {
  assert(!bstree_empty(*t) && !bstree_empty(current));
  if (!current->left && !current->right) {
    if (current->parent->left == current)
      current->parent->left = NULL;
    if (current->parent->right == current)
      current->parent->right = NULL;
    free(current);

  } else if (current->left && !current->right) {
    bstree_swap_nodes(t, current, current->left);
    free(current);
  } else if (!current->left && current->right) {
    bstree_swap_nodes(t, current, current->right);
    free(current);
  } else {
    BinarySearchTree *buffer = bstree_successor(current);
    if (!buffer)
      bstree_predecessor(current);
    bstree_swap_nodes(t, current, buffer);
    free(current);
  }
}

void bstree_remove(ptrBinarySearchTree *t, int v) {
  BinarySearchTree *cur = *t;
  while (cur && cur->root != v) {
    (cur->root > v) ? (cur = cur->left) : (cur = cur->right);
  }
  if (cur)
    bstree_remove_node(t, cur);
}

/*------------------------  BSTreeVisitors  -----------------------------*/

void bstree_depth_prefix(const BinarySearchTree *t, OperateFunctor f,
                         void *userData) {
  if (bstree_empty(t)) {
    return;
  }
  f(t, userData);
  bstree_depth_prefix(bstree_left(t), f, userData);
  bstree_depth_prefix(bstree_right(t), f, userData);
}

void bstree_depth_infix(const BinarySearchTree *t, OperateFunctor f,
                        void *userData) {
  if (bstree_empty(t))
    return;

  bstree_depth_infix(bstree_left(t), f, userData);
  f(t, userData);
  bstree_depth_infix(bstree_right(t), f, userData);
}

void bstree_depth_postfix(const BinarySearchTree *t, OperateFunctor f,
                          void *userData) {
  if (bstree_empty(t))
    return;
  bstree_depth_postfix(bstree_left(t), f, userData);
  bstree_depth_postfix(bstree_right(t), f, userData);
  f(t, userData);
}

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

void bstree_iterative_breadth_prefix(const BinarySearchTree *t, OperateFunctor f, void *userData) {
  Queue *q = createQueue();
  BinarySearchTree *cur;
  queuePush(q, t);

  while (!queueEmpty(q)) {
    cur = (BinarySearchTree *)queueTop(q);
    f(cur, userData);
    queuePop(q);
    if (bstree_left(cur))
      queuePush(q, bstree_left(cur));
    if (bstree_right(cur))
      queuePush(q, bstree_right(cur));
  }
}

/*------------------------  BSTreeIterator  -----------------------------*/

struct _BSTreeIterator {
  /* the collection the iterator is attached to */
  const BinarySearchTree *collection;
  /* the first element according to the iterator direction */
  const BinarySearchTree *(*begin)(const BinarySearchTree *);
  /* the current element pointed by the iterator */
  const BinarySearchTree *current;
  /* function that goes to the next element according to the iterator direction
   */
  BinarySearchTree *(*next)(const BinarySearchTree *);
};

/* minimum element of the collection */
const BinarySearchTree *goto_min(const BinarySearchTree *e) {
  (void)e;
  return NULL;
}

/* maximum element of the collection */
const BinarySearchTree *goto_max(const BinarySearchTree *e) {
  (void)e;
  return NULL;
}

/* constructor */
BSTreeIterator *bstree_iterator_create(const BinarySearchTree *collection,
                                       IteratorDirection direction) {
  (void)collection;
  (void)direction;
  return NULL;
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

bool bstree_iterator_end(const BSTreeIterator *i) { return i->current == NULL; }

BSTreeIterator *bstree_iterator_next(BSTreeIterator *i) {
  i->current = i->next(i->current);
  return i;
}

const BinarySearchTree *bstree_iterator_value(const BSTreeIterator *i) {
  return i->current;
}

