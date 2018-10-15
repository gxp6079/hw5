#include "tree.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int line = 0;

int compare(char * original, const char * current){
    if(strlen(original) >= strlen(current)){
        for(int i = 0 ; i < strlen(current) ; i++){
            if(original[i] != current[i]){
                return (int)current[i] - (int)original[i];
            }
        }
        return -1;
    }
    else{
        for(int i = 0 ; i < strlen(original) ; i++){
            if(original[i] != current[i]){
                return (int)current[i] - (int)original[i];
            }
        }
        return 1;
    }
}

TreeNode new_node(char * word){
    TreeNode node;
    node.left = NULL;
    node.right = NULL;
    node.word = (char *) malloc(strlen(word));
    for(int i = 0 ; i < strlen(word); i++){
        node.word[i] = word[i];
    }
    node.frequency = 1;
    node.refLength = 1;
    node.references = (unsigned int *) malloc(1);
    node.references[0] = line;
    return node;
}

void insert_word(TreeNode **root, const char *word){
    if((*root) != NULL){
        if(!strcasecmp((*root)->word, word)){
            (*root)->frequency ++;
            (*root)->refLength ++;
            (*root)->references = (unsigned int *) realloc((*root)->references, (*root)->refLength);
            (*root)->references[(*root)->refLength - 1] = line;
        }
        else if (compare((*root)->word, word) < 0){
            if((*root)->left != NULL) {
                TreeNode *pointer = (*root)->left;
                TreeNode ** new_root = &pointer;
                return insert_word(new_root, word);
            }else{
                TreeNode node = new_node(word);
                (*root)->left = &node;
            }
        }
        else{
            if((*root)->right != NULL) {
                TreeNode *pointer = (*root)->right;
                TreeNode ** new_root = &pointer;
                return insert_word(new_root, word);
            }else{
                TreeNode node = new_node(word);
                (*root)->right = &node;
            }
        }
    }else{
        TreeNode node = new_node(word);
        *root = &node;
    }
}

void traverse_tree(const TreeNode *root){
    if(root != NULL){
        traverse_tree(root->left);
        //printf("%s (%d):", root->word, root->refLength);
        for(int i = 0; i < root->refLength ; i++){
            if(i == root->refLength-1){
                printf("\t%d", root->references[i]);
            } else{
                printf("\t%d,", root->references[i]);
            }
        }
        traverse_tree(root->right);
    }
}

void cleanup_tree(TreeNode *root){
    if(root != NULL){
        free(root->word);
        free(root->references);
        cleanup_tree(root->left);
        cleanup_tree(root->right);
    }
    return;
}

int main(void){
    char * buf = NULL;
    size_t len = 0;
    TreeNode * root;
    while(getline(&buf, &len, stdin) > 0){
        int i = 0;
        while(buf != NULL){
            int k = 0;
            while(isalnum(buf[i])){
                i++;
                k++;
            }
            char * word = calloc(k, sizeof(char));
            for(int j = 0; j < k; j++){
                word[j] = buf[j];
            }
            insert_word(&root, word);
            free(word);
            while(!isalnum(buf[i])){
                i++;
            }
        }
        line++;
        //traverse_tree(root);
    }
    traverse_tree(root);
    if(buf != NULL){
        free(buf);
    }
    cleanup_tree(root);
    return 0;
}
