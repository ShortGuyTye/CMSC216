// Header for treemap problem
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Type of tree nodes
typedef struct node {
  char key[96];                 // key for the node
  char val[96];                 // value for the node
  struct node *left;            // left branch,  NULL if not present
  struct node *right;           // right branch, NULL if not present
} tmnode_t;

// Type of tree itself
typedef struct {
  tmnode_t *root;               // root of tree, NULL if tree empty
} treemap_t;

// tree functions which will be tested in binary
void treemap_init(treemap_t *tree);
int treemap_add(treemap_t *tree, char key[], char val[]);
char *treemap_get(treemap_t *tree, char key[]);
void treemap_clear(treemap_t *tree);
int treemap_size(treemap_t *tree);
void treemap_print_revorder(treemap_t *tree);
void treemap_print_preorder(treemap_t *tree);
void treemap_save(treemap_t *tree, char *fname);
int treemap_load(treemap_t *tree, char *fname);

// node functions, not tested but useful as helpers
char *node_get(tmnode_t *cur, char key[]);
void node_remove_all(tmnode_t *cur);
int node_count_all(tmnode_t *cur);
void node_print_revorder(tmnode_t *cur, int indent);
void node_write_preorder(tmnode_t *cur, FILE *out, int depth);
