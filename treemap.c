#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * map = (TreeMap *)malloc(sizeof(TreeMap));
    map->root = NULL;
    map->current = NULL;
    map->lower_than = lower_than;
    return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode * parent = NULL;
    TreeNode * current = tree->root;

    while (current != NULL) {
        parent = current;
        if (is_equal(tree, key, current->pair->key)) return;
        if (tree->lower_than(key, current->pair->key))
            current = current->left;
        else
            current = current->right;
    }

    TreeNode * newNode = createTreeNode(key, value);
    newNode->parent = parent;

    if (parent == NULL)
        tree->root = newNode;
    else if (tree->lower_than(key, parent->pair->key))
        parent->left = newNode;
    else
        parent->right = newNode;

    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;
    while (x->left != NULL)
        x = x->left;
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL)
            tree->root = NULL;
        else if (node->parent->left == node)
            node->parent->left = NULL;
        else
            node->parent->right = NULL;
    }
    else if (node->left == NULL || node->right == NULL) {
        TreeNode * child = node->left ? node->left : node->right;
        if (node->parent == NULL) {
            tree->root = child;
        } else if (node->parent->left == node) {
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }
        child->parent = node->parent;
    }
    else {
        TreeNode * minNode = minimum(node->right);
        node->pair = minNode->pair;
        removeNode(tree, minNode);
    }
    free(node);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode * current = tree->root;
    while (current != NULL) {
        if (is_equal(tree, key, current->pair->key)) {
            tree->current = current;
            return current->pair;
        }
        if (tree->lower_than(key, current->pair->key))
            current = current->left;
        else
            current = current->right;
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * node = minimum(tree->root);
    if (node == NULL) return NULL;
    tree->current = node;
    return node->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode * node = tree->current;

    if (node == NULL) return NULL;

    if (node->right != NULL) {
        node = minimum(node->right);
    } else {
        TreeNode * parent = node->parent;
        while (parent != NULL && parent->right == node) {
            node = parent;
            parent = parent->parent;
        }
        node = parent;
    }

    tree->current = node;
    return node ? node->pair : NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
