#include<stdio.h>
#include<stdlib.h>
#include"autocomplete.h"


int main() {
    FILE * dict = fopen("dict.txt", "r");
    WordList * wl = build_wordlist_from_file(dict);
    LookupTreeNode * test = build_tree_from_words(wl);
    
    printf("node count: %d\n", node_count(test));
    printf("result count: %d\n", result_count(test));
   
    return 0;
}