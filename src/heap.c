#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "../include/heap.h"

/* =========================================================
 * heap.c
 * Min-heap de perechi (node_id, dist) pentru Dijkstra.
 * Folosește un tablou dinamic cu indexarea standard
 * părinte/copil:
 *   parinte(i)       = (i - 1) / 2
 *   copil_stang(i)   = 2 * i + 1
 *   copil_drept(i)   = 2 * i + 2
 * ========================================================= */

/* ----------------------------------------------------------
 * Helper intern de swap
 * ---------------------------------------------------------- */

static void swap(HeapNode *a, HeapNode *b) __attribute__((unused));
static void swap(HeapNode *a, HeapNode *b)
{
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

/* ----------------------------------------------------------
 * Funcții de sift
 * ---------------------------------------------------------- */

static void sift_up(MinHeap *h, int i) __attribute__((unused));
static void sift_up(MinHeap *h, int i)
{
    while(i > 0)
    {
        int parent = (i-1)/2;
        if(h->data[parent].dist <= h->data[i].dist)
            break;

        swap(&h->data[parent],&h->data[i]);
        i = parent;
    }
}

static void sift_down(MinHeap *h, int i) __attribute__((unused));
static void sift_down(MinHeap *h, int i)
{
    while(1)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int minn = i;

        if(left < h->size && h->data[left].dist < h->data[minn].dist)
            minn = left;
        if(right < h->size && h->data[right].dist < h->data[minn].dist)
            minn = right;
        if(minn == i)
            break;
        swap(&h->data[i],&h->data[minn]);
        i = minn;
    }
}

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

MinHeap *heap_create(int initial_capacity)
{
    if(initial_capacity <= 0)
        return NULL;

    MinHeap *h = malloc(sizeof(MinHeap));
    if(h == NULL)
        return NULL;

    h->data = malloc(sizeof(HeapNode) * initial_capacity);
    if(h->data == NULL)
    {
        free(h);
        return NULL;
    }
    h->size = 0;
    h->capacity = initial_capacity;
    return h;
}

void heap_free(MinHeap *h)
{
    if(h == NULL)
        return;
    free(h->data);
    free(h);
}

/* ----------------------------------------------------------
 * Operații de bază
 * ---------------------------------------------------------- */

int heap_push(MinHeap *h, int node_id, float dist)
{
    if(h == NULL)
        return -1;
    if(h->size == h->capacity)
    {
        int new_capacity = h->capacity * 2;
        HeapNode *new_data = realloc(h->data,sizeof(HeapNode) * new_capacity);
        if(new_data == NULL)
            return -1;
        h->data = new_data;
        h->capacity = new_capacity;
    }

    int i = h->size;
    h->data[i].node_id = node_id;
    h->data[i].dist = dist;
    h->size++;
    sift_up(h,i);
    return 0;
}

HeapNode heap_pop(MinHeap *h)
{
    HeapNode rezultat = {-1, FLT_MAX};
    if(h == NULL || h->size == 0)
        return rezultat;

    rezultat = h->data[0];
    h->data[0] = h->data[h->size - 1];
    h->size--;
    if(h->size > 0)
        sift_down(h,0);
    return rezultat;
}

int heap_is_empty(const MinHeap *h)
{
    if(h == NULL)
        return 1;
    return h->size == 0;
}

int heap_decrease_key(MinHeap *h, int node_id, float new_dist)
{
    if(h == NULL)
        return -1;
    for(int i = 0;i < h->size;i++)
    {
        if(h->data[i].node_id == node_id)
        {
            if(new_dist >= h->data[i].dist)
                return 0;
            h->data[i].dist = new_dist;
            sift_up(h,i);
            return 0;
        }
    }
    return 1;
}
