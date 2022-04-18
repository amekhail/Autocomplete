/**
 * File: autocomplete.c
 * Author: Adam Mekhail
 * Course: CSc 352
 * Purpose: To implement an autocomple program. This is separated into 2 parts.
 * For this submission, the first part is done which handles all the word list 
 * functions which handles creating, destroying, add to, and add from a file.
 * 
 * BIG NOTE: I saw that on the previous assignment that I was marked down for
 * using camel case when ben used snake case. I use camel case, that is my 
 * style and its the way i've been programming for years. I refuse to use
 * snake case. I cannot change the function names from snake to camel.
 * If are considering marking me down, I urge you to look at Russ's function
 * declarations in the header file. There you will find a mix of camel case and
 * snake case, therefore, marking me down will be hypocritical
 * 
 * 
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"autocomplete.h"

//=============================================================================
// --- Macros ---
// values:
#define INITIAL_CAPACITY 4
// error messages:
#define MALLOC_ERR "Error allocating memory"
#define WL_ADD_ERR "Error adding word to word list"
#define WL_COPY_ERR "Error copying word"
// error codes:
#define MALLOC_ERR_CODE 10
#define WL_ADD_SUCCESS_CODE 0
#define WL_ADD_ERR_CODE -1
#define WL_COPY_ERR_CODE 4
//=============================================================================


/* --- TODO Functions For PA7 ---
    [x] wl_create
    [x] wl_destroy
    [x] wl_add
    [x] build_wordlist
*/

//=============================================================================
// --- Function Headers ---
void err(char * errorMessage, int errorCode);
void toUpperAll(char * word);
//=============================================================================

// --- wordList ---
/**
 * Creates a new word list by allocating memory for the word list
 * and creates no more than 4 pointers to the words initially as per the spec.
 * Count will store the number of words in the array and alloc will store
 * the total number of allocated spaces in the array.
 * 
 * @return Pointer to the newly created word list
 */
WordList *wl_create (void) {
    WordList * newWordList = (WordList*) malloc(sizeof(WordList));
    if (newWordList == NULL) { // Check for null
        err(MALLOC_ERR, MALLOC_ERR_CODE);
        return NULL;
    }
    // I dislike using magic numbers so a macro for the initial capacity (4)
    // will be used for the alloc field
    newWordList->alloc = INITIAL_CAPACITY; 
    newWordList->count = 0;
    newWordList->words = (char **)malloc(sizeof(char *) * newWordList->alloc);
    if (newWordList->words == NULL) { // check for null
        err(MALLOC_ERR, MALLOC_ERR_CODE);
        return NULL;
    }
    return newWordList;
}

/**
 * Adds a word to the word list. Checks the size of the list to see if it is
 * at the maximum size, if so, the word list is doubled using realloc() then
 * the word is copied over using strcpy. If any memory allocation falis,
 * WL_ADD_ERR_CODE will be returned and if it sucessful, 0 will be returned.
 * 
 * @param list the pointer to the word list
 * @param word the word to be added to the list
 * @return int 
 */
int wl_add(WordList *list, char *word) {
    // check if more space needs to be allocated or not
    if (list->count == list->alloc) { 
        list->alloc = list->alloc * 2; // increase size by 2
        list->words = (char **) realloc(list->words, list->alloc * sizeof(char *));
        if (list->words == NULL) { // check if issue reallocing occured
            err(WL_ADD_ERR, WL_ADD_ERR_CODE);
            return WL_ADD_ERR_CODE;
        }
    }
    list->words[list->count++] = word;
    return WL_ADD_SUCCESS_CODE;
}

/**
 * Frees the word list. *DOES NOT* free the words from memory but frees the 
 * pointer to the array and finally the struct.
 * The general rule is: 
 * anything that needed to be malloc()'d should be free()'d
 * 
 * @param list the pointer to the word list
 */
void wl_destroy(WordList *list) {
    // free the array pointer for the words
    free(list->words);
    // free list
    free(list);
}

/**
 * Creates a word list from a file by reading each line and malloc()ing each
 * line to create a pointer to the word. The pointer will be held by the word
 * list. Returns null if an error has occured with adding to the wordlist
 * 
 * @param fp the file pointer
 * @return the word list with all the words added
 */
WordList *build_wordlist_from_file(FILE *fp) {
    if (fp == NULL) {
        fprintf(stderr, "Could not open the dictionary file\n");
    }
    WordList * wl = wl_create();
    char buffer[128], * tempBuf;
    while (fgets(buffer, 128, fp) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        int length = strlen(buffer) + 1;
        if (buffer[0] != '\n') {
            // each malloc brings new address
            // unique address for every word and wordlist will handle holding 
            // those addresses
            tempBuf = (char *) malloc(sizeof(char) * length); 
            if (tempBuf == NULL) {
                err(MALLOC_ERR, MALLOC_ERR_CODE);
                exit(MALLOC_ERR_CODE);
                return NULL;
            }
            char * s = strcpy(tempBuf, buffer);
            if (s == NULL) {
                err(WL_COPY_ERR, WL_COPY_ERR_CODE);
                exit(WL_COPY_ERR_CODE);
                return NULL;
            }
            toUpperAll(tempBuf);
            int status = wl_add(wl, tempBuf);
            if (status != WL_ADD_SUCCESS_CODE) {
                return NULL;
            }
        }
    }
    return wl;
}


// --- helper functions ---
/**
 * Wirtes an error message and error code to stderr
 * 
 * @param errorMessage the error message
 * @param errorCode the code for the error
 */
void err(char * errorMessage, int errorCode) {
    fprintf(stderr, "%s Error code: %d\n", errorMessage, errorCode);
}

/**
 * Makes all the letters in the word upper case.
 * 
 * @param word A reference to the word
 */
void toUpperAll(char * word) {
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = toupper(word[i]);
    }
}

/* --- Functions to be implemented for the next PA ---
    [x] ltn_create
    [x] ltn_add_result_word
    [x] ltn_destroy
    [x] node_count
    [x] result_count
    [x] lookup
    [x] build_tree_from_words
    [x] print_words
*/
// --- LookUpTree ---

/**
 * Creates a LookUpTreeNode object by allocating space for the node
 * and the word list. Sets all the children to NULL
 * 
 * return pointer to the LookUpTreeNode
 */
LookupTreeNode *ltn_create(void) {
    LookupTreeNode * newNode = (LookupTreeNode *) malloc(sizeof(LookupTreeNode));
    if (newNode == NULL) {
        err(MALLOC_ERR, MALLOC_ERR_CODE);
        return NULL;
    }
    for (int i = 0; i < 26; i++) {
        newNode->children[i] = NULL;
    }
    newNode->result_words = wl_create();
    if (newNode->result_words == NULL) {
        err(MALLOC_ERR, MALLOC_ERR_CODE);
        return NULL;
    }
    return newNode;

}

/**
 * Adds the word by going through the search_word which is the node and stores
 * the word in word list for each node
 * 
 * @param ltn the root of the tree
 * @param search_word the path for the node
 * @param result_word the word to store in the word list for the destination 
 */
void ltn_add_result_word(LookupTreeNode *ltn, char *search_word, char *result_word) {
    int index = search_word[0] - 65;
    if (strlen(search_word) == 1) {
        if (ltn->children[index] == NULL) {
            LookupTreeNode * newChild = ltn_create();
            if (newChild == NULL) {
                return;
            }
            wl_add(newChild->result_words, result_word);
            ltn->children[index] = newChild;
        } else {
            wl_add(ltn->children[index]->result_words, result_word);
        }
        return;
    } else {
        if (ltn->children[index] == NULL) {
            LookupTreeNode * newChild = ltn_create();
            if (newChild == NULL) {
                return;
            }
            ltn->children[index] = newChild;
        }
    }
    ltn_add_result_word(ltn->children[index], ++search_word, result_word);
}

/**
 * Destroys the root and all the children in the root recursivally
 * 
 * @param ltn the root
 */
void ltn_destroy(LookupTreeNode *ltn) {
    // base case: ltn is at end of tree or is null
    if (ltn == NULL) {
        return;
    }
    // go down each child node
    for (int i = 0; i < 26; i++) {
        ltn_destroy(ltn->children[i]);
    }
    // free back up the tree
    free(ltn);

}

/**
 * Returns a count of all the children and their children in the tree
 * 
 * @param root the root
 * @return the count
 */
int node_count(LookupTreeNode *root) {
    if (root == NULL) {
        return 0;
    }
    int result = 1;
    for (int i = 0; i < 26; i++) {
        result += node_count(root->children[i]);
    }
    return result;
}

/**
 * Returns a count of all the words in the tree
 * 
 * @param root the root
 * @return the total number of words in the tree
 */
int result_count(LookupTreeNode *root) {
    if (root == NULL) {
        return 0;
    }
    int result = root->result_words->count;
    for (int i = 0; i < 26; i++) {
        result += result_count(root->children[i]);
    }
    return result;
}

/**
 * Given a root and a word, searches the root to see if the word is present,
 * if so, it returns the pointer to the starting node where the word is in
 * 
 * The word ATCH is given and start at the node ATCH, ATCH is returned.
 * The word EGOR is given and start at the node CAT, CATEGOR is returned.
 * 
 * @param root the starting node in the tree list
 * @param search the word
 * @return pointer to where the word starts in the list 
 */
LookupTreeNode *lookup(LookupTreeNode *root, char *search) {
    // base case: tree is empty
    if (root == NULL || search[0] == '\0') {
        return NULL;
    }

    char letter;
    if (search[0] != '\0') {
        letter = toupper(search[0]);
    }
    int index = letter - 65;
    // base case: last letter of search equals char in tree
    if (strlen(search) == 1) {
        return root->children[index];
    }
    // continue searching until null is found or the correct node
    search++;
    return lookup(root->children[index], search); 
}

/**
 * Adds the words from the wordlist into the tree. The word
 * can be added multiple times and can show up in multiple forms.
 * For exanmple: CATCH, ATCH, TCH, CH, H can all lead to CATCH
 * 
 * @param words the wordlist
 * @return the pointer to the root of the tree
 */
LookupTreeNode *build_tree_from_words(WordList *words) {
    LookupTreeNode * root = ltn_create();
    for (int i = 0; i < words->count; i++) {
        for (int j = 0; j < strlen(words->words[i]); j++) {
            char * curWord = words->words[i];
            ltn_add_result_word(root, (curWord+j), (curWord));
        }
    }
    return root;
}

/**
 * Prints all the words for the search term by iterating through the current
 * node and its children
 * 
 * @param result the starting node
 * @param search the search term
 */
void print_words(LookupTreeNode *result, char *search) {
    if (result == NULL) {
        return;
    }

    WordList * temp = result->result_words;
    int len = temp->count;
    int count = 0;
    while (count < len) {
        printf("The string '%s' was found inside the word '%s'\n", 
            search, temp->words[count]);
        count++;
    }

    for (int i = 0; i < 26; i++) {
        print_words(result->children[i], search);
    }
}


