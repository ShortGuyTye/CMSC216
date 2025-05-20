#include "treemap.h"
#include <stdio.h>

int main(int argc, char *argv[]){
    int echo = 0;
    if(argc > 1 && strcmp("-echo",argv[1])==0) {
     echo=1;
    }
    treemap_t *treemap = malloc(sizeof(treemap_t));
    treemap->root = NULL;
    printf("TreeMap Editor\n");
    printf("Commands:\n");
    printf("  quit:            exit the program\n");
    printf("  print:           shows contents of the tree in reverse sorted order\n");
    printf("  add <key> <val>: inserts the given key/val into the tree, duplicate keys are ignored\n");
    printf("  get <key>:       prints FOUND if the name is in the tree, NOT FOUND otherwise\n");
    printf("  clear:           eliminates all key/vals from the tree\n");
    printf("  size:            prints the total number of nodes in the tree\n");
    printf("  preorder:        prints contents of the tree in pre-order which is how it will be saved\n");
    printf("  save <file>:     writes the contents of the tree in pre-order to the given file\n");
    printf("  load <file>:     clears the current tree and loads the one in the given file\n");

    char cmd[128];
    int success;

    while(1){
        printf("TM> ");                 // print prompt
        success = fscanf(stdin,"%s",cmd); // read a command
        if(success==EOF){                 // check for end of input
            printf("\n");                   // found end of input
            break;                          // break from loop
        }
        //quit
        if( strcmp("quit", cmd)==0 ){
            if(echo){ 
                printf("quit\n");
            }
            break;
        }
        //print
        if( strcmp("print", cmd)==0 ){
            if(echo){ 
                printf("print\n");
            }
            treemap_print_revorder(treemap);
        }
        //add
        if( strcmp("add", cmd)==0 ){
            fscanf(stdin,"%s",cmd);
            char add_temp[128];
            fscanf(stdin,"%s",add_temp);
            if(echo){ 
                printf("add %s %s\n",cmd, add_temp);
            }
            int add_ret = treemap_add(treemap, cmd, add_temp);
            //check for already exists
            if (add_ret == 0){
                printf("modified existing key\n");
            }
        }
        //get
        if( strcmp("get", cmd)==0 ){
            fscanf(stdin,"%s",cmd);
            if(echo){ 
                printf("get %s\n",cmd);
            }
            char *get_ret = treemap_get(treemap, cmd);
            //check if found or not
            if (get_ret == NULL){
                printf("NOT FOUND\n");
            } else {
                printf("FOUND: %s\n", get_ret);
            }
        }
        //size
        if( strcmp("size", cmd)==0 ){
            if(echo){ 
                printf("size\n");
            }
            printf("%d nodes\n", treemap_size(treemap));
        }
        //preorder
        if( strcmp("preorder", cmd)==0 ){
            if(echo){ 
                printf("preorder\n");
            }
            treemap_print_preorder(treemap);
        }
        //clear
        if( strcmp("clear", cmd)==0 ){
            if(echo){ 
                printf("clear\n");
            }
            treemap_clear(treemap);
        }
        //save
        if( strcmp("save", cmd)==0 ){
            fscanf(stdin,"%s",cmd);
            if(echo){ 
                printf("save %s\n",cmd);
            }
            treemap_save(treemap, cmd);
        }
        //load
        if( strcmp("load", cmd)==0 ){
            fscanf(stdin,"%s",cmd);
            if(echo){ 
                printf("load %s\n",cmd);
            }
            int load_ret = treemap_load(treemap, cmd);
            //check if file loaded
            if (load_ret == 0){
                printf("ERROR: could not open file '%s'\n", cmd);
                printf("load failed\n");
            }
        }

  } 
    //free treemap
    treemap_clear(treemap);
    free (treemap);
    return 1;
}