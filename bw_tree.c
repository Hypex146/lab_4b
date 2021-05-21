#include "bw_tree.h"

char *get_str(){
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;
    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            if (!res) {
                return NULL;
            }
        } else if (n > 0) {
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        } else {
            scanf("%*c");
        }
    } while (n > 0);

    if (len > 0) {
        res[len] = '\0';
    } else {
        res = calloc(1, sizeof(char));
    }
    return res;
}


char *fget_str(FILE *file){
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;
    do {
        n = fscanf(file, "%80[^\n]", buf);
        if (n == EOF) return NULL;
        else if (n < 0) {
            if (!res) {
                return NULL;
            }
        } else if (n > 0) {
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        } else {
            fscanf(file, "%*c");
        }
    } while (n > 0);

    if (len > 0) {
        res[len] = '\0';
    } else {
        res = calloc(1, sizeof(char));
    }

    return res;
}


int get_unint(unsigned *a){
    int n;
	int tmp;
    do{
        n = scanf("%u", &tmp);
		if (tmp<0) n=0;
        if (n < 0) return 1;
        if (n == 0){
            printf("Error!\n");
            scanf("%*c");
        }
    } while (n == 0);
	*a=(unsigned)tmp;
    return 0;
}


int get_int(int *a){
    int n;
    do{
        n = scanf("%d", a);
        if (n < 0) return 1;
        if (n == 0){
            printf("Error!\n");
            scanf("%*c");
        }
    } while (n == 0);
    return 0;
}


key_type input_key(void){
	key_type key;
	printf("Enter the key:\n");
	get_unint(&key);
	return key;
}


Tree *create_tree(void){
	Tree *tree = (Tree*)malloc(sizeof(Tree));
	tree->root = NULL;
	return tree;
}


static Tree_element *get_ptr_by_key_without_parent(Tree *tree, key_type key){
	if (tree->root){
		Tree_element *now = tree->root;
		while(now){
			if (now->key==key) return now;
			else if (now->key>key) now = now->left;
			else now = now->right;
		}
		//printf("Element not found\n");
		return NULL;
	} else{
		//printf("Tree is empty\n");
		return NULL;
	}
}


static Tree_element *create_tree_element(key_type key, char *string, int balance){
	Tree_element *tree_element = (Tree_element*)malloc(sizeof(Tree_element));
	tree_element->key = key;
	tree_element->data_list = list_init();
	list_insert(tree_element->data_list, string, list_len(tree_element->data_list));
	tree_element->balance = balance;
	tree_element->left = tree_element->right = NULL;
	return tree_element;
}


static void del_element(Tree_element *element){
	list_clear(element->data_list);
	free(element);
}


static Tree_element *get_ptr_by_key(Tree *tree, key_type key, Tree_element **parent){
	*parent = NULL;
	if (tree->root){
		Tree_element *now = tree->root;
		while(now){
			if (now->key==key) return now;
			else if (now->key>key) {
				*parent = now;
				now = now->left;
			}
			else {
				*parent = now;
				now = now->right;
			}
		}
		//printf("Element not found\n");
		return NULL;
	} else{
		//printf("Tree is empty\n");
		return NULL;
	}
}


static void count_element_rec(Tree_element *element, int *count){
	if (element){
		(*count)++;
		count_element_rec(element->left, count);
		count_element_rec(element->right, count);
	}
}


static int count_element(Tree_element *element){
	int count = 0;
	count_element_rec(element, &count);
	return count;
}


static int calc_balance_for_element(Tree_element *element){
	return count_element(element->right) - count_element(element->left);
}


static void calc_balance_rec(Tree_element *element){
	if (element){
		element->balance = calc_balance_for_element(element);
		calc_balance_rec(element->right);
		calc_balance_rec(element->left);
	}
}


static void calc_balance(Tree *tree){
	if (tree->root){
		calc_balance_rec(tree->root);
	} else{
		printf("Tree is empty\n");
	}
}


void insert(Tree* tree, key_type key, char *string){
	if (!tree->root){
		tree->root = create_tree_element(key, string, 0);
		lets_balanced_it(tree);
		return;
	}
	Tree_element *now = get_ptr_by_key_without_parent(tree, key);
	if (now){
		printf("~~~We have already the same key~~~\n");
		list_insert(now->data_list, string, list_len(now->data_list));
		lets_balanced_it(tree);
		return;
	}
	now = tree->root;
	while(now){
		if (key>now->key){
			if (now->right){
				now->balance++;
				now = now->right;
				continue;
			} else{
				now->balance++;
				now->right = create_tree_element(key, string, 0);
				lets_balanced_it(tree);
				return;
			}
		} else if (key<now->key){
			if (now->left){
				now->balance--;
				now = now->left;
				continue;
			} else{
				now->balance--;
				now->left = create_tree_element(key, string, 0);
				lets_balanced_it(tree);
				return;
			}
		}
	}
	lets_balanced_it(tree);
}


static void insert_element(Tree *tree, Tree_element *element){
	if (!tree->root){
		tree->root = element;
		return;
	}
	Tree_element *now = tree->root;
	while(now){
		if (element->key>now->key){
			if (now->right){
				now = now->right;
				continue;
			} else{
				now->right = element;
				return;
			}
		} else if (element->key<now->key){
			if (now->left){
				now = now->left;
				continue;
			} else{
				now->left = element;
				return;
			}
		}
	}
}


static void find_scapegoat_rec(Tree_element *element, Tree_element *prev_element, Tree_element **scapegoat, Tree_element **parent, int *flag){
	if (element && !(*flag)){
		//printf("E1\n");
		if (abs(element->balance)>1) {
			*flag = 1;
			*scapegoat = element;
			*parent = prev_element;
		}
		//if (!(*flag)) *parent = element;
		find_scapegoat_rec(element->right, element, scapegoat, parent, flag);
		find_scapegoat_rec(element->left, element, scapegoat, parent, flag);
	}
}


/*static*/ Tree_element *find_scapegoat(Tree *tree, Tree_element **scapegoat_parent){
	if (tree->root){
		Tree_element *scapegoat = NULL;
		int flag = 0;
		find_scapegoat_rec(tree->root, NULL, &scapegoat, scapegoat_parent, &flag);
		if (!scapegoat) *scapegoat_parent = NULL;
		return scapegoat;
	} else{
		return NULL;
	}
}


static void add_scapegoat_element_to_mas(Tree_element *element, Tree_element **mas_element, int *index){
	if (element){
		add_scapegoat_element_to_mas(element->left, mas_element, index);
		mas_element[*index] = element;
		(*index)++;
		add_scapegoat_element_to_mas(element->right, mas_element, index);
		element->left = NULL;
		element->right = NULL;
	}
}


static void rebuild_scapegoat_tree(Tree *tree, Tree_element **mas_element, int left, int right){
	if (right>=left){
		int center = (left + right) / 2;
		//printf("%d\n", mas_element[center]->key);
		//printf("left -> %d | center -> %d | right -> %d\n", left, center, right);
		insert_element(tree, mas_element[center]);
		rebuild_scapegoat_tree(tree, mas_element, left, center-1);
		rebuild_scapegoat_tree(tree, mas_element, center+1, right);
	}
}


void lets_balanced_it(Tree *tree){
	Tree_element *scapegoat_parent = NULL;
	Tree_element *scapegoat = find_scapegoat(tree, &scapegoat_parent);
	int count_element_in_mas = count_element(scapegoat);
	int left_or_right = -1;
	if (scapegoat){
		//printf("Scapegoat -> %d\n", scapegoat->key);
		Tree_element **mas_element = (Tree_element**)malloc(sizeof(Tree_element*)*count_element_in_mas);
		int index = 0;
		add_scapegoat_element_to_mas(scapegoat, mas_element, &index);
		if (scapegoat_parent){
			//printf("For debug:\n");
			//printf("parent -> %d\n", scapegoat_parent->key);
			//printf("E1\n");
			if (scapegoat_parent->left==scapegoat) {
				scapegoat_parent->left = NULL;
				left_or_right = 0;
				//printf("E2\n");
			}
			else {
				scapegoat_parent->right = NULL;
				left_or_right = 1;
				//printf("E3\n");
			}
		} else{
			tree->root = NULL;
		}
		rebuild_scapegoat_tree(tree, mas_element, 0, count_element_in_mas - 1);
		//printf("EE1\n");
		if (left_or_right==0) {
			calc_balance_rec(scapegoat_parent->left);
			//printf("F1\n");
		} else if (left_or_right==1){
			calc_balance_rec(scapegoat_parent->right);
			//printf("F2\n");
		} else{
			calc_balance(tree);
		}
		free(mas_element);
	}
}


static Tree_element *find_max_element(Tree_element *element, Tree_element **parent){
	*parent = NULL;
	while(element->right) {
		*parent = element;
		element = element->right;
	}
	return element;
}


static void remove_element_ptr(Tree *tree, Tree_element *element, Tree_element *parent, int mod){
	if (element->left && element->right) {
		Tree_element *localMax_parent;
        Tree_element *localMax = find_max_element(element->left, &localMax_parent);
		if (!localMax_parent) localMax_parent = element;
		element->key = localMax->key;
		list_clear(element->data_list);
        element->data_list = localMax->data_list;
        remove_element_ptr(tree, localMax, localMax_parent, 0);
        return;
    } else if (element->left) {
		if (parent){
			if (element == parent->left) {
				parent->left = element->left;
			} else {
				parent->right = element->left;
			}
		} else{
			tree->root = element->left;
		}
    } else if (element->right){
		//printf("E4\n");
		if (parent){
			if (element == parent->right) {
				parent->right = element->right;
			} else {
				parent->left = element->right;
			}
		} else{
			tree->root = element->right;
		}
    } else {
		//printf("E5\n");
		if (parent){
			if (element == parent->left) {
				parent->left = NULL;
			} else {
				parent->right = NULL;
			}
		} else{
			tree->root = NULL;
		}
    }
	if (mod) del_element(element);
	else free(element);
}


void remove_element(Tree *tree, key_type key){
	Tree_element *parent;
	Tree_element *element = get_ptr_by_key(tree, key, &parent);
	if (element && list_len(element->data_list)>1){
		int index;
		printf("We have several item with this key, enter a index to del\n");
		get_int(&index);
		list_remove(element->data_list, index);
	} else{
		if (element) {
			remove_element_ptr(tree, element, parent, 1);
			calc_balance(tree);
		}
		else printf("Element not found\n");
	}
	lets_balanced_it(tree);
}


void print_info_by_key(Tree *tree, key_type key){
	Tree_element *element = get_ptr_by_key_without_parent(tree, key);
	if (element){
		char *string;
		if (list_len(element->data_list)>1){
			int index;
			printf("We have several item with this key, enter a index\n");
			get_int(&index);
			list_take(element->data_list, index, &string);
		} else{
			list_take(element->data_list, 0, &string);
		}
		printf("data -> %s\n", string);
	} else{
		printf("Element not found\n");
	}
}


Tree_element *get_biggest_among_smaller(Tree *tree, key_type pending){
	if (tree->root){
		Tree_element *now = tree->root;
		Tree_element *possible = NULL;
		while(now->right || now->left){
			if (now->key<pending){
				if (now->right) {
					//printf("E1\n");
					possible = now;
					now = now->right;
				}
				else return now;
			} else if (now->key>pending){
				//printf("E3\n");
				if (now->left) now = now->left;
				else {
					//printf("E2\n");
					if (possible) return possible;
					else{
						//printf("Element not found\n");
						return NULL;
					}
				}
			} else{
				return now;
			}
		}
		if (now->key<=pending) return now;
		else{
			if (possible) return possible;
			else{
				//printf("Element not found\n");
				return NULL;
			}
		}
	} else{
		printf("Tree is empty\n");
		return NULL;
	}
}


static void print_tree_rec(Tree_element *element, key_type left, key_type right){
	if (element){
		if (element->key<right){
			print_tree_rec(element->right, left, right);
		}
		if (element->key>=left && element->key<=right){
			printf("\nkey=%d | balance=%d\n", element->key, element->balance);
			list_write(element->data_list);
		}
		if (element->key>left){
			print_tree_rec(element->left, left, right);
		}
	}
}


int print_tree(Tree* tree){
	if (tree->root){
		key_type left, right;
		printf("Enter the left border\n");
		get_unint(&left);
		printf("Enter the right border\n");
		get_unint(&right);
		printf("\n~~~Tree:~~~\n");
		print_tree_rec(tree->root, left, right);
		printf("\n~~~~~~~~~~~\n");
		return 0;
	} else {
		printf("Tree is empty\n");
		printf("\n");
		return 1;
	}
}


static void del_tree_rec(Tree_element *element){
	if (element){
		del_tree_rec(element->left);
		del_tree_rec(element->right);
		del_element(element);
	}
}


void del_tree(Tree *tree){
	printf("Clearing...\n");
	if (tree->root){
		Tree_element *now = tree->root;
		del_tree_rec(now);
		tree->root = NULL;
		printf("Done!\n");
		return;
	} else{
		printf("The tree has already been cleared\n");
		return;
	}
}


static int int_len(int a){
	int len = 0;
	if (a<=0) len++;
	while(a){
		a/=10;
		len++;
	}
	return len;
}


static void print_beauty_tree_rec(Tree_element *element, int level, int mod){
	if (element){
		if (mod){
			for (int i=0; i<level-2; i++) printf(" ");
			printf("\\_");
		}
		printf("%d", element->key);
		level += 1 + int_len(element->key);
		if (element->left) printf("_");
		print_beauty_tree_rec(element->left, level, 0);
		if (element->right) printf("\n");
		print_beauty_tree_rec(element->right, level, 1);
	}
}


int print_beauty_tree(Tree *tree){
	if (tree->root){
		printf("Tree:\n");
		print_beauty_tree_rec(tree->root, 0, 0);
		printf("\n");
		return 0;
	} else {
		printf("Tree is empty\n");
		printf("\n");
		return 1;
	}
}


void input_from_file(Tree *tree){
	char name[] = "debug.txt";
	FILE *file;
	char *string;
	key_type key;
	if ((file = fopen(name, "r")) == NULL)
    {
		printf("File not found\n");
		return;
    }
	while(fscanf(file, "%u", &key)!=EOF){
		fscanf(file, "%*c");
		string = fget_str(file);
		insert(tree, key, string);
	}
	fclose(file);
	printf("Information has been loaded\n");
}


static void create_meta_code_for_gv_rec(Tree_element *element, FILE *file, int *counter){
	if (element){
		fprintf(file, "  \"%u\";\n", element->key);
		if (element->right || element->left){
			if (element->left) fprintf(file, "  \"%u\" -> \"%u\";\n", element->key, element->left->key);
			else {
				fprintf(file, "  \"%u\" -> \"NaE(%d)\";\n", element->key, *counter);
				(*counter)++;
			}
			if (element->right) fprintf(file, "  \"%u\" -> \"%u\";\n", element->key, element->right->key);
			else {
				fprintf(file, "  \"%u\" -> \"NaE(%d)\";\n", element->key, *counter);
				(*counter)++;
			}
		}
		create_meta_code_for_gv_rec(element->left, file, counter);
		create_meta_code_for_gv_rec(element->right, file, counter);
	}
}


static void create_meta_code_for_gv(Tree *tree){
	int counter = 0;
	if (tree->root){
		char name[] = "gv.gv";
		FILE *file;
		key_type key;
		if ((file = fopen(name, "w")) == NULL)
		{
			printf("File not found\n");
			return;
		}
		fprintf(file, "digraph Tree {\n");
		create_meta_code_for_gv_rec(tree->root, file, &counter);
		fprintf(file, "}\n");
		fclose(file);
	} else{
		printf("Tree is empty\n");
	}
}


void gv(Tree *tree){
	create_meta_code_for_gv(tree);
	system("./mkgv.sh");
}

