#include<assert.h>
#include<stdio.h>
#include"string.h"
#include<stdlib.h>

struct llt {
    int val;
    struct llt **children;
  };

// count_leaves(t) Count how many leaves there are in t.
int count_leaves(const struct llt *t) {
    assert(t);

    int count = 0;

    if (NULL == t->children) {
        return 1;
    } else {
        for (int i = 0; i < t->val; i++) {
            count += count_leaves(t->children[i]);
        }
    }
    return count;
}
// get_leaves(t, leaves, len) recursively find all the leaves in t
// and store them in the array pointed to by leaves.
void get_leaves(const struct llt *t, int **leaves, int *len) {
    assert(t);
    assert(leaves);
    assert(len);

    if (NULL == t->children) {
        ++*len;
        *leaves = realloc(*leaves, sizeof(int) * *len);
        (*leaves)[*len - 1] = t->val;
    } else {
        for (int i = 0; i < t->val; i++) {
            get_leaves(t->children[i], leaves, len);
        }
    }
}

// flatten(t, len) Create an array containing all the (leaf) values
// stored in t, in order.  Mutate *len so it contains the length of
// this array.
// effects: allocates memory; caller must call free; mutates len.
// time: O(n)
int *flatten(const struct llt *t, int *len) {
    assert(t);
    assert(len);
    int *leaves = malloc(sizeof(int) * *len);
    *len = 0;

    get_leaves(t, &leaves, len);
    return leaves;
}

void llt_print_helper(const struct llt *t) {
    if (t->children == NULL) {  // Leaf node
        printf("%d", t->val);
    } else {
        printf("<");
        for (int i = 0; i < t->val; i++) {
            llt_print_helper(t->children[i]);
            if (i < t->val - 1)
                printf(", ");
        }
        printf(">");
    }
}

// llt_print(t) Print the tree t, nicely, with brackets.
void llt_print(const struct llt *t) {
    llt_print_helper(t);
    printf("\n");
}
// createLeaf() Create a llt from stdin
// effects: allocates memory; caller must call free.
// time: O(1)
// returns: a pointer to the new leaf node, or NULL if input is invalid.
struct llt *createLeaf() {
    int value;
    struct llt *leaf = malloc(sizeof(struct llt));
    if (1 == scanf("%d", &value)) {
      leaf->val = value;
      leaf->children = NULL;
    } else {
      free(leaf);
      return NULL;
    }
    return leaf;
  }
  
  // llt_read(void) Read from stdin and creates a struct llt in the heap.
  // The data are serialized as follows:
  // * A leaf shall be stored as the integer 0 followed by the label for
  //   the leaf;
  // * A non-leaf shall be stored as a positive integer representing the
  //   number of children of a node, followed by the representations of
  //   all its children, recursively.
  // If the first read fails, return NULL (indicating that no further
  // trees are available to read).
  
  // effects: reads from stdin
  //          allocates memory; user must call llt_destroy.
  // time: O(n) where n is the number of nodes in the tree.
  struct llt *llt_read(void) {
    int read;
    if (1 == scanf("%d", &read)) {
      if (0 == read) {
        return createLeaf();
      } else {
        struct llt *node = malloc(sizeof(struct llt));
        node->val = read;
        node->children = malloc(read * sizeof(struct llt *));
  
        for (int i = 0; i < read; i++) {
          node->children[i] = llt_read();
        }
  
        return node;
      }
    } else {
      return NULL;
    }
  }

  // llt_destroy(t) Clean up t.
  // effects: frees memory.
  void llt_destroy(struct llt *t) {
    if (!t) return;
  
    if (t->children != NULL) {
      int len = t->val;
      for (int i = 0; i < len; i++) {
        llt_destroy(t->children[i]);
      }
      free(t->children);
    }
    free(t);
  }

  // reads in trees from stdin, prints them, and flattens them.
  int main(void) {
    struct llt *t = NULL;
    int *flat = NULL;
    int flatlen = 0;
  
    printf("Reading trees from stdin.\n"); 
    
    while ((t = llt_read()) != NULL) {
      printf("\nNew tree of %d leaves:\n",
             count_leaves(t));
      
      llt_print(t);
  
      flat = flatten(t, &flatlen);
      array_print(flat, flatlen);
      free(flat);
      
      llt_destroy(t);
    }
  }