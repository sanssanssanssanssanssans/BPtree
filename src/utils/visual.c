#include "visual.h"
#include <stdio.h>
void bplus_tree_print(BPlusTree *tree) {
    if (!tree || !tree->root) return;
    Node *queue[1024];
    int head = 0, tail = 0;
    queue[tail++] = tree->root;
    while (head < tail) {
        int size = tail - head;
        while (size--) {
            Node *n = queue[head++];
            printf("[");
            for (int i = 0; i < n->num_keys; i++)
                printf("%d%c", *(int*)n->keys[i], (i == n->num_keys - 1 ? ']' : ' '));
            printf("  ");
            if (!n->is_leaf) {
                for (int i = 0; i <= n->num_keys; i++) queue[tail++] = n->pointers[i];
            }
        }
        printf("\n");
    }
}