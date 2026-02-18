#ifndef CBARROSO_QUEUE_H
#define CBARROSO_QUEUE_H

#include <stddef.h>
#include <stdint.h>

typedef struct QueueNode
{
    void *value;
    size_t valueSize;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *head;
    QueueNode *tail;
    size_t numberOfNodes;
} Queue;

Queue *Queue__new();
int8_t Queue__enqueue(Queue *self, void *value, size_t valueSize);
int8_t Queue__dequeue(Queue *self, void **valueAddress);
void Queue__del(Queue *self);

#endif
