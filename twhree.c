/* 
 * twhree.h - twhree v1.16
 * =============================================================
 * Header file of twhree (2-3 tree implementation) library
 */

#include <stdio.h>
#include <stdlib.h>
#include "twhree.h"

/* decide, in which branch of the node to continue in... */
struct node * branch(struct node *cur, int key) {
  if (key <= cur->kidz[0].max_key) {
    return cur->kidz[0].sub;                /* -> left tree */
  } else if (key <= cur->kidz[1].max_key    /* key in middle tree */
      || NULL == cur->kidz[2].sub) {        /* ...or no right tree */
    return cur->kidz[1].sub;                /* -> middle tree */
  } else {
    return cur->kidz[2].sub;                /* -> right tree */
  }
}

/* BubbleSort of kidz */
void BubbleKidz(struct subtree * tmp_kidz) {
  struct subtree tmp;
  int i;
  for (i = 3; i > 0; i--) {
    if (NULL == tmp_kidz[i-1].sub              /* empty subtree goes always to the right */
        || tmp_kidz[i-1].max_key >= tmp_kidz[i].max_key) { /* inversion */
      tmp = tmp_kidz[i-1];
      tmp_kidz[i-1] = tmp_kidz[i];
      tmp_kidz[i] = tmp;
    }
  }
}

/* preorder DFS display of 2-3 tree - with structured indentation */
void displayStruct(struct node *cur, int lvl) {
  if (NULL == cur) {
    printf("Empty tree!\n");
    return;
  }

  int i;
  /* print out info on current node */

  /* indentation according to lvl parametr */
  for (i = 0; i < lvl; i++) {
    printf(" ");
  }

  /* leaf */
  if (NULL == cur->kidz[0].sub) {
    if (NULL != cur->pData) {               /* not empty tree */
      printf("(%d,\"%s\")\n", cur->pData->key, cur->pData->name);
    }

  /* search subtrees left to right */
  } else {
    /* print out maximal keys of subtrees of current node */
    printf("[%d", cur->kidz[0].max_key);
    if (NULL != cur->kidz[2].sub) {         /* 3 children? -> 2 max keys */
      printf("|%d", cur->kidz[1].max_key);
    }
    printf("]\n");

    for (i=0; i<3; i++) {
      if (NULL != cur->kidz[i].sub) {
        displayStruct(cur->kidz[i].sub, lvl+1);
      }
    }
  }
}

/* display all about current node */
void displayNode(struct node * cur) {
  printf("%p\n", cur);
  printf(" parent = %p\n", cur->parent);

  int i;
  for (i = 0; i < 3; i++) {
    printf(" kidz@%p = %d", cur->kidz[i].sub, cur->kidz[i].max_key);
  }
  printf("\n pData@%p\n", cur->pData);
}

/* update max_key of start's ancestors */
void updateMaxKeys(struct node * start) {
  struct node *cur = start;

  /* repeat for all start's ancestors */
  while (NULL != cur && NULL != cur->parent) {
    /* new value of max_key */
    /* 3 kidz ? max_key of 3rd kid : max_key of 2nd kid */
    int new_max_key = (NULL != cur->kidz[2].sub) ? cur->kidz[2].max_key : cur->kidz[1].max_key;

    /* which position does cur have in parent? */
    int index_in_parent, i;
    for (i = 0; i < 3; i++) {
      if (cur->parent->kidz[i].sub == cur) {
        index_in_parent = i;
        break;
      }
    }

    cur->parent->kidz[index_in_parent].max_key = new_max_key;
    cur = cur->parent;
  }
}

/* insert new node & return a pointer to the newly added node */
struct node * insert(struct TItem *pNewItem, struct node **pRoot) {
  int i;

  /* --------very trivial cases-------- */
  if (NULL == *pRoot) {                       /* empty tree */
    printf("Empty root...\n");
    
    /* creating new root with no children */
    *pRoot = (struct node *) malloc(sizeof(struct node));
    (*pRoot)->parent = NULL;
    for (i=0; i<3; i++) {
      (*pRoot)->kidz[i].sub = NULL;
    }
    (*pRoot)->pData = pNewItem;

    return *pRoot;

  } else if (NULL == (*pRoot)->kidz[0].sub) {   /* only root present => create new
                                               node in the right side of the root */
    /* creating the new node */
    struct node *tmp = (struct node *) malloc(sizeof(struct node));
    for (i=0; i<3; i++) {
      tmp->kidz[i].sub = NULL;
    }
    tmp->pData = pNewItem;

    /* new parent of the root and the tmp */
    struct node *ancestor = (struct node *) malloc(sizeof(struct node));
    ancestor->parent = NULL;
    if ((*pRoot)->pData->key < tmp->pData->key) {
      ancestor->kidz[0].sub = *pRoot;
      ancestor->kidz[0].max_key = (*pRoot)->pData->key;
      ancestor->kidz[1].sub = tmp;
      ancestor->kidz[1].max_key = tmp->pData->key;
    } else {
      ancestor->kidz[0].sub = tmp;
      ancestor->kidz[0].max_key = tmp->pData->key;
      ancestor->kidz[1].sub = *pRoot;
      ancestor->kidz[1].max_key = (*pRoot)->pData->key;
    }
    ancestor->kidz[2].sub = NULL;
    ancestor->pData = NULL;

    (*pRoot)->parent = tmp->parent = ancestor;   /* make it the parent of both */
    *pRoot = ancestor;                           /* ... and set it as the new root of the tree */
    return tmp;

  /* --------non-trivial cases-------- */
  } else {
    /* search for a parent of the new node */
    /* ----------------------------------- */
    struct node *cur;                       /* current node */
    cur = *pRoot;

    int j = 1;
    while (NULL == cur->pData) {            /* ...not a leaf */
      cur = branch(cur, pNewItem->key);     /* Which branch to continue in? */
    }
    cur = cur->parent;

    /* join the new node & update the parent */
    /* ------------------------------------- */
    struct subtree tmp_kidz[4];             /* copy of kidz & new node addr. in this array */
    for (i=0; i<3; i++) {
      tmp_kidz[i] = cur->kidz[i];
    }

    /* make a new node */
    struct node *pNewNode = (struct node *) malloc(sizeof(struct node));
    pNewNode->pData = pNewItem;
    pNewNode->parent = cur;

    /* and make a reference of it in the temp. array */
    tmp_kidz[3].sub = pNewNode;
    tmp_kidz[3].max_key = pNewNode->pData->key;

    /* BubbleSort of kidz */
    BubbleKidz(tmp_kidz);

    if (NULL == tmp_kidz[3].sub) {               /* will have 3 children - OK */
      for (i = 0; i < 3; i++) {
        cur->kidz[i] = tmp_kidz[i];
      }
      updateMaxKeys(cur);                        /* update max_key of cur's ancestors */
    } else {                                     /* will have 4 children - split to 2 nodes */
      struct node *pAdjacent = NULL;             /* new nodes adjacent to overfull ones */

      do {
        pAdjacent = (struct node *) malloc(sizeof(struct node));

        /* left half of tmp_kidz -> old parent */
        cur->kidz[0] = tmp_kidz[0];
        cur->kidz[1] = tmp_kidz[1];
        cur->kidz[0].sub->parent = cur->kidz[1].sub->parent = cur;
        cur->kidz[2].sub = NULL;
        updateMaxKeys(cur);                        /* update max_key of cur's ancestors */

        /* right half of tmp_kidz -> new parent */
        pAdjacent->kidz[0] = tmp_kidz[2];
        pAdjacent->kidz[1] = tmp_kidz[3];
        pAdjacent->kidz[0].sub->parent = pAdjacent->kidz[1].sub->parent = pAdjacent;
        pAdjacent->kidz[2].sub = NULL;

        pAdjacent->pData = NULL;

        /* create new common parent for cur & pAdjacent, if there's none */
        if (NULL == cur->parent) {
          cur->parent = (struct node *) malloc(sizeof(struct node));
          cur->parent->kidz[0].sub = cur;
          cur->parent->kidz[0].max_key = cur->kidz[1].max_key;
          cur->parent->kidz[1].sub = cur->parent->kidz[2].sub = NULL;

          *pRoot = cur->parent;
        }

        /* make pAdjacent cur's right sibling */
        pAdjacent->parent = cur->parent;

        /* array of cur's new siblings */
        cur = cur->parent;
        for (i=0; i<3; i++) {
          tmp_kidz[i] = cur->kidz[i];
        }
        tmp_kidz[3].sub = pAdjacent;
        tmp_kidz[3].max_key = pAdjacent->kidz[1].max_key;

        /* BubbleSort of previous cur and his siblings */
        BubbleKidz(tmp_kidz);

        /* updating the kidz of the parent */
        if (NULL == cur->kidz[1].sub) {        /* new root with the only kid */
          cur->kidz[1].sub = pAdjacent;
          cur->kidz[1].max_key = pAdjacent->kidz[1].max_key;
          break;
        } else if (NULL == cur->kidz[2].sub) { /* cur's parent has only 2 kidz */
          for (i = 0; i < 3; i++) {
            cur->kidz[i] = tmp_kidz[i];
          }
          break;
        } 
      } while (1);
    }

    return pNewNode;
  }
}

int delete(struct node **pRoot, int key) {
  /* --------very trivial cases-------- */
  if (NULL == *pRoot) {                       /* empty tree */
    return -1;
  } else if (NULL == (*pRoot)->kidz[0].sub) { /* only root present => destroy it... */
    if (key == (*pRoot)->pData->key) {        /* ...iff key is the root's key */
      free((*pRoot)->pData);
      free(*pRoot);
      *pRoot = NULL;
      return 0;
    }
    else {                                    /* otherwise key not found */
      return 1;
    }
  }

  /* --------non-trivial cases-------- */
  /* get to the leaf */
  struct node * cur = *pRoot;
  while (NULL != cur->kidz[0].sub) {
    cur = branch(cur, key);                   /* Which branch to continue in? */
  }

  /* recognize which index (place) cur has in his parent */
  int index_in_parent, i;
  for (i = 0; i < 3; i++) {
    if (cur->parent->kidz[i].sub == cur) {
      index_in_parent = i;
      break;
    }
  }

  struct node * par = cur->parent;
  if (key == cur->pData->key) {             /* key found */
    if (NULL != cur->parent->kidz[2].sub) { /* cur has 2 more siblings => OK, just delete */
      par->kidz[index_in_parent].sub = NULL;

      /* no sort the cur's siblings into right order - viz BubbleKidz, but this
       * sorts only 3 kidz, not 4 kidz */
      struct subtree tmp;
      for (i = 2; i > 0; i--) {
        if (NULL == par->kidz[i-1].sub              /* empty subtree goes always to the right */
            || par->kidz[i-1].max_key >= par->kidz[i].max_key) { /* inversion */
          tmp = par->kidz[i-1];
          par->kidz[i-1] = par->kidz[i];
          par->kidz[i] = tmp;
        }
      }
      updateMaxKeys(par);

      free(cur->pData);
      free(cur);

      return 0;
    } else {                                /* cur has only 1 sibling => NOT
                                               IMPLEMENTED yet... */
      return 2;
    }
  } else {
    return 1;                               /* key not found */
  }
}

/* delete the whole tree in *pRoot */
void deleteAll(struct node **pRoot) {
  struct node *cur = *pRoot;
  if (NULL == cur) {
    printf("Empty tree!\n");
    return;
  }

  int i;

  /* leaf */
  if (NULL == cur->kidz[0].sub) {
    if (NULL != cur->pData) {               /* not empty tree */
      free(cur->pData);
    }

  /* delete subtrees left to right */
  } else {
    for (i=0; i<3; i++) {
      if (NULL != cur->kidz[i].sub) {
        deleteAll(&cur->kidz[i].sub);
      }
    }
  }
  
  free(cur);
  *pRoot = cur = NULL;
}

/* search sKey in the tree of "root" and if it's found, return the pointer to
 * data indexed by this sKey */
struct TItem * find(struct node *root, int sKey) {
  if (NULL == root) {                       /* nothing to look for in an empty tree */
    return NULL;
  }

  struct node * cur = root;

  /* get to the leaf */
  while (NULL != cur->kidz[0].sub) {
    cur = branch(cur, sKey);                /* Which branch to continue in? */
  }

  if (sKey == cur->pData->key) {            /* key found */
    return cur->pData;
  } else {
    return NULL;                            /* key not found */
  }
}

/* search data with maximal key */
struct TItem * findMax(struct node *root) {
  if (NULL == root) {                       /* nothing to look for in an empty tree */
    return NULL;
  }

  struct node * cur = root;

  /* get to the rightmost leaf */
  while (NULL != cur->kidz[0].sub) {
    cur = (NULL == cur->kidz[2].sub) ? cur->kidz[1].sub : cur->kidz[2].sub;
  }

  return cur->pData;
}

/* search data with minimal key */
struct TItem * findMin(struct node *root) {
  if (NULL == root) {                       /* nothing to look for in an empty tree */
    return NULL;
  }

  struct node * cur = root;

  /* get to the leftmost leaf */
  while (NULL != cur->kidz[0].sub) {
    cur = cur->kidz[0].sub;
  }

  return cur->pData;
}

/* find data with key previous to given key */
struct TItem * previous(struct node *root, int sKey) {
  struct node * cur = root;

  /* no previous in an empty tree or single node */
  if (NULL == cur || NULL == cur->kidz[0].sub) {
    return NULL;
  }

  /* get to the leaf */
  while (NULL != cur->kidz[0].sub) {
    cur = branch(cur, sKey);                /* Which branch to continue in? */
  }

  if (sKey != cur->pData->key) {            /* key not found */
    return NULL;
  } else {                                  /* key found */
    /* ascend until node with non-empty right subtree is found */
    while (NULL != cur && NULL != cur->parent) {
      /* which position does cur have in parent? */
      int index_in_parent, i;
      for (i = 0; i < 3; i++) {
        if (cur->parent->kidz[i].sub == cur) {
          index_in_parent = i;
          break;
        }
      }

      /* try to get the max of subtree in cur's left sibling, if there's any */
      switch (index_in_parent) {
        case 1: return findMax(cur->parent->kidz[0].sub);
        case 2: return findMax(cur->parent->kidz[1].sub);
        default: cur = cur->parent;          /* no left sibling, rise to upper level */
      }
    }

    return NULL;                             /* not succesful, already the min of tree */
  }
}

/* find data with key next to given key */
struct TItem * next(struct node *root, int sKey) {
  struct node * cur = root;

  /* no previous in an empty tree or single node */
  if (NULL == cur || NULL == cur->kidz[0].sub) {
    return NULL;
  }

  /* get to the leaf */
  while (NULL != cur->kidz[0].sub) {
    cur = branch(cur, sKey);                /* Which branch to continue in? */
  }

  if (sKey != cur->pData->key) {            /* key not found */
    return NULL;
  } else {                                  /* key found */
    /* ascend until node with non-empty right subtree is found */
    while (NULL != cur && NULL != cur->parent) {
      /* which position does cur have in parent? */
      int index_in_parent, i;
      for (i = 0; i < 3; i++) {
        if (cur->parent->kidz[i].sub == cur) {
          index_in_parent = i;
          break;
        }
      }

      /* try to get the min of subtree in cur's right sibling, if there's any */
      switch (index_in_parent) {
        case 0: return findMin(cur->parent->kidz[1].sub);
        case 1: if (NULL != cur->parent->kidz[2].sub) {     /* node with 3 kidz */
                  return findMin(cur->parent->kidz[2].sub);
                }
        default: cur = cur->parent;          /* no right sibling, rise to upper level */
      }
    }

    return NULL;                             /* not succesful, already the max of tree */
  }
}
