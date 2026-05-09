#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/queue.h"

/* =========================================================
 * queue.c
 * Coadă FIFO susținută de o listă simplu înlănțuită.
 * Stochează copii alocate pe heap ale șirurilor.
 * ========================================================= */

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

Queue *queue_create(void)
{
    Queue *q = malloc(sizeof(Queue));
    if(q == NULL)
        return NULL;

    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

void queue_free(Queue *q)
{
    if(q == NULL)
        return;

    QueueNode *curr = q->front;

    while(curr != NULL)
    {
        QueueNode *next = curr->next;
        free(curr->data);
        free(curr);
        curr = next;
    }
    free(q);
}

/* ----------------------------------------------------------
 * Mutație
 * ---------------------------------------------------------- */

int queue_enqueue(Queue *q, const char *data)
{
    if(q == NULL || data == NULL)
        return -1;

    QueueNode *node = malloc(sizeof(QueueNode));
    
    if(node == NULL)
        return -1;

    node->data = malloc(strlen(data)+1);

    if(node->data == NULL)
    {
        free(node);
        return -1;
    }

    strcpy(node->data,data);
    node->next = NULL;

    if(q->rear == NULL)
    {
        q->front = node;
        q->rear = node;
    }
    else
    {
        q->rear->next = node;
        q->rear = node;
    }

    q->size++;
    return 0;
}

char *queue_dequeue(Queue *q)
{
    if(q == NULL || q->front == NULL)
        return NULL;

    QueueNode *tmp = q->front;
    char *data = tmp->data;
    q->front = q->front->next;

    if(q->front == NULL)
        q->rear = NULL;

    free(temp);
    q->size--;
    return data;
}

/* ----------------------------------------------------------
 * Inspecție
 * ---------------------------------------------------------- */

int queue_is_empty(const Queue *q)
{
    return (q == NULL || q->size == 0);
}

void queue_print(const Queue *q)
{
    if(q == NULL)
        return;

    QueueNode *curr = q->front;

    while(curr)
    {
        printf("%s\n",curr->data);
        curr = curr->next;
    }
}
