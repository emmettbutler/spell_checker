#include "rb_tree.h"

// a slot containing the root of a collision tree
typedef struct bucket {
    unsigned int count;
    rb_node *strings;
} Bucket;

// a hash table full of slots
typedef struct _strmap {
    unsigned int count;
    Bucket *buckets;
} strMap;

strMap *sm_new(unsigned int capacity);
int sm_get(strMap *map, char *string);
void sm_put(strMap *map, char *string);
