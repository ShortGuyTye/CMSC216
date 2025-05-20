// treemap_funcs.c: Provides a small library of functions that operate on
// binary search trees mapping strings keys to string values.

#include "treemap.h"

//root null
void treemap_init(treemap_t *tree){
    tree->root = NULL;
}

int treemap_add(treemap_t *tree, char key[], char val[]){
    //check if there is any nodes
    if (tree->root == NULL){
        //make first node if none
        tmnode_t *node = malloc(sizeof(tmnode_t));
        strcpy(node->key, key);
        strcpy(node->val, val);
        node->left = NULL;
        node->right = NULL;
        tree->root = node;
        return 1;
    }
    //check which way to go
    if (strcmp(tree->root->key, key) == 0){
        strcpy(tree->root->val, val);
        return 0;
    } else if (strcmp(tree->root->key, key) > 0){
        //check if left is empty and if is set
        if (tree->root->left == NULL){
            tmnode_t *node = malloc(sizeof(tmnode_t));
            strcpy(node->key, key);
            strcpy(node->val, val);
            node->left = NULL;
            node->right = NULL;
            tree->root->left = node;
            return 1;
        //if not, recursvie with new root being left
        } else {
            tmnode_t *temp = tree->root;
            tree->root = tree->root->left;
            int ret = treemap_add(tree, key, val);
            tree->root = temp;
            return ret;
        }

    } else {
        //check right
         if (tree->root->right == NULL){
            tmnode_t *node = malloc(sizeof(tmnode_t));
            strcpy(node->key, key);
            strcpy(node->val, val);
            node->left = NULL;
            node->right = NULL;
            tree->root->right = node;
            return 1;
        //if not, recursvie with new root being right
        } else {
            tmnode_t *temp = tree->root;
            tree->root = tree->root->right;
            int ret = treemap_add(tree, key, val);
            tree->root = temp;
            return ret;
        }

    }
    return 1;
}

char *treemap_get(treemap_t *tree, char key[]){
    //check if tree is empty
    if (tree->root == NULL){
        return NULL;
    }
    //check if root is it
    if (strcmp(tree->root->key, key) == 0){
        return tree->root->val;
    //check if left
    } else if (strcmp(tree->root->key, key) > 0){
        if (tree->root->left == NULL){
            return NULL;
        //if left exists recurse on new root
        } else {
            tmnode_t *temp = tree->root;
            tree->root = tree->root->left;
            char *ret = treemap_get(tree, key);
            tree->root = temp;
            return ret;
        }

    } else {
        //check right
         if (tree->root->right == NULL){
            return NULL;
        //if exists recurse
        } else {
            tmnode_t *temp = tree->root;
            tree->root = tree->root->right;
            char *ret = treemap_get(tree, key);
            tree->root = temp;
            return ret;
        }

    }
    return NULL;
}

void treemap_clear(treemap_t *tree){
    //call recursive function and set null
    node_remove_all(tree->root);
    tree->root = NULL;
}

void node_remove_all(tmnode_t *cur){
    //stop if root is null
    if (cur == NULL){
        return;
    }
    //if left node exists then recurse on it
    if (cur->left != NULL){
        node_remove_all(cur->left);
    }
    //if right node, then recurse
    if (cur->right != NULL){
        node_remove_all(cur->right);
    }
    //free node when all children checked
    free(cur);
}

int treemap_size(treemap_t *tree){
    //add one for the root
    return node_count_all(tree->root) + 1;
}

int node_count_all(tmnode_t *cur){
    //stop if root null
    if (cur == NULL){
        return -1;
    }
    int total = 0;
    //if left exists, recurse on it
    if (cur->left != NULL){
        total++;
        total += node_count_all(cur->left);
    }
    //if right exists, recurse on it
    if (cur->right != NULL){
        total++;
        total += node_count_all(cur->right);
    }
    //return number of children and current
    return total;
}

void treemap_print_revorder(treemap_t *tree){
    //call recursive function
    node_print_revorder(tree->root, 0);
}

void node_print_revorder(tmnode_t *cur, int indent){
    //if root null, stop
    if (cur == NULL){
        return;
    }
    //check if most right item
    if (cur->right == NULL){
        printf("%*s%s -> %s\n", indent, "", cur->key, cur->val);
    //if right does exist, recurse on it
    } else if (cur->right != NULL){
        node_print_revorder(cur->right, indent + 2);
        printf("%*s%s -> %s\n", indent, "", cur->key, cur->val);
    }
    //if left exists, recurse on it
    if (cur->left != NULL){
        node_print_revorder(cur->left, indent + 2);
    }
}

void treemap_print_preorder(treemap_t *tree){
    //call recursive function
    node_write_preorder(tree->root, NULL, 0);
}

void treemap_save(treemap_t *tree, char *fname){
    //create file and call recursive function
    FILE *file = fopen(fname, "w");
    node_write_preorder(tree->root, file, 0);
    fclose(file);
}

void node_write_preorder(tmnode_t *cur, FILE *out, int depth){
    //if root no, stop
    if (cur == NULL){
        return;
    }
    //checks if printing or writing to file
    if (out != NULL){
        fprintf(out, "%*s%s %s\n", depth, "", cur->key, cur->val);
    } else {
        printf("%*s%s %s\n", depth, "", cur->key, cur->val);
    }
    //if left exists, recurse on it
    if (cur->left != NULL){
        node_write_preorder(cur->left, out, depth + 1);
    }
    //if right exists, recurse on it
    if (cur->right != NULL){
        node_write_preorder(cur->right, out, depth + 1);
    }
}

int treemap_load(treemap_t *tree, char *fname ){
    //open file and checks if succeeded
    FILE *file = fopen(fname, "r");
    if (file == NULL){
        return 0;
    }
    //clear tree for writing and set strings
    treemap_clear(tree);
    char key[128];
    char val[128];
    //loop through file
    while (1){
        fscanf(file, "%s", key);
        int ret = fscanf(file, "%s", val);
        //add to tree
        treemap_add(tree, key, val);
        //break at end of file
        if (ret == EOF){
            break;
        }
    }
    return 1;
}