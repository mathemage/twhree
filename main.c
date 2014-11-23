/*
 * main.c - twhree v1.16
 * =============================================================
 * Demonstration program of twhree (2-3 tree implementation) library
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "twhree.h"

void displayMenu() {
  printf("------------------------------------------------------------------\n");
  printf("Choose option (enter the part in brackets)\n");
  printf("(I)nsert \t - insert a new node \n");
  printf("(D)elete \t - delete the node containing specified value \n");
  printf("delete (A)ll\t - delete the node containing specified value \n");
  printf("(F)ind \t\t - ascertain the presence of the value in the tree \n");
  printf("find(M)in \t - show the minimum of the tree \n");
  printf("findma(X) \t - show the maximum of the tree \n");
  printf("(P)revious \t - show the previous item according to the key \n");
  printf("(N)ext \t\t - show the next item according to the key \n");
  printf("displa(Y) \t - display tree (with structured indentation) \n");
  printf("c(L)ear \t - clear screen (only for in *nix like OS) \n");
  printf("(H)elp \t\t - displays this menu:) \n");
  printf("(Q)uit \t\t - quit the program \n");
  printf("------------------------------------------------------------------\n");
}

int main() {
  struct TItem *pItem;                  /* auxiliary item */
  struct node *root = NULL;             /* root node of 2-3 tree */
  struct node *pNewNode = NULL;         /* pointer to a place, where node was added */
  char cOption;                         /* chosen command */

  printf("Welcome to twhree v1.16!\n");
  printf("This is a demonstration program of twhree [T(wo)-(t)H(ree) (t)REE] library.\n");
  displayMenu();
  
  do {
    printf(">> ");
    scanf("%c%*c", &cOption);           /* read the first char, omit the rest of line */
    cOption = toupper(cOption);

    /* what option? */
    switch (cOption) {
      /* insert new node */
      case 'I': printf("Enter new item...\n");
                pItem = (struct TItem *) malloc(sizeof(struct TItem));

                printf(" New key: ");
                scanf("%d", &pItem->key);
                printf(" New name: ");
                scanf("\n%[^\n]%*c", pItem->name);

                pNewNode = insert(pItem, &root);

                /* message of successful inserting */
                printf("Adding \"%s\" with key %d......OK\n", pNewNode->pData->name,
                    pNewNode->pData->key);
                break;

      /* delete node with given key */
      case 'D': printf(" Desired key: ");
                int key;
                scanf("%d%*c", &key);
                printf("Deleting node with key %d......", key);
                switch(delete(&root, key)) {
                  case -1: printf("EMPTY TREE!");
                           break;
                  case 0:  printf("OK");
                           break;
                  case 1:  printf("KEY NOT FOUND!");
                           break;
                  case 2:  printf("NOT IMPLEMENTED (yet:)\n Please try another key...");
                           break;
                }
                printf("\n");
                break;

      /* delete all - the whole tree */
      case 'A': deleteAll(&root);
                break;

      /* find data with given key */
      case 'F': printf(" Search key: ");
                int sKey;
                scanf("%d%*c", &sKey);
                printf("Serching key %d......", sKey);
                pItem = find(root, sKey);

                if (NULL == pItem) {
                  printf("NOT FOUND!\n");
                } else {
                  printf(" (%d,\"%s\")\n", pItem->key, pItem->name);
                }
                break;

      /* find data with maximal key */
      case 'X': printf("Searching maximal key......");
                pItem = findMax(root);

                if (NULL == pItem) {
                  printf("NOT FOUND!\n");
                } else {
                  printf(" (%d,\"%s\")\n", pItem->key, pItem->name);
                }
                break;

      /* find data with minimal key */
      case 'M': printf("Searching minimal key......");
                pItem = findMin(root);

                if (NULL == pItem) {
                  printf("NOT FOUND!\n");
                } else {
                  printf(" (%d,\"%s\")\n", pItem->key, pItem->name);
                }
                break;

      /* find data with key previous to given key */
      case 'P': printf(" Previous of which key? ");
                scanf("%d%*c", &sKey);

                printf("Searching previous item of %d key......", sKey);
                pItem = previous(root, sKey);

                if (NULL == pItem) {
                  printf("NOT FOUND!\n");
                } else {
                  printf(" (%d,\"%s\")\n", pItem->key, pItem->name);
                }
                break;

      /* find data with key next to given key */
      case 'N': printf(" Next of which key? ");
                scanf("%d%*c", &sKey);

                printf("Searching next item of %d key......", sKey);
                pItem = next(root, sKey);

                if (NULL == pItem) {
                  printf("NOT FOUND!\n");
                } else {
                  printf(" (%d,\"%s\")\n", pItem->key, pItem->name);
                }
                break;

      /* display the tree with structured indentation */
      case 'Y': displayStruct(root,0);
                break;

      /* display command menu */
      case 'H': displayMenu();
                break;

      /* clear screen - works only on *nix systems */
      case 'L': system("clear");
                break;

      /* quit & delete the tree */
      case 'Q': deleteAll(&root);
                printf("Bye bye...\n");
                break;

      default:  printf("Invalid command \"%c\"! Please try again...\n", cOption);
    }
  } while ('Q' != cOption);

  return 0;
}
