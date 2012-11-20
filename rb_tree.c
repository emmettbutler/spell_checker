#include "rb_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>

// a basic red-black tree implementation that supports insertion and lookup
// *no deletion*

rb_node *rb_create_tree(char *val){
    // make a node
    rb_node *node = rb_create_node(val);
    insertion_invariants(node);
    return node;
}

rb_node *grandparent(rb_node *node){
    // helper for invariants
    if(node != NULL && node->parent != NULL)
        return node->parent->parent;
    return NULL;
}

rb_node *uncle(rb_node *node){
    // helper for invariants
    rb_node *g = grandparent(node);
    if(g == NULL)
        return NULL;
    if(node->parent == g->left)
        return g->right;
    return g->left;
}

rb_node *rb_insert(rb_node *tree, char *insVal){
    // standard binary tree insert algorithm
    if(tree == NULL || tree->value == NULL){
        tree = rb_create_tree(insVal);
        return;
    }

    rb_node *root;
    int cmp;
    cmp = strcasecmp(insVal, tree->value);
    if(cmp < 0){
        if(tree->left != NULL){
            rb_insert(tree->left, insVal);
        } else {
            tree->left = rb_create_node(insVal);
            tree->left->parent = tree;
            insertion_invariants(tree->left);
        }
    } else if(cmp > 0){
        if(tree->right != NULL){
            rb_insert(tree->right, insVal);
        } else {
            tree->right = rb_create_node(insVal);
            tree->right->parent = tree;
            insertion_invariants(tree->right);
        }
    }

    // make sure to return a pointer to the new root
    // adds no complexity
    return get_root(tree);
}

// puts the "red-black" in "red-black tree"
// pointer surgery to maintain the red-black tree invariants
// and hopefully guarantee some general height-balancing
void insertion_invariants(rb_node *n){
    if(n->parent == NULL){
        n->red = 0;
    } else {
        if(!n->parent->red){
            return;
        } else {
            rb_node *u = uncle(n), *g;
            if(u != NULL && u->red){
                n->parent->red = 0;
                u->red = 0;
                g = grandparent(n);
                g->red = 1;
                insertion_invariants(g);
            } else {
                g = grandparent(n);
                if(g == NULL) return;
                if(n == n->parent->right && n->parent == g->left){
                    rotate_left(n->parent);
                    n = n->left;
                } else if(n == n->parent->left && n->parent == g->right){
                    rotate_right(n->parent);
                    n = n->right;
                }
                g = grandparent(n);
                n->parent->red = 0;
                g->red = 1;
            }
        }
    }
}

// convenience function for balancing the tree
void rotate_left(rb_node *n){
    rb_node *q = n->right;

    n->right = q->left;
    if(n->right)
        n->right->parent = n;

    q->parent = n->parent;

    q->left = n;

    if(n->parent){
        if(n == n->parent->right)
            n->parent->right = q;
        else if(n == n->parent->left)
            n->parent->left = q;
    }

    // commenting this line out fixes a segfault
    n->parent = q;
}

// convenience function for balancing the tree
void rotate_right(rb_node *n){
    rb_node *p = n->left;

    n->left = p->right;
    if(n->left)
        n->left->parent = n;

    p->parent = n->parent;

    p->right = n;

    if(n->parent){
        if(n == n->parent->right)
            n->parent->right = p;
        else if(n == n->parent->left)
            n->parent->left = p;
    }

    n->parent = p;

}

// allocate a new node
rb_node *rb_create_node(char *value){
    rb_node *node = (rb_node *)malloc(sizeof(rb_node));
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->red = 1;
    node->value = (char *)malloc(sizeof(char) * (strlen(value) + 1));
    // use safe string copy - thanks valgrind!
    strncpy(node->value, value, strlen(value) + 1);
    return node;
}

// walk the tree and print something that kinda looks like it
void print_node(rb_node *tree, int depth){
    if(tree->left != NULL){
        print_node(tree->left, depth+1);
    }
    int i;
    for(i = 0; i < depth; i++){
        printf("  ");
    }

    if(tree->red) printf("(");
    else printf(" ");
    printf("%s", tree->value);
    if(tree->red) printf(")\n");
    else printf("\n");

    if(tree->right != NULL){
        print_node(tree->right, depth+1);
    }
}

// binary tree search - O(logn)
rb_node *rb_search(rb_node *tree, char *sVal){
    int cmp = strcasecmp(sVal, tree->value);
    if(cmp == 0){
        return tree;
    } else if(tree->right == NULL && tree->left == NULL){
        return NULL;
    }
    if(tree->left != NULL && cmp < 0){
        return rb_search(tree->left, sVal);
    } else if(tree->right != NULL && cmp > 0){
        return rb_search(tree->right, sVal);
    } else {
        return NULL;
    }
}

void rb_delete(rb_node *tree, char *delVal){
    // unimplemented
}

// traverse and deallocate all nodes
void rb_dealloc(rb_node *tree){
    if(tree && tree->left != NULL){
        rb_dealloc(tree->left);
    }
    if(tree && tree->right != NULL){
        rb_dealloc(tree->right);
    }
    if(tree == NULL) return;
    free(tree->value);
    free(tree);
}

// helper for proper top-level pointers
rb_node *get_root(rb_node *n){
    if(n->parent == NULL){
        return n;
    } else {
        get_root(n->parent);
    }
}

// used for testing
int rb_main(int argc, char* argv[]){
    rb_node *root = rb_create_tree("zz");
    char c = 'a';
    int i = 0;
    char s[5000];
    for(i = 0; i < 5000; i++){
        s[i] = '\0';
    }
    int strIndex = 1;
    s[0] = 'a';
    for(i = 0; i < 3000; i++){
        if(c < 'z'){
            c++;
        } else {
            c = 'a';
            strIndex++;
        }
        s[strIndex] = c;
        root = rb_insert(root, s);
    }
    root = get_root(root);
    print_node(root, 0);
    printf("\n");
    rb_node *n = rb_search(root, "bonners");
    if(n){
        printf("bonners in tree\n");
    }
    n = rb_search(root, "zz");
    if(n){
        printf("zz in tree\n");
    }
    printf("\n");
    rb_dealloc(root);

    return 0;
}
