#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/bst.h"

/* =========================================================
 * bst.c
 * BST indexat după numele entității; nodurile conțin pointeri
 * în tabloul de noduri al grafului (fără duplicarea datelor).
 * ========================================================= */

/* ----------------------------------------------------------
 * Funcții auxiliare interne (statice, neexportate)
 * ---------------------------------------------------------- */

/* Eliberează recursiv nodii BSTNode (nu GraphNode-urile spre care
 * pointează — acelea aparțin Grafului). */
static void bst_free_recursive(BSTNode *node)
    __attribute__((unused));
static void bst_free_recursive(BSTNode *node)
{
    if(node == NULL)
        return;

    bst_free_recursive(node->left);
    bst_free_recursive(node->right);

    free(node);
}

/* Inserare recursivă; returnează rădăcina (posibil nouă) a subarbore. */
static BSTNode *bst_insert_recursive(BSTNode *node, GraphNode *gn)
    __attribute__((unused));

static BSTNode *bst_insert_recursive(BSTNode *node, GraphNode *gn)
{
    if (node == NULL)
    {
        BSTNode *newnode = malloc(sizeof(BSTNode));
        newnode->graph_node = gn;
        newnode->left = NULL;
        newnode->right = NULL;
        return newnode;
    }

    if(strcmp(gn->entity.name,node->graph_node->entity.name) < 0)
        node->left = bst_insert_recursive(node->left,gn);
    else if(strcmp(gn->entity.name,node->graph_node->entity.name) > 0)
        node->right = bst_insert_recursive(node->right,gn);

    return node;
}

/* Căutare recursivă; returnează GraphNode* sau NULL. */
static GraphNode *bst_search_recursive(const BSTNode *node, const char *name)
    __attribute__((unused));

static GraphNode *bst_search_recursive(const BSTNode *node, const char *name)
{
    if(node == NULL)
        return NULL;
    int x = strcmp(name,node->graph_node->entity.name);

    if(x < 0)
        return bst_search_recursive(node->left,name);

    if (x > 0)
        return bst_search_recursive(node->right,name);

    return node->graph_node;

}

/* Traversare inordine pentru afișare. */
static void bst_inorder_recursive(const BSTNode *node)
    __attribute__((unused));
static void bst_inorder_recursive(const BSTNode *node)
{
    if(node == NULL)
        return;

    bst_inorder_recursive(node->left);

    printf("%s\n",node->graph_node->entity.name);

    bst_inorder_recursive(node->right);
}

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

/* Alocă și inițializează un BST gol. Returnează NULL la eșec. */
BST *bst_create(void)
{
    BST *p = malloc(sizeof(BST));

    if(p == NULL)
        return NULL;

    p->root = NULL;

    return p;
}

/* Eliberează recursiv toți nodii BST, apoi structura BST. */
void bst_free(BST *tree)
{
    if(tree == NULL)
        return;

    bst_free_recursive(tree->root);

    free(tree);
}

/* ----------------------------------------------------------
 * Mutație
 * ---------------------------------------------------------- */

int bst_insert(BST *tree, GraphNode *graph_node)
{   if(tree == NULL || graph_node == NULL)
        return -1;
    
    tree->root = bst_insert_recursive(tree->root,graph_node);

    return 0;
}

/* ----------------------------------------------------------
 * Interogare
 * ---------------------------------------------------------- */

GraphNode *bst_search(const BST *tree, const char *name)
{
    if(tree == NULL)
        return NULL;
    return bst_search_recursive;
}

/* ----------------------------------------------------------
 * Afișare
 * ---------------------------------------------------------- */

void bst_inorder_print(const BST *tree)
{
    if(tree == NULL)
        return;

    bst_inorder_print(tree);
}
