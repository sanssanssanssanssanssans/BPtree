#ifndef BPLUS_H
#define BPLUS_H
#include <stdbool.h>
#define MX_KEYS 3

typedef int (*CompareFunc)(void* a,void *b);
typedef struct Node {
    void *keys[MX_KEYS];
    void *pointers[MX_KEYS+1];
    struct Node *parent;
    bool is_leaf;
    int num_keys;
    struct Node *nxt;
} Node;

typedef struct {
    Node *root;
    CompareFunc cmp;
} BPlusTree;

BPlusTree* bplus_tree_create(CompareFunc cmp);
void bplus_tree_insert(BPlusTree *tree, void *key, void *value);
void* bplus_tree_find(BPlusTree *tree, void *key);
void bplus_tree_destroy(BPlusTree *tree);

#endif