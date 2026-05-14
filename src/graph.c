#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/graph.h"

/* =========================================================
 * graph.c
 * Implementarea grafului direcționat ponderat cu liste de
 * adiacență.
 * ========================================================= */

/* ----------------------------------------------------------
 * Funcții ajutătoare pentru relații
 * ---------------------------------------------------------- */

const char *relation_type_to_str(RelationType type)
{
    switch(type)
    {
        case WORKS_AT:
            return "works_at";
        case FRIEND_OF:
            return "friend_of";
        case LOCATED_IN:
            return "located_in";
        case PARTICIPATES_IN:
            return "participates_in";
        default:
            return "unknown";
    }
}

RelationType str_to_relation_type(const char *str)
{
    if(str == NULL)
        return (RelationType)-1;

    if(strcmp(str,"works_at") == 0)
        return WORKS_AT;
    if(strcmp(str,"located_in") == 0)
        return LOCATED_IN;
    if(strcmp(str,"friend_of") == 0)
        return FRIEND_OF;
    if(strcmp(str,"participates_in") == 0)
        return PARTICIPATES_IN;
    
    return (RelationType)-1;

}

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

Graph *graph_create(int initial_capacity)
{
    if(initial_capacity <= 0)
        return NULL;

    Graph *g = malloc(sizeof(Graph));
    if(g == NULL)
        return NULL;

    g->nodes = malloc(sizeof(GraphNode) * initial_capacity);
    if(g->nodes == NULL)
    {
        free(g);
        return NULL;
    }
    g->size = 0;
    g->capacity = initial_capacity;

    return g;
}

void graph_free(Graph *g)
{
    if(g == NULL)
        return;

    for(int i = 0;i < g->size;i++)
    {
        GraphNode *node = &g->nodes[i];
        EdgeNode *e = node->edges;
        while(e)
        {
            EdgeNode *urm = e->next;
            free(e);
            e = urm;
        }
        entity_free(&node->entity);
    }
    free(g->nodes);
    free(g);
}

/* ----------------------------------------------------------
 * Mutație
 * ---------------------------------------------------------- */

int graph_add_node(Graph *g, const char *name, EntityType type)
{
    if(g == NULL || name == NULL)
        return -1;

    if(g->size == g->capacity)
        {
            int new_capacity = g->capacity * 2;
            GraphNode *newnodes = realloc(g->nodes,sizeof(GraphNode)*new_capacity);
            if(newnodes == NULL)
                return -1;
            g->nodes = newnodes;
            g->capacity = new_capacity;
        }
        int id = g->size;
        Entity *e = entity_create(name,type,id);
        if(e == NULL)
            return -1;

        g->nodes[id].entity = *e;
        free(e);
        g->nodes[id].edges = NULL;
        g->size++;
        return id;
}

int graph_add_edge(Graph *g, int src_id, int dest_id,
                   RelationType type, float cost)
{
    if(g == NULL)
        return -1;

    if(src_id < 0 || src_id >= g->size)
        return -1;

    if(dest_id < 0 || dest_id >= g->size)
        return -1;

    EdgeNode *new_edge = malloc(sizeof(EdgeNode));
    if(new_edge == NULL)
        return -1;

    new_edge->dest_id = dest_id;
    new_edge->type = type;
    new_edge->cost = cost;
    new_edge->next = NULL;

    EdgeNode **head = &g->nodes[src_id].edges;

    if(*head == NULL)
        {
            *head = new_edge;
        }
    else
        {
            EdgeNode *curr = *head;
            while(curr->next != NULL)
                curr = curr->next;
            curr->next = new_edge;
        }

        return 0;
}

/* ----------------------------------------------------------
 * Funcții ajutătoare pentru interogare
 * ---------------------------------------------------------- */

int graph_find_id(const Graph *g, const char *name)
{
    if(g == NULL || name == NULL)
        return -1;

    for(int i = 0;i < g->size;i++)
    {
        if(strcmp(g->nodes[i].entity.name,name) == 0)
            return g->nodes[i].entity.id;
    }

    return -1;
}

GraphNode *graph_get_node(const Graph *g, int id)
{
    if(g == NULL)
        return NULL;
    if(id < 0 || id >= g->size)
        return NULL;

    return &g->nodes[id];
}

/* ----------------------------------------------------------
 * Afișare
 * ---------------------------------------------------------- */

void graph_print(const Graph *g)
{
    if(g == NULL)
        return;
    for(int i = 0;i < g->size;i++)
    {
        GraphNode *node = &g->nodes[i];
        printf("%d %s:",node->entity.id,node->entity.name);

        EdgeNode *edge = node->edges;

        while(edge != NULL)
        {
            printf(" [%d %s %.2f]",edge->dest_id,relation_type_to_str(edge->type),edge->cost);
            edge = edge->next;
        }
        printf("\n");
    }
}
