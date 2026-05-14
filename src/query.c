#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "../include/query.h"
#include "../include/heap.h"
#include "../include/graph.h"
#include "../include/bst.h"

/* =========================================================
 * query.c
 * Parsarea și dispatcharea interogărilor de graf:
 *   EXISTS, EDGE, NEIGHBORS, PATH (BFS), DIJKSTRA
 * ========================================================= */

/* ----------------------------------------------------------
 * Helper de parsare
 * ---------------------------------------------------------- */

QueryType parse_query_type(const char *line)
{
    if(line == NULL)
        return Q_UNKNOWN;

    if(strncmp(line,"EXISTS",6) == 0)
        return Q_EXISTS;
    if(strncmp(line,"EDGE",4) == 0)
        return Q_EDGE;
    if(strncmp(line,"NEIGHBORS",9) == 0)
        return Q_NEIGHBORS;
    if(strncmp(line,"PATH",4) == 0)
        return Q_PATH;
    if(strncmp(line,"DIJKSTRA",8) == 0)
        return Q_DIJKSTRA;

    return Q_UNKNOWN;
}

/* ----------------------------------------------------------
 * EXISTS
 * ---------------------------------------------------------- */

void process_exists(const BST *tree, const char *name)
{
    GraphNode *node = bst_search(tree,name);
    if(node == NULL)
        printf("EXISTS %s: NU\n",name);
    else
        printf("EXISTS %s: DA\n",name);
}

/* ----------------------------------------------------------
 * EDGE
 * ---------------------------------------------------------- */

void process_edge(const Graph *g, const BST *tree,
                  const char *src_name, const char *dest_name)
{
    GraphNode *src = bst_search(tree,src_name);
    GraphNode *dest = bst_search(tree,dest_name);

    if(src == NULL || dest == NULL)
        {
            printf("EDGE %s %s: NU\n",src_name,dest_name);
            return;
        }
    EdgeNode *e = src->edges;
    while(e)
    {
        if(e->dest_id == dest->entity.id)
        {
            printf("EDGE %s %s: DA\n",src_name,dest_name);
            return;
        }
        e = e->next;
    }

    printf("EDGE %s %s: NU\n",src_name,dest_name);
}

/* ----------------------------------------------------------
 * NEIGHBORS
 * ---------------------------------------------------------- */

void process_neighbors(const Graph *g, const BST *tree,
                       const char *name)
{
    GraphNode *node = bst_search(tree,name);

    if(node == NULL)
    {
        printf("NEIGHBORS %s: NULL\n",name);
        return;
    }

    EdgeNode *e = node->edges;

    if(e == NULL)
    {
        printf("NEIGHBORS %s: NULL\n",name);
        return;
    }

    printf("NEIGHBORS %s:",name);

    while(e)
    {
        GraphNode *dest = graph_get_node(g,e->dest_id);

        if(dest != NULL)
            printf(" %s",dest->entity.name);

        e = e->next;
    }
    printf("\n");
}

/* ----------------------------------------------------------
 * PATH (BFS)
 * ---------------------------------------------------------- */

void process_path_bfs(const Graph *g, const BST *tree,
                      const char *src_name, const char *dest_name)
{
    GraphNode *src = bst_search(tree,src_name);
    GraphNode *dest = bst_search(tree,dest_name);

    if(src == NULL || dest == NULL)
    {
        printf("PATH %s %s: NU\n",src_name,dest_name);
        return;
    }

    int n = g->size;
    int *visited = calloc(n,sizeof(int));
    int *parents = malloc(n * sizeof(int));
    for(int i = 0;i < n;i++)
        parents[i] = -1;

    Queue *q = queue_create();

    int s = src->entity.id;
    int d = dest->entity.id;

    char buf[32];
    snprintf(buf,sizeof(buf),"%d",s);
    queue_enqueue(q,buf);

    visited[s] = 1;
    
    while(!queue_is_empty(q))
    {
        char *a = queue_dequeue(q);
        int u = atoi(a);
        free(a);

        if(u == d)
            break;

        GraphNode *node = graph_get_node(g,u);  
        EdgeNode *e;

        for(e = node->edges;e != NULL;e = e->next)
        {
            int v = e->dest_id;

            if(!visited[v])
            {
                visited[v] = 1;
                parents[v] = u;

                char tmp[32];
                snprintf(tmp,sizeof(tmp),"%d",v);
                queue_enqueue(q,tmp);
            }
        }
    }

    if(visited[d])
    {
        int path[n];
        int len = 0;

        for(int p = d;p != -1;p = parents[p])
        path[len++] = p;

    printf("PATH %s %s:",src_name,dest_name);

    for(int i = len - 1;i >= 0;i--)
    {
        printf(" %s",graph_get_node(g,path[i])->entity.name);
        if(i > 0)
            printf(" ->");

    }

    printf("\n");
    }
    else
        printf("PATH %s %s: NU\n",src_name,dest_name);

    free(visited);
    free(parents);
    queue_free(q);
}

/* ----------------------------------------------------------
 * DIJKSTRA
 * ---------------------------------------------------------- */

void process_dijkstra(const Graph *g, const BST *tree,
                      const char *src_name, const char *dest_name)
{
    GraphNode *src = bst_search(tree,src_name);
    GraphNode *dest = bst_search(tree,dest_name);

    if(src == NULL || dest == NULL)
    {
        printf("DIJKSTRA %s %s: NU\n",src_name,dest_name);
        return;
    }

    int n = g->size;
    int s = src->entity.id;
    int d = dest->entity.id;

    float *dist = malloc(n * sizeof(float));
    int *parent = malloc(n * sizeof(int));
    int *visited = calloc(n,sizeof(int));

    for(int i = 0;i < n;i++)
    {
        dist[i] = FLT_MAX;
        parent[i] = -1;
    }

    MinHeap *h = heap_create(n);

    dist[s] = 0;
    heap_push(h,s,0);

    while(!heap_is_empty(h))
    {
        HeapNode hn = heap_pop(h);
        int u = hn.node_id;

        if(visited[u])
            continue;

        visited[u] = 1;

        if(u == d)
            break;

        GraphNode *node = graph_get_node(g,u);
        EdgeNode *e;
        for(e = node->edges;e != NULL;e = e->next)
        {
            int v = e->dest_id;
            float new_dist = dist[u] + e->cost;

            if(new_dist < dist[v])
            {
                dist[v] = new_dist;
                parent[v] = u;
                heap_push(h,v,new_dist);
            }
        }
    }

    if(dist[d] == FLT_MAX)
        printf("DIJKSTRA %s %s: NU\n",src_name,dest_name);
    else
    {
        int path[n];
        int len = 0;

        for(int p = d;p != -1;p = parent[p])
            path[len++] = p;

        printf("DIJKSTRA %s %s: COST = %.2f; DRUM =",src_name,dest_name,dist[d]);

        for(int i = len-1;i >= 0;i--)
        {
            printf(" %s",graph_get_node(g,path[i])->entity.name);
            if(i > 0)
                printf(" ->");
        }
        printf("\n");
    }

    free(dist);
    free(parent);
    free(visited);
    heap_free(h);
}

/* ----------------------------------------------------------
 * Dispatcher batch
 * ---------------------------------------------------------- */

void process_all_queries(Queue *q, const Graph *g, const BST *tree)
{
    while(!queue_is_empty(q))
    {
        char *line = queue_dequeue(q);
        if(line == NULL)
            continue;
        QueryType type = parse_query_type(line);
        char a[100], b[100];

        switch(type)
        {
            case Q_EXISTS:
                if(sscanf(line,"EXISTS %99s",a) == 1)
                    process_exists(tree,a);
                break;
            case Q_EDGE:
                if(sscanf(line,"EDGE %99s %99s",a,b) == 2)
                    process_edge(g,tree,a,b);
                break;
            case Q_NEIGHBORS:
                if(sscanf(line,"NEIGHBORS %99s",a) == 1)
                    process_neighbors(g,tree,a);
                break;
            case Q_PATH:
                if(sscanf(line,"PATH %99s %99s",a,b) == 2)
                    process_path_bfs(g,tree,a,b);
                break;
            case Q_DIJKSTRA:
                if(sscanf(line,"DIJKSTRA %99s %99s",a,b) == 2)
                    process_dijkstra(g,tree,a,b);
                break;
            default:
                printf("Query necunoscut: %s\n",line);
                break;
        }
        free(line);
    }
}
