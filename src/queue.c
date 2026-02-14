#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cbarroso/constants.h>
#include <cbarroso/queue.h>

int8_t Queue__enqueue(Queue *self, void *value, size_t valueSize)
{
    QueueNode *newNode = malloc(sizeof(QueueNode));

    if (newNode == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the new node\n");
        return CBR_ERROR;
    }

    newNode->value = malloc(valueSize);

    if (newNode->value == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the new queue node value\n");
        return CBR_ERROR;
    }

    memcpy(newNode->value, value, valueSize);
    newNode->valueSize = valueSize;
    newNode->next = NULL;

    if (self->numberOfNodes > 0)
    {
        self->tail->next = newNode;
    }
    else
    {
        self->head = newNode;
    }

    self->tail = newNode;
    self->numberOfNodes++;

    return CBR_SUCCESS;
}

Queue *Queue__new()
{
    Queue *queue = malloc(sizeof(Queue));

    if (queue == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the queue\n");
        return NULL;
    }

    queue->head = NULL;
    queue->tail = NULL;
    queue->numberOfNodes = 0;

    return queue;
}

int8_t Queue__dequeue(Queue *self, void **valueAddress)
{
    if (self->head == NULL)
    {
        fprintf(stderr, "Empty queue\n");
        return CBR_ERROR;
    }

    QueueNode *nodeToFree = self->head;
    *valueAddress = self->head->value;
    self->head = self->head->next;
    self->numberOfNodes--;

    if (self->head == NULL)
    {
        self->tail = NULL;
    }

    free(nodeToFree);

    return CBR_SUCCESS;
}
