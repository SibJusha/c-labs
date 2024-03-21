#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    int height;
    char *string;
    struct node *left;
    struct node *right;
} node;
// a->string == (*a).string
void swap_node(node* a, node *b) {
    node temp;
    temp.string = a->string;
    a->string = b->string;
    b->string = temp.string;

    temp.right = a->right;
    a->right = b->right;
    b->right = temp.right;

    temp.left = a->left;
    a->left = b->left;
    b->left = temp.left;
}

void scan(char** str, int *size) {
    char p;
    int i = 0;
    p = getchar();
    while (p != ' ' && p != 0 && p != '\n') {
        if (i == *size) {
            *str = (char*) realloc(*str, 2 * (*size) * sizeof(char));
            *size *= 2;
        }
        (*str)[i++] = p;
        p = getchar();
    }
    if (i == *size) {
        *str = (char*) realloc(*str, (*size)*sizeof(char) + 1);
    }
    (*str)[i] = 0;
}

int height(node* root) {
    if (root == NULL) {
        return -1;
    }
    if (root->left == NULL && root->right == NULL) {
        return 0;
    }
    if (root->left == NULL && root->right != NULL) {
        return 1 + root->right->height;
    }
    if (root->left != NULL && root->right == NULL) {
        return 1 + root->left->height;
    }
    if (root->left->height > root->right->height) {
        return 1 + root->left->height;
    }
    return 1 + root->right->height;
}

node* rotate_right_right(node* root) {
    node *tmp;
    tmp = root->right;
    root->right = tmp->left;
    tmp->left = root;
    root->height = height(root);
    tmp->height = height(tmp);
    return tmp;
}

node* rotate_left_left(node* root) {
    node *tmp;
    tmp = root->left;
    root->left = tmp->right;
    tmp->right = root;
    root->height = height(root);
    tmp->height = height(tmp);
    return tmp;
}

node* rotate_left_right(node* root) {
    root->left = rotate_right_right(root->left);
    root = rotate_left_left(root);
    return root;
}

node* rotate_right_left(node* root) {
    root->right = rotate_left_left(root->right);
    root = rotate_right_right(root);
    return root;
}

int balance_factor(node* root) {
    int r, l;
    if (root == NULL) {
        return 0;
    }
    if (root->left != NULL) {
        l = 1 + root->left->height;
    } else {
        l = 0;
    }
    if (root->right != NULL) {
        r = 1 + root->right->height;
    } else {
        r = 0;
    }
    return l - r;
}

node* add_node(node* root, char *word) {
    if (root == NULL) {
        root = (node*) malloc(sizeof(node));
        root->string = (char*) malloc(strlen(word) + 1);
        strcpy(root->string, word);
        root->left = NULL;
        root->right = NULL;
        root->height = 0;
        return root;
    }
    int compr = strcmp(root->string, word);
    if (compr < 0) {
        root->right = add_node(root->right, word);
        if (balance_factor(root) == -2) {
            if (balance_factor(root->right) <= 0) {
                root = rotate_right_right(root);
            } else {
                root = rotate_right_left(root);
            }
        }
    }
    if (compr > 0) {
        root->left =  add_node(root->left, word);
        if (balance_factor(root) == 2) {
            if (balance_factor(root->left) >= 0) {
                root = rotate_left_left(root);
            } else {
                root = rotate_left_right(root);
            }
        }
    }
    root->height = height(root);
    return root;
}

node* minimal(node* root) {//самая левая вершина у root
    if (root == NULL) {
        return NULL;
    }
    if (root->left == NULL) {
        return root;
    }
    return minimal(root->left);
}

node* delete_node(node* root, char* word) {
    if (root == NULL) {
        return root;
    }
    int compr = strcmp(root->string, word);
    if (compr == 0) {
        if (root->left == NULL && root->right == NULL) {
            free(root->string);
            free(root);
            return NULL;
        }
        if (root->left == NULL && root->right != NULL) {
            node *t = root->right;
            swap_node(root, root->right);
            free(t->string);
            free(t);
            root->height = height(root);
            return root;
        }
        if (root->left != NULL && root->right == NULL) {
            node* t = root->left;
            swap_node(root, root->left);
            free(t->string);
            free(t);
            root->height = height(root);
            return root;
        }
        node* min_node = minimal(root->right);
        strcpy(root->string, min_node->string);
        root->right = delete_node(root->right, min_node->string);
        if (balance_factor(root) == 2) {
            if (balance_factor(root->left) >= 0) {
                root = rotate_left_left(root);
            } else {
                root = rotate_left_right(root);
            }
        }
        root->height = height(root);
        return root;
    }
    if (compr > 0) {
        root->left = delete_node(root->left, word);
        if (balance_factor(root) == -2) {
            if (balance_factor(root->right) <= 0) {
                root = rotate_right_right(root);
            } else {
                root = rotate_right_left(root);
            }
        }
    }
    if (compr < 0) {
        root->right = delete_node(root->right, word);
        if (balance_factor(root) == 2) {
            if (balance_factor(root->left) >= 0) {
                root = rotate_left_left(root);
            } else {
                root = rotate_left_right(root);
            }
        }
    }
    root->height = height(root);
    return root;
}

int count_nodes(node* root) {
    if (root == NULL) {
        return 0;
    }
    if (root->right == NULL && root->left == NULL) {
        return 1;
    }
    return count_nodes(root->left) + count_nodes(root->right) + 1;
}

void level_nodes(node* root, int goal, int current) {
    if (root == NULL || goal < current) {
        return;
    }
    if (goal == current) {
        printf("%s ", root->string);
        return;
    }
    if (root->left != NULL) {
        level_nodes(root->left, goal, current + 1);
    }
    if (root->right != NULL) {
        level_nodes(root->right, goal, current + 1);
    }
}

void free_tree(node* root) {
    if (root == NULL) {
        return;
    }
    if (root->left != NULL) {
        free_tree(root->left);
    }
    if (root->right != NULL) {
        free_tree(root->right);
    }
    free(root->string);
    free(root);
}

int main() {
    node *tree = NULL;
    scanf("TEXT:\n");
    int size = 1;
    char *s = (char*) malloc(sizeof(char));
    scan(&s, &size);
    while (strcmp("DELETE:", s) != 0) {
        tree = add_node(tree, s);
        scan(&s, &size);
    }
    scan(&s, &size);
    while (strcmp("LEVEL:", s) != 0) {
        tree = delete_node(tree, s);
        scan(&s, &size);
    }
    free(s);
    int level;
    scanf("%d", &level);
    printf("%d\n", count_nodes(tree));
    level_nodes(tree, level, 0);
    free_tree(tree);
    return 0;
}
