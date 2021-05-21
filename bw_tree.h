#ifndef BW_TREE
#define BW_TREE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include <math.h>

typedef unsigned key_type;

typedef struct _Tree_element{
	key_type key;
	struct _LIST *data_list;
	struct _Tree_element *left;
	struct _Tree_element *right;
	int balance;
} Tree_element;

typedef struct _Tree{
	struct _Tree_element *root;
} Tree;

char *fget_str(FILE *file);
int get_unint(unsigned *a);
int get_int(int *a);
key_type input_key(void);
Tree *create_tree(void);
void insert(Tree* tree, key_type key, char *string);
int print_tree(Tree* tree);
char *get_str();
void del_tree(Tree *tree);
void remove_element(Tree *tree, key_type key);
int print_beauty_tree(Tree *tree);
void lets_balanced_it(Tree *tree);
Tree_element *find_scapegoat(Tree *tree, Tree_element **scapegoat_parent);
void print_info_by_key(Tree *tree, key_type key);
Tree_element *get_biggest_among_smaller(Tree *tree, key_type pending);
void input_from_file(Tree *tree);
void gv(Tree *tree);


#endif