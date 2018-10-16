#include "tree.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define _DEFAULT_SOURCE

unsigned int line = 0;

TreeNode new_node(char * word){
    TreeNode node;
    node.right = NULL;
    node.left = NULL;
    node.references = malloc(sizeof(unsigned int));
    node.references[0] = line;
    node.refLength = 0;
    node.frequency = 1;
    node.word =  calloc(strlen(word), sizeof(char));
    for(int i = 0 ; i < strlen(word) ; i++){
        node.word[i] = word[i];
    }
    return node;
}

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
        TreeNode node;
        node = new_node(word);
        (*root) = &node;
    }
}

void traverse_tree(const TreeNode *root){
}

void cleanup_tree(TreeNode *root){

}

int main(void){
    char *buf = NULL;  // important: must initially be NULL
    size_t len = 0;    // should initially be 0
    TreeNode * root;
    char * w;
    while( getline(&buf,&len,stdin) > 0 ) {
        int size = strlen(buf);
        int i = 0;
        int las_i = 0;
        while(i < size){
            while (isalnum(buf[i]) || buf[i] == '-' || buf[i] == '\''){
                i++;
            }
            int j = 0;
            w = calloc((i+1)-las_i, sizeof(char));
            for(int k = las_i ; k < i ;k++){
                w[j] = buf[k];
                j++;
            }
            insert_word(&root, w);
            while (!isalnum(buf[i]) && i < size  && !(buf[i] == '-' || buf[i] == '\'')){
                i++;
            }
            las_i = i;
        }
    }
    traverse_tree(root);

    if( w != NULL) {
        free(w);
    }

    if( buf != NULL ) {
        free( buf );  // must deallocate this space!
    }

    return( 0 );
}
