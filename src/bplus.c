#include "bplus.h"
#include <stdlib.h>
#include <string.h>
Node* create_node(bool is_leaf) {
    Node *node = (Node *)calloc(1, sizeof(Node));
    node->is_leaf = is_leaf;
    return node;
}
BPlusTree* bplus_tree_create(CompareFunc cmp) {
    BPlusTree *tree = (BPlusTree *)malloc(sizeof(BPlusTree));
    tree->root = NULL;
    tree->cmp = cmp;
    return tree;
}
void insert_into_parent(BPlusTree *tree, Node *left, void *key, Node *right) {
    Node *parent = left->parent;
    if (!parent) {
        Node *new_root = create_node(false);
        new_root->keys[0] = key;
        new_root->pointers[0] = left;
        new_root->pointers[1] = right;
        new_root->num_keys = 1;
        tree->root = new_root;
        left->parent = right->parent = new_root;
        return;
    }
    int i = 0;
    while (i < parent->num_keys && tree->cmp(parent->keys[i], key) < 0) i++;
    for (int j = parent->num_keys; j > i; j--) {
        parent->keys[j] = parent->keys[j-1];
        parent->pointers[j+1] = parent->pointers[j];
    }
    parent->keys[i] = key;
    parent->pointers[i+1] = right;
    parent->num_keys++;
    right->parent = parent;
    if (parent->num_keys == MX_KEYS) {
        Node *new_internal = create_node(false);
        int split = MX_KEYS / 2;
        void *up_key = parent->keys[split];
        new_internal->num_keys = MX_KEYS - split - 1;
        for (int j = 0; j < new_internal->num_keys; j++) {
            new_internal->keys[j] = parent->keys[split + 1 + j];
            new_internal->pointers[j] = parent->pointers[split + 1 + j];
            ((Node*)new_internal->pointers[j])->parent = new_internal;
        }
        new_internal->pointers[new_internal->num_keys] = parent->pointers[MX_KEYS];
        ((Node*)new_internal->pointers[new_internal->num_keys])->parent = new_internal;
        parent->num_keys = split;
        insert_into_parent(tree, parent, up_key, new_internal);
    }
}
void bplus_tree_insert(BPlusTree *tree, void *key, void *value) {
    if (!tree->root) {
        tree->root = create_node(true);
        tree->root->keys[0] = key;
        tree->root->pointers[0] = value;
        tree->root->num_keys = 1;
        return;
    }
    Node *curr = tree->root;
    while (!curr->is_leaf) {
        int i = 0;
        while (i < curr->num_keys && tree->cmp(key, curr->keys[i]) >= 0) i++;
        curr = curr->pointers[i];
    }
    int i = 0;
    while (i < curr->num_keys && tree->cmp(curr->keys[i], key) < 0) i++;
    for (int j = curr->num_keys; j > i; j--) {
        curr->keys[j] = curr->keys[j-1];
        curr->pointers[j] = curr->pointers[j-1];
    }
    curr->keys[i] = key;
    curr->pointers[i] = value;
    curr->num_keys++;
    if (curr->num_keys == MX_KEYS) {
        Node *new_leaf = create_node(true);
        int split = (MX_KEYS + 1) / 2;
        for (int j = split, k = 0; j < MX_KEYS; j++, k++) {
            new_leaf->keys[k] = curr->keys[j];
            new_leaf->pointers[k] = curr->pointers[j];
            new_leaf->num_keys++;
        }
        curr->num_keys = split;
        new_leaf->nxt = curr->nxt;
        curr->nxt = new_leaf;
        insert_into_parent(tree, curr, new_leaf->keys[0], new_leaf);
    }
}
void* bplus_tree_find(BPlusTree *tree, void *key) {
    if (!tree->root) return NULL;
    Node *curr = tree->root;
    while (!curr->is_leaf) {
        int i = 0;
        while (i < curr->num_keys && tree->cmp(key, curr->keys[i]) >= 0) i++;
        curr = curr->pointers[i];
    }
    for (int i = 0; i < curr->num_keys; i++) {
        if (tree->cmp(curr->keys[i], key) == 0) return curr->pointers[i];
    }
    return NULL;
}
void free_node(Node *node) {
    if (!node) return;
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) free_node(node->pointers[i]);
    }
    free(node);
}
void bplus_tree_destroy(BPlusTree *tree) {
    if (tree) {
        free_node(tree->root);
        free(tree);
    }
}