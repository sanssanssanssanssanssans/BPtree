#include "../src/bplus.h"
#include "../src/utils/visual.h"
#include <stdio.h>
int cmp_int(void *a, void *b) { return (*(int*)a - *(int*)b); }
int main() {
    BPlusTree *tree = bplus_tree_create(cmp_int);
    int keys[] = {1, 4, 7, 2, 5, 8, 3, 6, 9, 10, 11};
    for(int i = 0; i < 11; i++) {
        bplus_tree_insert(tree, &keys[i], "Data");
        bplus_tree_print(tree);
    }
    int target = 7;
    if (bplus_tree_find(tree, &target)) printf("Found key 7!\n");
    bplus_tree_destroy(tree);
    return 0;
}