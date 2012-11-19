// a red-black tree node with parent pointers
typedef struct _rb_node{
    struct _rb_node *parent;
    struct _rb_node *left;
    struct _rb_node *right;
    int red;
    char *value;
} rb_node;

rb_node *rb_create_tree(char *val);
rb_node *rb_create_node(char *value);
void print_tree(rb_node *tree);
rb_node *rb_insert(rb_node *tree, char *insVal);
rb_node *rb_search(rb_node *tree, char *sVal);
void rb_delete(rb_node *tree, char *delVal);
void print_node(rb_node *tree, int depth);
rb_node *grandparent(rb_node *node);
rb_node *uncle(rb_node *node);
void insertion_invariants(rb_node *n);
void rotate_right(rb_node *n);
void rotate_left(rb_node *n);
void rb_dealloc(rb_node *tree);
rb_node *get_root(rb_node *n);
