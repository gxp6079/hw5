//
//file: main.c
// @author: GIovana Puccini
// @email: gxp6079@rit.edu
// ________________________________________________________

#include "tree.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int line = 0; //keeps track of the line number

/*
 * new_node creates a new node
 * @param word - character array containing the word to be added to the node
 * @return node - a new TreeNode containing word, a NULL left and right nodes,
 * a references list with the current line number
 */
TreeNode new_node(const char * word){
    TreeNode node;
    node.right = NULL;
    node.left = NULL;
    node.references = malloc(sizeof(unsigned int));
    node.references[0] = line;
    node.refLength = 1;
    node.frequency = 1;
    node.word =  calloc(strlen(word)+1, sizeof(char));
    strcpy(node.word, word);
    return node;
}

/*
 * insert_word - adds a new word to the tree (if the word is not there, it creates
 * a new node for it, if it is, it adds another occurrence to the existing node
 * @param root - a pointer to a pointer containing the root node of the tree
 * @param word - character array containing the word to be added to the tree
 */
void insert_word(TreeNode **root, const char *word){
    if((*root) != NULL){
        if(!strcasecmp((*root)->word, word)){
            (*root)->refLength += 1;
            (*root)->references = realloc((*root)->references, sizeof(unsigned int));
            (*root)->references[(*root)->refLength-1] = line;
            (*root)->frequency += 1;
        }else if(strcasecmp((*root)->word, word) < 0){
            insert_word(&((*root)->right), word);
        } else{
            insert_word(&((*root)->left), word);
        }
    } else{
        (*root)= malloc(sizeof(TreeNode));
        (**root) = new_node(word);
    }
}

/*
 * traverse_tree pretty prints the content on the tree in alphabetical order
 * @param root - pointer to the root node of the tree
 */
void traverse_tree(const TreeNode *root){
    if(root != NULL){
        traverse_tree(root->left);
        printf("%s (%d):", root->word, root->frequency);
        for (int i = 0; i < root->refLength - 1; ++i) {
            printf(" %d,", root->references[i]);
        }
        printf(" %d\n", root->references[root->refLength - 1]);
        traverse_tree(root->right);
    }
}

/*
 * cleanup_tree frees all the allocated memory in each node of the tree
 * @param root - pointer to the root node of the tree
 */
void cleanup_tree(TreeNode *root){
    if(root != NULL){
        free(root->word);
        free(root->references);
        cleanup_tree(root->right);
        cleanup_tree(root->left);
    }
}

/*
 * main reads the standard input and adds all the words read to the tree
 * duplicates are added to the same node
 * the tree keeps track of the lines the word was seen and the amount of occurrences
 */
int main(void){
    char *buf = NULL;  // important: must initially be NULL
    size_t len = 0;    // should initially be 0
    TreeNode * root = NULL;
    char * w;
    while( getline(&buf,&len,stdin) > 0 ) {
        int size = strlen(buf);
        int i = 0;
        int las_i = 0; //keeps track of the lass index read before the new word began
        while(i < size){
            //reads until a character that does not belong to a word is read
            while (isalnum(buf[i]) || buf[i] == '-' || buf[i] == '\''){
                i++;
            }
            int j = 0;
            w = calloc((i+2)-las_i, sizeof(char));
            strncpy(w, buf+las_i, i - las_i);
            insert_word(&root, w);
            free(w); //frees the allocated memory for w
            //reads the buffer until it finds the beginning of the next word
            while (!isalnum(buf[i]) && i < size  && !(buf[i] == '-' || buf[i] == '\'')){
                i++;
            }
            las_i = i;
        }
    }
    traverse_tree(root);
    cleanup_tree(root);
    if( buf != NULL ) {
        free( buf );  // must deallocate this space!
    }

    return( 0 );
}
