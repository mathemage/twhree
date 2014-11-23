/* 
 * twhree.h - twhree v1.16
 * =============================================================
 * Header file of twhree (2-3 tree implementation) library
 */

#ifndef TWHREE_H
#define TWHREE_H

/************************** DATA STRUCTURES *************************/
struct TItem {
  int key;
  char name[10];
};

struct subtree {
  struct node *sub;                  /* pointer to a subtree */
  int max_key;                       /* ...and its maximal key */
};

struct node {                        /* node of 2-3 tree */
  struct node *parent;               /* parent of the node */
  struct subtree kidz[3];            /* children of the node */
  struct TItem *pData;
};
/************************** DATA STRUCTURES *************************/

/************************** FUNCTION HEADERS ************************/
/* preorder DFS display of 2-3 tree - with structured indentation */
void displayStruct(struct node *cur, int lvl);

/* insert new node & return a pointer to the newly added node */
struct node * insert(struct TItem *pNewItem, struct node **pRoot);

/* delete node with given key */
int delete(struct node **pRoot, int key);

/* delete the whole tree in *pRoot */
void deleteAll(struct node **pRoot);

/* search sKey in the tree of "root" and if it's found, return the pointer to
 * data indexed by this sKey */
struct TItem * find(struct node *root, int sKey);

/* search data with maximal key */
struct TItem * findMax(struct node *root);

/* search data with minimal key */
struct TItem * findMin(struct node *root);

/* find data with key previous to given key */
struct TItem * previous(struct node *root, int sKey);

/* find data with key next to given key */
struct TItem * next(struct node *root, int sKey);
/************************** FUNCTION HEADERS ************************/

#endif
