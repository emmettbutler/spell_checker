#include "my_strmap.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* a strMap provides a mapping between hashed strings and their plaintext
 * values, using a red-black tree to handle collisions
 * */


strMap *sm_new(unsigned int capacity){
    // allocate all necessary space for an uninitialized hash table
    strMap *map;
    Bucket *bucket;
    int i = 0;

    map = malloc(sizeof(strMap));
    map->count = capacity;
    map->buckets = malloc(map->count * sizeof(Bucket));
    memset(map->buckets, 0, map->count * sizeof(Bucket));

    bucket = map->buckets;
    for(i = 0; i < map->count; i++){
        bucket->count = 0;
    }

    return map;
}

static unsigned long hash(char *str)
{
    unsigned long hash = 5381;
    int c, i;

    // hash function must be case-insensitive
    for(i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }

    // magic
    while (c = *str++) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

int sm_get(strMap *map, char *string){
    unsigned int index;
    Bucket *bucket;
    rb_node *ret;

    index = hash(string) % map->count;
    bucket = &(map->buckets[index]);

    // bucket always contains a node or empty
    if(bucket->strings == NULL){
        return 0;
    }

    // search the collision tree
    ret = rb_search(bucket->strings, string);
    if(ret){
        return 1;
    }
    return 0;
}

void sm_put(strMap *map, char *string){
    unsigned int index;
    Bucket *bucket;
    rb_node *node;

    // special case, must remove trailing chars in some cases
    if(string[strlen(string) - 1] == '\r'){
        string[strlen(string) - 1] = '\0';
    }

    index = hash(string) % map->count;
    bucket = &(map->buckets[index]);

    // similar to sm_get: add to the collision tree
    // or create a tree
    if(bucket->strings == NULL){
        bucket->strings = rb_create_tree(string);
    } else {
        bucket->strings = rb_insert(bucket->strings, string);
    }
    bucket->count++;
}

int sm_get_count(strMap *map){
    // walk the hash table and accumulate counts for each slot
    int i = 0, count = 0;
    Bucket *bucket = map->buckets;

    while(i < map->count){
        if(bucket && bucket->count)
            count += bucket->count;
        bucket++;
        i++;
    }
    return count;
}

void sm_dealloc(strMap *map){
    int i;
    Bucket *bucket = map->buckets, *tmp_bucket;
    // deallocate the collision trees
    for(i = 0; i < map->count; i++){
        rb_dealloc(bucket->strings);
        bucket++;
    }
    // deallocate the rest of the table
    free(map->buckets);
    free(map);
}

// used for testing
int sm_main(int argc, char *argv[]){
    int present = 0;
    strMap *map = sm_new(527);

    char c = 'a';
    int i = 0, in;
    char s[500];
    for(i = 0; i < 500; i++){
        s[i] = '\0';
    }
    int strIndex = 1;
    s[0] = 'a';
    for(i = 0; i < 1284; i++){
        if(c < 'z'){
            c++;
        } else {
            c = 'a';
            strIndex++;
        }
        s[strIndex] = c;
        sm_put(map, s);
    }

    in = sm_get(map, "bzb");
    printf("%d\n", in);
    in = sm_get(map, "azb");
    printf("%d\n", in);
    printf("%d\n", sm_get_count(map));
}
